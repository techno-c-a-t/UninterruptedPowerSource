#include "engine.h"
#include <chrono>
#include <windows.h>
#include <timeapi.h>

void PSEngine::SetSessionCore(SessionCore *session) {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  activeSessionCore = session;
}

unsigned int PSEngine::GetCalculatedStepMs() const {
  DWORD baud = device.GetSettings().BaudRate;
  if (baud == 0) baud = CBR_9600;
  double transTimeMs = (config::engine::STEP_BASE_TRANSACTION_BYTES * 1000.0) / static_cast<double>(baud);
  unsigned int delayMs = static_cast<unsigned int>(transTimeMs + config::engine::STEP_HARDWARE_LAG_MS);
  if (delayMs < config::engine::STEP_MIN_DELAY_MS) delayMs = config::engine::STEP_MIN_DELAY_MS;
  if (delayMs > config::engine::STEP_MAX_DELAY_MS) delayMs = config::engine::STEP_MAX_DELAY_MS;
  return delayMs;
}

void PSEngine::StartLoop() {
  if (isRunning.load()) return;
  isRunning = true;
  workerThread = std::thread(&PSEngine::WorkerLoop, this);
}

void PSEngine::StopLoop() {
  if (!isRunning.load()) return;
  isRunning = false;
  if (workerThread.joinable()) {
    workerThread.join();
  }
}

void PSEngine::WorkerLoop() {
  timeBeginPeriod(1);
  auto next_wake = std::chrono::steady_clock::now();

  while (isRunning.load()) {
    unsigned int stepMs = GetCalculatedStepMs();
    next_wake += std::chrono::milliseconds(stepMs);

    float v = 0.0f;
    float c = 0.0f;
    uint64_t v_time = 0;
    uint64_t c_time = 0;
    bool isConn = false;

    {
      std::lock_guard<std::recursive_mutex> lock(engineMutex);
      Tick();
      isConn = Connected();
      v = cachedVoltage;
      c = cachedCurrent;
      v_time = lastVoltageUpdate;
      c_time = lastCurrentUpdate;
    }

    if (isConn && activeSessionCore && v_time > 0 && c_time > 0) {
      uint64_t avg_time_us = (v_time + c_time) / 2;
      activeSessionCore->Update(v, c, avg_time_us);
    }

    std::this_thread::sleep_until(next_wake);
  }

  timeEndPeriod(1);
}

// === Реализация ErrorLogBuffer ===

void PSEngine::ErrorLogBuffer::push(CommandID cmd, const std::string &msg,
                                    size_t tick, unsigned duration,
                                    uint64_t timestamp) {
  buffer[head] = {cmd, msg, tick, duration, timestamp};
  head = (head + 1) % buffer.size();
  if (head == 0)
    full = true;
}

std::vector<PSEngine::RuntimeError> PSEngine::ErrorLogBuffer::getAll() const {
  std::vector<RuntimeError> result;
  size_t count = full ? buffer.size() : head;
  result.reserve(count);

  if (full) {
    for (size_t i = head; i < buffer.size(); ++i)
      result.push_back(buffer[i]);
  }
  for (size_t i = 0; i < head; ++i)
    result.push_back(buffer[i]);

  return result;
}

// === Реализация PSEngine ===

PSEngine::PSEngine(float minVoltage, float minCurrent, float maxVoltage, float maxCurrent)
	: minV(minVoltage), minC(minCurrent), maxV(maxVoltage), maxC(maxCurrent) {
  // Инициализация слотов
  for (auto &slot : commandRegistry) {
    slot.payload = 0.0f;
    slot.isQueued = false;
    slot.isCritical = false;
    slot.retryCount = 0;
  }

  // Назначаем критические команды (идут в highQueue)
  commandRegistry[(int)CommandID::CMD_SET_OUTPUT].isCritical = true;
  commandRegistry[(int)CommandID::CMD_SET_LOCK].isCritical = true;
  commandRegistry[(int)CommandID::CMD_CONNECT].isCritical = true;
  commandRegistry[(int)CommandID::CMD_DISCONNECT].isCritical = true;
}

// Приватный хелпер для добавления/обновления команд
void PSEngine::UpdateSlot(CommandID id, float val) {
  int idx = static_cast<int>(id);
  engine_logic::trace(std::string("Pushed commnand number") +
                      std::to_string(idx));
  // 1. Всегда обновляем значение (схлопывание)
  commandRegistry[idx].payload = val;
  commandRegistry[idx].retryCount = 0; // Сброс попыток для новой команды

  // 2. Если уже в очереди - выходим (значение мы обновили выше)
  if (commandRegistry[idx].isQueued) {
    engine_logic::trace(std::string("Command ") + std::to_string(idx) +
                        " is queued. Exiting...");
    return;
  }

  // 3. Добавляем в нужную очередь
  commandRegistry[idx].isQueued = true;
  if (commandRegistry[idx].isCritical) {
    highQueue.push_back(id);
  } else {
    queue.push_back(id);
  }
}

// === API Methods ===


void PSEngine::SetTargetVoltage(float val, bool sync) {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  if (sync) {
    if (val <= maxV && val >= minV) {
		device.SetVoltage(val);
      } else {
        pso_logic::warning("Discarded attempt to set voltage in sync mode out of [min, max] bounaties.");
      }
  } else {
	UpdateSlot(CommandID::CMD_SET_VOLTAGE, val);
  }
}

void PSEngine::SetTargetCurrent(float val, bool sync) {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  if (sync) {
    if (val <= maxC && val >= minC) {
		device.SetCurrent(val);
      } else {
        pso_logic::warning("Discarded attempt to set current in sync mode out of [min, max] bounaties.");
      }
  } else {
	UpdateSlot(CommandID::CMD_SET_CURRENT, val);
  }
}

void PSEngine::SetLock(bool enabled, bool sync) {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  if (sync) {
	device.SetLock(enabled);
  } else {
    UpdateSlot(CommandID::CMD_SET_LOCK, enabled ? 1.0f : 0.0f);
  }
}

void PSEngine::SetOutput(bool enabled, bool sync) {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  if (sync) {
	device.SetOut(enabled);
  } else {
    UpdateSlot(CommandID::CMD_SET_OUTPUT, enabled ? 1.0f : 0.0f);
  }
}

void PSEngine::Connect(const std::string &port) {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  targetPortName = port;                    // Сохраняем имя порта
  UpdateSlot(CommandID::CMD_CONNECT, 0.0f); // Payload не важен
}

void PSEngine::Disconnect() {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  UpdateSlot(CommandID::CMD_DISCONNECT, 0.0f);
}
// === Логика исполнения ===

void PSEngine::Tick() {
  currentTick++;

  // 1. Выбираем команду для исполнения
  CommandID idToExec = CommandID::CMD_NONE;
  bool isHighPrio = false;
  if (!highQueue.empty()) {
    idToExec = highQueue.front();
    isHighPrio = true;
    int idx = static_cast<int>(idToExec);
    engine_logic::trace("Executing hi - prior command " + std::to_string(idx) +
                        std::string(" with value ") +
                        std::to_string(commandRegistry[idx].payload * 1000));
  } else if (!queue.empty()) {
    idToExec = queue.front();
    isHighPrio = false;
  }

  // 2. Если есть команда - выполняем
  if (idToExec != CommandID::CMD_NONE) {

    // --- ЗАМЕР ВРЕМЕНИ (Если включен) ---
    Timer t;
    t.start();
    int idx = static_cast<int>(idToExec);
    bool success = ExecuteCommand(idToExec);
    unsigned duration = static_cast<unsigned>(t.check());
    if (success) {
      // Успех: удаляем из очереди
      commandRegistry[idx].isQueued = false;
      commandRegistry[idx].retryCount = 0;
      if (isHighPrio)
        highQueue.pop_front();
      else
        queue.pop_front();
    } else {
      // Ошибка: Логика режимов
      commandRegistry[idx].retryCount++;
      ReportError(idToExec, "Exec failed", duration);

      bool drop = false;
      std::string current_mode = "";
      if (currentMode == EngineMode::FAST) {
        drop = true; // Сразу выкидываем
        current_mode = "FAST";
      } else if (currentMode == EngineMode::STABLE) {
        // Критическим даем 3 попытки
        int maxRetries = commandRegistry[idx].isCritical ? 3 : 1;
        if (commandRegistry[idx].retryCount >= maxRetries) {
          drop = true;
        } else {
          // Выходим, так как больше ничего делать не надо, а так же не нужна
          // ротация, которя будет выполнена если не выйти тут
          return;
        }
        current_mode = "STABLE";
      } else if (currentMode == EngineMode::PRECISE) {
        drop = false; // он и так фолз, но это для наглядности
        current_mode = "PRECISE";
      }

      if (drop) {
        commandRegistry[idx].isQueued = false;
        if (isHighPrio) {
          highQueue.pop_front();
        } else {
          queue.pop_front();
        }
        ReportError(idToExec, "Dropped (Max Retries) in mode " + current_mode,
                    duration, t.checkS().start);
      } else {
        // Ротация: перемещаем в конец очереди, чтобы не стопорить поток
        if (isHighPrio) {
          highQueue.pop_front();
          highQueue.push_back(idToExec);
        } else {
          queue.pop_front();
          queue.push_back(idToExec);
        }

        pso_logic::warning(current_mode + "Mode: Command " +
                           std::to_string((int)idToExec) +
                           " failed, rotated to back.");
      }
    }
    return;
  }

  // 3. Если команд нет - Фоновая телеметрия
  if (Connected()) {
    Timer t_telemetry;
    t_telemetry.start();
    float val = -1.0f;
    CommandID currentPollId =
        askVoltage ? CommandID::CMD_GET_VOLTAGE : CommandID::CMD_GET_CURRENT;

    if (askVoltage) {
      val = device.GetVoltage();
      if (val >= 0) {
        cachedVoltage = val;
        lastVoltageUpdate = t_telemetry.checkS().now;
      }
    } else {
      val = device.GetCurrent();
      if (val >= 0) {
        cachedCurrent = val;
        lastCurrentUpdate = t_telemetry.checkS().now;
      }
    }

#ifdef ENABLE_PROFILING
    unsigned duration = static_cast<unsigned>(t_telemetry.check());
    if (val >= 0) {
      // Логируем успех с временем
      pso_logic::trace("Telemetry " + std::string(askVoltage ? "U" : "I") +
                       " updated in " + std::to_string(duration) + " us");
    } else {
      // Логируем ошибку с временем
      ReportError(askVoltage ? CommandID::CMD_GET_VOLTAGE
                             : CommandID::CMD_GET_CURRENT,
                  "Telemetry Timeout/Error", duration);
    }
#endif

    askVoltage = !askVoltage;
  }
}

bool PSEngine::ExecuteCommand(CommandID id) {
  // Если порт закрыт, разрешаем только CONNECT
  if (device.GetConnectionState() == utils::State::DISABLED &&
      id != CommandID::CMD_CONNECT) {
    engine_logic::trace("exit because port closed");
    return false;
  }

  float val = commandRegistry[static_cast<int>(id)].payload;
  Timer t;
  t.start();
  switch (id) {
  case CommandID::CMD_SET_VOLTAGE:
    if (val <= maxV && val >= minV) {
      return device.SetVoltage(val);
    } else {
      pso_logic::warning("Discarded attempt to set voltage out of [min, max] bounaties.");
    }
    return 0;
  case CommandID::CMD_SET_CURRENT:
    if (val <= maxC && val >= minC) {
      return device.SetCurrent(val);
    } else {
      pso_logic::warning("Discarded attempt to set current out of [min, max] bounaties.");
    }
    return 0;
  case CommandID::CMD_SET_OUTPUT:
    return device.SetOut(val > 0.5f);
  case CommandID::CMD_SET_LOCK:
    return device.SetLock(val > 0.5f);
  case CommandID::CMD_CONNECT:
    return device.Setup(targetPortName); // Используем сохраненное имя
  case CommandID::CMD_DISCONNECT:
	return device.Teardown();
  case CommandID::CMD_GET_VOLTAGE: {
    float tmp = device.GetVoltage();
    if (tmp >= 0) {
      lastVoltageUpdate = t.checkS().now;
    }
    return tmp;
  }
  case CommandID::CMD_GET_CURRENT: {
    float tmp = device.GetCurrent();
    if (tmp >= 0) {
      lastCurrentUpdate = t.checkS().now;
    }
    return tmp;
  }
  default:
    return false;
  }
}

void PSEngine::ReportError(CommandID id, const std::string &msg,
                           unsigned duration, uint64_t start_time) {
  // Если start_time не передан (0), берем текущее
  uint64_t ts = (start_time > 0)
                    ? start_time
                    : std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::steady_clock::now().time_since_epoch())
                          .count();

  errorLog.push(id, msg, currentTick, duration, ts);
}

// Полезные методы для UI, чтобы узнать насколько старые данные
uint64_t PSEngine::GetVoltageAgeUs() const {
  auto now = std::chrono::steady_clock::now();
  uint64_t ts = std::chrono::duration_cast<std::chrono::microseconds>(
                    now.time_since_epoch())
                    .count();
  return (lastVoltageUpdate == 0) ? 0 : (ts - lastVoltageUpdate);
}

uint64_t PSEngine::GetCurrentAgeUs() const {
  auto now = std::chrono::steady_clock::now();
  uint64_t ts = std::chrono::duration_cast<std::chrono::microseconds>(
                    now.time_since_epoch())
                    .count();
  return (lastCurrentUpdate == 0) ? 0 : (ts - lastCurrentUpdate);
}

bool PSEngine::LockConnectSync(const std::string &port) {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  // 1. Пытаемся открыть порт напрямую через device
  bool success = device.Setup(port);
  engine_logic::debug(std::string("Setup returned ") +
                      (success ? "true" : "false"));
  engine_logic::debug(
      std::string("Connection state is ") +
      (static_cast<bool>(device.GetConnectionState()) ? "true" : "false"));
  // 2. Если успешно — сбрасываем очереди, чтобы старые команды не полетели в
  // новый порт
  if (success) {
    targetPortName = port;
    queue.clear();
    highQueue.clear();
  } else {
    // Логируем ошибку сразу
    ReportError(CommandID::CMD_CONNECT, "Sync Connect Failed");
  }

  return success;
}

bool PSEngine::LockDisconnectSync() {
  std::lock_guard<std::recursive_mutex> lock(engineMutex);
  queue.clear();
  engine_logic::trace(std::string("High Queue size") +
                      std::to_string(highQueue.size()));
  while (!highQueue.empty()) {
    const auto idToExec = highQueue.front();
    highQueue.pop_front();
    engine_logic::trace(std::string("Executing command") +
                        std::to_string(static_cast<int>(idToExec)));
    try {
      ExecuteCommand(idToExec);
      int idx = static_cast<int>(idToExec);
      commandRegistry[idx].isQueued = false;
      commandRegistry[idx].retryCount = 0;
      Sleep(100);
      engine_logic::trace("Success");
    } catch (...) {

      // Не даем исключению прервать процесс дисконнекта
    }
  }
  bool success = device.Teardown();
  highQueue.clear();
  return success;
}

void PSEngine::DropState() const noexcept {
  engine_logic::debug_struct d;
  using st = std::string;
  Timer t;
  t.start();
  d(st("\n\n"));
  d(st("Connection state: ") + st(Connected() ? "" : "dis") + "connected");
  d(st("Lock state: ") +
    st(static_cast<bool>(device.GetLockState()) ? "locked" : "unlocked"));
  d(st("Out state: ") +
    st(static_cast<bool>(device.GetOutState()) ? "enabed" : "disabled"));
  d(st("Stab mode: ") + st(IsVoltageStab() ? "CV" : "CC"));
  d(st("Latest voltage: ") +
    std::to_string(static_cast<int>(GetLastVoltage() * 1000)) +
    st("mV received at ") + std::to_string(GetVoltageAgeUs()));
  d(st("Latest current: ") +
    std::to_string(static_cast<int>(GetLastCurrent() * 1000)) +
    st("mA received at ") + std::to_string(GetCurrentAgeUs()));
  d(st("Now is ") + std::to_string(t.checkS().now));
  d(st("Tick number") + std::to_string(currentTick));
  d(st("Max V is ") + std::to_string(static_cast<int>(maxV * 1000)) +
    st("mV, max C is ") + std::to_string(static_cast<int>(maxC * 1000)) + "mA");
  d(st("\n\n"));
}