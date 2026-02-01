#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <string>
#include <chrono>
#include <array>
#include <deque>
#include "src/PS/PowerSourceObject.h" 
#include "src/utils/timer.h" 
#include "src/utils/common_namespace.h"         

// Включает замер времени выполнения каждой команды
#define ENABLE_PROFILING 1

// === НАСТРОЙКИ ===
enum class EngineMode
{
    FAST,   // 1 попытка. Ошибка -> удаляем команду.
    STABLE, // 3 попытки для критических, 1 для обычных.
    PRECISE // Бесконечные попытки для критических. Обычные ждут. Потом бесконечные для обычных
};

enum class CommandID : uint8_t
{
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

struct CommandSlot
{
    float payload;   // Значение (V, A, 1.0/0.0)
    bool isCritical; // true -> идет в highQueue
    bool isQueued;   // Есть ли комманду уже в очереди (не добавляем второй раз)
    int retryCount;  // Счетчик попыток для STABLE режима
};

struct RuntimeError
{
    CommandID cmdId;
    std::string message;
    size_t tick;
    unsigned durationUs; // 0, если профилирование выключено
    uint64_t timestamp;  // Абсолютное время (start_time)
};

class ErrorLogBuffer
{
private:
    std::vector<RuntimeError> buffer;
    size_t head = 0;
    bool full = false;

public:
    explicit ErrorLogBuffer(size_t size = 50) : buffer(size) {}

    void push(CommandID cmd, const std::string &msg, size_t tick, unsigned duration, uint64_t timestamp);
    std::vector<RuntimeError> getAll() const;
};

// === КЛАСС ДВИЖКА ===

class PSEngine
{
private:
    PowerSource device;
    EngineMode currentMode = EngineMode::STABLE;

    // Реестр команд
    std::array<CommandSlot, static_cast<int>(CommandID::CMD_COUNT)> commandRegistry;

    // Очереди исполнения (хранят только ID)
    std::deque<CommandID> queue;      // Обычные (напряжение, ток)
    std::deque<CommandID> highQueue;  // Критические (Connect, Output, Lock)

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
    std::string targetPortName; // Храним имя порта для Connect ?? Он же уже есть в ComPort

    // Внутренние методы
    bool ExecuteCommand(CommandID id);
    void PSEngine::ReportError(CommandID id, const std::string &msg, unsigned duration = 0, uint64_t start_time = 0);

    // Шаблонный метод для добавления в очередь (реализован в cpp или header)
    void UpdateSlot(CommandID id, float val);

public:
    PSEngine();

    // === API УПРАВЛЕНИЯ ===
    void SetTargetVoltage(float v);
    void SetTargetCurrent(float a);
    void SetLock(bool enabled);
    void SetOutput(bool enabled);
    
    void Connect(const std::string &port);
    void Disconnect();

    // === ГЛАВНЫЙ ЦИКЛ ===
    void Tick();

    // === ГЕТТЕРЫ ===
    float GetLastVoltage() const { return cachedVoltage; }
    float GetLastCurrent() const { return cachedCurrent; }
    
    // Время последнего обновления (можно использовать для подсветки "тухлых" данных в UI)
    uint64_t GetVoltageAgeUs() const;
    uint64_t GetCurrentAgeUs() const;

    bool IsConnected() const { return device.GetConnectionState() == utils::State::ENABLED; }

    std::vector<RuntimeError> GetErrors() { return errorLog.getAll(); }
    void SetMode(EngineMode mode) { currentMode = mode; }
};

#endif