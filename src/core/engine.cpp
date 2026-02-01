#include "engine.h"
#include <chrono>

// === Реализация ErrorLogBuffer ===

void ErrorLogBuffer::push(CommandID cmd, const std::string &msg, size_t tick, unsigned duration, uint64_t timestamp)
{
    buffer[head] = {cmd, msg, tick, duration, timestamp};
    head = (head + 1) % buffer.size();
    if (head == 0)
        full = true;
}

std::vector<RuntimeError> ErrorLogBuffer::getAll() const
{
    std::vector<RuntimeError> result;
    result.reserve(buffer.size());
    if (full)
    {
        for (size_t i = head; i < buffer.size(); ++i)
            result.push_back(buffer[i]);
    }
    for (size_t i = 0; i < head; ++i)
        result.push_back(buffer[i]);
    return result;
}

// === Реализация PSEngine ===

PSEngine::PSEngine()
{
    // Инициализация слотов
    for (auto &slot : commandRegistry)
    {
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
void PSEngine::UpdateSlot(CommandID id, float val)
{
    int idx = static_cast<int>(id);

    // 1. Всегда обновляем значение (схлопывание)
    commandRegistry[idx].payload = val;
    commandRegistry[idx].retryCount = 0; // Сброс попыток для новой команды

    // 2. Если уже в очереди - выходим (значение мы обновили выше)
    if (commandRegistry[idx].isQueued)
        return;

    // 3. Добавляем в нужную очередь
    commandRegistry[idx].isQueued = true;
    if (commandRegistry[idx].isCritical)
    {
        highQueue.push_back(id);
    }
    else
    {
        queue.push_back(id);
    }
}

// === API Methods ===

void PSEngine::SetTargetVoltage(float v)
{
    UpdateSlot(CommandID::CMD_SET_VOLTAGE, v);
}

void PSEngine::SetTargetCurrent(float a)
{
    UpdateSlot(CommandID::CMD_SET_CURRENT, a);
}

void PSEngine::SetLock(bool enabled)
{
    UpdateSlot(CommandID::CMD_SET_LOCK, enabled ? 1.0f : 0.0f);
}

void PSEngine::SetOutput(bool enabled)
{
    UpdateSlot(CommandID::CMD_SET_OUTPUT, enabled ? 1.0f : 0.0f);
}

void PSEngine::Connect(const std::string &port)
{
    targetPortName = port;                    // Сохраняем имя порта
    UpdateSlot(CommandID::CMD_CONNECT, 0.0f); // Payload не важен
}

void PSEngine::Disconnect()
{
    UpdateSlot(CommandID::CMD_DISCONNECT, 0.0f);
}

// === Логика исполнения ===

void PSEngine::Tick()
{
    currentTick++;

    // 1. Выбираем команду для исполнения
    CommandID idToExec = CommandID::CMD_NONE;
    bool isHighPrio = false;

    if (!highQueue.empty())
    {
        idToExec = highQueue.front();
        isHighPrio = true;
    }
    else if (!queue.empty())
    {
        idToExec = queue.front();
        isHighPrio = false;
    }

    // 2. Если есть команда - выполняем
    if (idToExec != CommandID::CMD_NONE)
    {

        // --- ЗАМЕР ВРЕМЕНИ (Если включен) ---
        uint64_t start_ts = 0;
#ifdef ENABLE_PROFILING
        Timer t;
        auto start_clock = std::chrono::steady_clock::now();
        start_ts = std::chrono::duration_cast<std::chrono::microseconds>(start_clock.time_since_epoch()).count();
        t.start();
#endif

        bool success = ExecuteCommand(idToExec);

#ifdef ENABLE_PROFILING
        unsigned duration = static_cast<unsigned>(t.stop());
#else
        unsigned duration = 0;
#endif
        // ------------------------------------

        int idx = static_cast<int>(idToExec);

        if (success)
        {
            // Успех: удаляем из очереди
            commandRegistry[idx].isQueued = false;
            commandRegistry[idx].retryCount = 0;
            if (isHighPrio)
                highQueue.pop_front();
            else
                queue.pop_front();
        }
        else
        {
            // Ошибка: Логика режимов
            commandRegistry[idx].retryCount++;
            ReportError(idToExec, "Exec failed", duration);

            bool drop = false;

            if (currentMode == EngineMode::FAST)
            {
                drop = true; // Сразу выкидываем
            }
            else if (currentMode == EngineMode::STABLE)
            {
                // Критическим даем 3 попытки
                int maxRetries = commandRegistry[idx].isCritical ? 3 : 1;
                if (commandRegistry[idx].retryCount >= maxRetries)
                    drop = true;
            }
            else if (currentMode == EngineMode::PRECISE)
            {
                // Критические долбим до победного (drop = false)
                // Обычные дропаем после 1 попытки, чтобы не забивать очередь (или можно тоже держать)
                if (!commandRegistry[idx].isCritical)
                    drop = true;
            }

            if (drop)
            {
                commandRegistry[idx].isQueued = false;
                if (isHighPrio)
                    highQueue.pop_front();
                else
                    queue.pop_front();
                ReportError(idToExec, "Dropped (Max Retries)", duration);
            }
            else if (currentMode == EngineMode::PRECISE && isHighPrio)
            {
                // Убираем из головы и засовываем в хвост, чтобы в следующем тике
                // попробовать другую критическую команду
                highQueue.pop_front();
                highQueue.push_back(idToExec);
                pso_logic::debug("Precise Mode: Command " + std::to_string((int)idToExec) + " failed, rotated to back.");
            }
            // Если drop == false, команда остается в head очереди и повторится в следующий тик
        }
        return; // В этом тике больше ничего не делаем (одна команда записи за тик)
    }

    // 3. Если команд нет - Фоновая телеметрия (Round Robin)
    // ... в самом конце Tick() ...
    if (IsConnected())
    {
        // ПРАВКА 2: Замер времени телеметрии
        Timer t_telemetry;
#ifdef ENABLE_PROFILING
        t_telemetry.start();
#endif

        auto now = std::chrono::steady_clock::now();
        uint64_t nowUs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

        float val = -1.0f;
        CommandID currentPollId = askVoltage ? CommandID::CMD_GET_VOLTAGE : CommandID::CMD_GET_CURRENT;

        if (askVoltage)
        {
            val = device.GetVoltage();
            if (val >= 0)
            {
                cachedVoltage = val;
                lastVoltageUpdate = nowUs;
            }
        }
        else
        {
            val = device.GetCurrent();
            if (val >= 0)
            {
                cachedCurrent = val;
                lastCurrentUpdate = nowUs;
            }
        }

#ifdef ENABLE_PROFILING
        unsigned duration = static_cast<unsigned>(t_telemetry.stop());
        if (val >= 0)
        {
            // Логируем успех с временем
            pso_logic::trace("Telemetry " + std::string(askVoltage ? "U" : "I") +
                             " updated in " + std::to_string(duration) + " us");
        }
        else
        {
            // Логируем ошибку с временем
            ReportError(askVoltage ? CommandID::CMD_GET_VOLTAGE : CommandID::CMD_GET_CURRENT, "Telemetry Timeout/Error", duration);
        }
#endif

        askVoltage = !askVoltage;
    }
}

bool PSEngine::ExecuteCommand(CommandID id)
{
    // Если порт закрыт, разрешаем только CONNECT
    if (
        device.GetConnectionState() == utils::State::DISABLED &&
        id != CommandID::CMD_CONNECT)
    {
        return false;
    }

    float val = commandRegistry[static_cast<int>(id)].payload;

    switch (id)
    {
    case CommandID::CMD_SET_VOLTAGE:
        return device.SetVoltage(val);
    case CommandID::CMD_SET_CURRENT:
        return device.SetCurrent(val);
    case CommandID::CMD_SET_OUTPUT:
        return device.SetOut(val > 0.5f);
    case CommandID::CMD_SET_LOCK:
        return device.SetLock(val > 0.5f);
    case CommandID::CMD_CONNECT:
        return device.Setup(targetPortName); // Используем сохраненное имя
    case CommandID::CMD_DISCONNECT:
        return device.Disconnect();
    default:
        return false;
    }
}

void PSEngine::ReportError(CommandID id, const std::string &msg, unsigned duration, uint64_t start_time)
{
    // Если start_time не передан (0), берем текущее
    uint64_t ts = (start_time > 0) ? start_time : std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

    errorLog.push(id, msg, currentTick, duration, ts);
}

// Полезные методы для UI, чтобы узнать насколько старые данные
uint64_t PSEngine::GetVoltageAgeUs() const
{
    auto now = std::chrono::steady_clock::now();
    uint64_t ts = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    return (lastVoltageUpdate == 0) ? 0 : (ts - lastVoltageUpdate);
}

uint64_t PSEngine::GetCurrentAgeUs() const
{
    auto now = std::chrono::steady_clock::now();
    uint64_t ts = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    return (lastCurrentUpdate == 0) ? 0 : (ts - lastCurrentUpdate);
}