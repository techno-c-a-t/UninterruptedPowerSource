#ifndef ENGINE_H
#define ENGINE_H

#include "engine_log.h"
#include "src/PS/PowerSourceObject.h"
#include "src/utils/common_namespace.h"
#include "src/utils/time_manager.h"
#include <array>
#include <chrono>
#include <deque>
#include <string>
#include <vector>


// Включает замер времени выполнения каждой команды
#define ENABLE_PROFILING 1

// === КЛАСС ДВИЖКА ===

class PSEngine {
private:
  enum class EngineMode {
    FAST,   // 1 попытка. Ошибка -> удаляем команду.
    STABLE, // 3 попытки для критических, 1 для обычных.
    PRECISE // Бесконечные попытки для критических. Обычные ждут. Потом
            // бесконечные для обычных
  };

  enum class CommandID : uint8_t {
    CMD_NONE = 0,
    CMD_SET_VOLTAGE = 1,
    CMD_SET_CURRENT = 2,
    CMD_SET_OUTPUT = 3,
    CMD_SET_LOCK = 4,
    CMD_CONNECT = 5,
    CMD_DISCONNECT = 6,
    CMD_GET_VOLTAGE = 7,
    CMD_GET_CURRENT = 8,

    CMD_COUNT = 9
  };

  // === СТРУКТУРЫ ДАННЫХ ===

  // Стуктура для комманды
  struct CommandSlot {
    float payload;   // Значение (V, A, 1.0/0.0)
    bool isCritical; // true -> идет в highQueue
    bool isQueued;   // Есть ли комманду уже в очереди (не добавляем второй раз)
    int retryCount;  // Счетчик попыток для STABLE режима
  };

  // структура для ощибки рантайма
  struct RuntimeError {
    CommandID cmdId;
    std::string message;
    size_t tick;
    unsigned durationUs; // 0, если профилирование выключено
    uint64_t timestamp;  // Абсолютное время (start_time)
    void log_error() const {
      engine_logic::error(
          "Command " + std::to_string(static_cast<int>(cmdId)) + " at tick " +
          std::to_string(tick) + " failed with message " + message +
          ". Execution time: " + std::to_string(durationUs) + "microseconds.");
    }
  };

  // Класс кольцевого буфера ошибок
  class ErrorLogBuffer {
  private:
    std::vector<RuntimeError> buffer;
    size_t head = 0;
    bool full = false;

  public:
    explicit ErrorLogBuffer(size_t size = 50) : buffer(size) {}

    void push(CommandID cmd, const std::string &msg, size_t tick,
              unsigned duration, uint64_t timestamp);
    void log() const {
      engine_logic::error("\n\n");
      if (full) {
        for (size_t i = head; i < buffer.size(); i++) {
          buffer[i].log_error();
        }
      }
      for (size_t i = 0; i < head; i++) {
        buffer[i].log_error();
      }
      engine_logic::error("\n\n");
    }
    void flush_log() {
      log();
      size_t tmp_size = buffer.size();
      buffer.clear();
      head = 0;
      full = false;
      buffer.resize(tmp_size);
    }
    std::vector<RuntimeError> getAll() const;
  };

private:
  PowerSource device;
  EngineMode currentMode = EngineMode::STABLE;

  float maxC, maxV;
  float minC, minV;

  // Реестр команд
  std::array<CommandSlot, static_cast<int>(CommandID::CMD_COUNT)>
      commandRegistry;

  // Очереди исполнения (хранят только ID)
  std::deque<CommandID> queue;     // Обычные (напряжение, ток)
  std::deque<CommandID> highQueue; // Критические (Connect, Output, Lock)

  // Логирование
  ErrorLogBuffer errorLog;
  size_t currentTick = 0;

  // Кэш и Телеметрия
  float cachedVoltage = 0.0f;
  float cachedCurrent = 0.0f;

  // Время последнего успешного обновления кэша (микросекунды с эпохи)
  uint64_t lastVoltageUpdate = 0;
  uint64_t lastCurrentUpdate = 0;

  bool askVoltage = true; // Чередование опроса
  std::string targetPortName; // Храним имя порта для Connect чтобы выполнять
                              // отключение подключение

  // Внутренние методы
  bool ExecuteCommand(CommandID id);
  void ReportError(CommandID id, const std::string &msg, unsigned duration = 0,
                   uint64_t start_time = 0);

  // Шаблонный метод для добавления в очередь (реализован в cpp или header)
  void UpdateSlot(CommandID id, float val);






// Функции ядра  
public:
  PSEngine(float minVoltage, float minCurrent, float maxVoltage, float maxCurrent);

  // Сохраняем обратную совместимость с конструктором от двух параметров с дефлотным нулем
  PSEngine(float maxVoltage, float maxCurrent) : PSEngine(0.0f, 0.0f, maxVoltage, maxCurrent) {}; 


  // === API УПРАВЛЕНИЯ ===

  void SetTargetVoltage(float v, bool sync = false);
  void SetTargetCurrent(float a, bool sync = false);
  void SetLock(bool enabled, bool sync = false);
  void SetOutput(bool enabled, bool sync = false);

  void Connect(const std::string &port);
  void Disconnect();
  bool LockConnectSync(
      const std::string &port); // Тоже синхронно, чтобы мгновенно обновить UI
  bool LockDisconnectSync();    // Тоже синхронно, чтобы мгновенно обновить UI

  // === ГЛАВНЫЙ ЦИКЛ ===
  void Tick();

  // === ГЕТТЕРЫ ===
  float GetLastVoltage() const { return cachedVoltage; }
  float GetLastCurrent() const { return cachedCurrent; }

  // Время последнего обновления (можно использовать для подсветки "тухлых"
  // данных в UI)
  uint64_t GetVoltageAgeUs() const;
  uint64_t GetCurrentAgeUs() const;
  bool GetOutState() const { return static_cast<bool>(device.GetOutState()); }

  bool Connected() const {
    return device.GetConnectionState() == utils::State::ENABLED;
  }
  bool IsVoltageStab() const {
    return device.GetMode() == utils::Mode::VOLTAGE;
  }

  std::vector<RuntimeError> GetErrors() const { return errorLog.getAll(); }
  void SetMode(EngineMode mode) { currentMode = mode; }
  void flush_errors() { errorLog.flush_log(); }


  void DropState() const noexcept;


  
  void SetMaxCurrent(float maxCurrent) noexcept {
    maxC =  maxCurrent;
  }

  void SetMaxVoltage(float maxVoltage) noexcept {
    maxV =  maxVoltage;
  }

  void SetMinCurrent(float minCurrent) noexcept {
    minC =  minCurrent;
  }

  void SetMinVoltage(float minVoltage) noexcept {
    minV =  minVoltage;
  }

};

#endif