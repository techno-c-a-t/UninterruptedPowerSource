#ifndef PS_ENGINE_H
#define PS_ENGINE_H

#include <vector>
#include <string>
#include <chrono>
#include <array>
#include <deque>
#include "src/PS/PowerSourceObject.h"

// === НАСТРОЙКИ ===
enum class EngineMode {
    FAST,       // 1 попытка. Ошибка -> удаляем команду.
    STABLE,     // 3 попытки для критических, 1 для обычных.
    PRECISE     // Бесконечные попытки пока не выполнится.
};

enum class CommandID : int {
    CMD_NONE = -1,
    CMD_SET_VOLTAGE = 0,
    CMD_SET_CURRENT = 1,
    CMD_SET_OUTPUT = 2,
    CMD_SET_LOCK = 3,
    CMD_CONNECT = 4,
    CMD_DISCONNECT = 5,
    // Телеметрия не кладется в очередь, она фоновая
    CMD_COUNT = 6 
};

// === СТРУКТУРЫ ДАННЫХ ===

// "Умная" команда. Хранится в единственном экземпляре.
struct CommandSlot {
    float payload;      // Значение (V, A, или 1.0/0.0 для bool)
    bool isQueued;      // Флаг: уже стоит в очереди на отправку?
    bool isCritical;    // Критичная ли это команда (Connect, Output)
    int retryCount;     // Сколько раз уже пытались
};

// Структура ошибки для кольцевого буфера
struct RuntimeError {
    size_t tick;
    CommandID cmdId;
    std::string message;
    long long durationUs; // Время попытки в мкс
};

// Кольцевой буфер ошибок
class ErrorLogBuffer {
private:
    std::vector<RuntimeError> buffer;
    size_t head = 0;
    bool full = false;
public:
    explicit ErrorLogBuffer(size_t size = 50) : buffer(size) {}
    
    void push(size_t tick, CommandID cmd, const std::string& msg, long long duration) {
        buffer[head] = {tick, cmd, msg, duration};
        head = (head + 1) % buffer.size();
        full = full || (head == 0);
    }
    
    // Возвращает копию всех ошибок (от старых к новым)
    std::vector<RuntimeError> getAll() const;
};

// === КЛАСС ДВИЖКА ===

class PSEngine {
private:
    PowerSource device;
    EngineMode currentMode = EngineMode::STABLE;
    
    // Реестр команд (здесь хранятся актуальные значения)
    std::array<CommandSlot, (int)CommandID::CMD_COUNT> commandRegistry;
    
    // Очередь исполнения (хранит только ID команд)
    // std::deque позволяет эффективно удалять с обоих концов
    std::deque<CommandID> executionQueue;

    // История ошибок
    ErrorLogBuffer errorLog;
    size_t currentTick = 0;

    // Кэш для UI
    float cachedVoltage = 0.0f;
    float cachedCurrent = 0.0f;
    bool telemetryToggle = false; // Чередование V/I

    // Внутренние методы
    bool ExecuteCommand(CommandID id);
    void HandleError(CommandID id, const std::string& msg, long long duration);

public:
    PSEngine();

    // === API УПРАВЛЕНИЯ (Неблокирующее) ===
    
    // Если команда уже в очереди -> обновляет значение.
    // Если нет -> обновляет значение и добавляет в очередь.
    void SetTargetVoltage(float v);
    void SetTargetCurrent(float a);
    
    // bool кастуем в float (1.0f / 0.0f)
    void SetOutput(bool enabled);
    void SetLock(bool enabled);
    
    // Критические команды
    void Connect(const std::string& port);
    void Disconnect();

    // === ГЛАВНЫЙ ЦИКЛ ===
    void Tick();

    // === ГЕТТЕРЫ ===
    float GetLastVoltage() const { return cachedVoltage; }
    float GetLastCurrent() const { return cachedCurrent; }
    bool IsConnected() const { return static_cast<bool>(device.GetConnectionState()); }
    
    // Получить дамп ошибок для UI
    std::vector<RuntimeError> GetErrors() { return errorLog.getAll(); }
    
    void SetMode(EngineMode mode) { currentMode = mode; }
};

#endif