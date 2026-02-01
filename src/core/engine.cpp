#include "src/core/engine.h"
#include <iostream>

PSEngine::PSEngine() {
    // Инициализация реестра
    for (auto& slot : commandRegistry) {
        slot.payload = 0.0f;
        slot.isQueued = false;
        slot.retryCount = 0;
        slot.isCritical = false;
    }
    // Отмечаем критические команды
    commandRegistry[(int)CommandID::CMD_SET_OUTPUT].isCritical = true;
    commandRegistry[(int)CommandID::CMD_CONNECT].isCritical = true;
    commandRegistry[(int)CommandID::CMD_DISCONNECT].isCritical = true;
}

// === УНИВЕРСАЛЬНЫЙ SETTER (Принцип схлопывания) ===
// Этот шаблон можно использовать для всех методов
template <CommandID ID>
void UpdateSlot(std::array<CommandSlot, (int)CommandID::CMD_COUNT>& registry, 
                std::deque<CommandID>& queue, 
                float value) {
    int idx = (int)ID;
    // 1. Обновляем значение (всегда последнее актуально)
    registry[idx].payload = value;
    registry[idx].retryCount = 0; // Сбрасываем попытки, т.к. это новая хотелка

    // 2. Если команда УЖЕ в очереди -> ничего не делаем (она отправится с новым значением)
    // 3. Если НЕТ -> добавляем в хвост
    if (!registry[idx].isQueued) {
        queue.push_back(ID);
        registry[idx].isQueued = true;
    }
}

void PSEngine::SetTargetVoltage(float v) {
    UpdateSlot<CommandID::CMD_SET_VOLTAGE>(commandRegistry, executionQueue, v);
}

void PSEngine::SetTargetCurrent(float a) {
    UpdateSlot<CommandID::CMD_SET_CURRENT>(commandRegistry, executionQueue, a);
}

void PSEngine::SetOutput(bool enabled) {
    UpdateSlot<CommandID::CMD_SET_OUTPUT>(commandRegistry, executionQueue, enabled ? 1.0f : 0.0f);
}

void PSEngine::SetLock(bool enabled) {
    UpdateSlot<CommandID::CMD_SET_LOCK>(commandRegistry, executionQueue, enabled ? 1.0f : 0.0f);
}

// Connect - особый случай, порт передаем через payload (но у нас float...)
// Хак: сохраняем имя порта в device.Setup временно или используем отдельное поле.
// Для простоты и надежности Connect сделаем "сквозным" или используем float как флаг.
// Но пользователь просил надежность.
void PSEngine::Connect(const std::string& port) {
    // Тут лучше вызвать синхронно, т.к. без порта ничего не работает
    // Либо если очень хочется в очередь - нужно хранить строку порта отдельно
    if (device.Connect(port)) {
        // Успех
    } else {
        errorLog.push(currentTick, CommandID::CMD_CONNECT, "Connection failed", 0);
    }
}

void PSEngine::Disconnect() {
    UpdateSlot<CommandID::CMD_DISCONNECT>(commandRegistry, executionQueue, 0.0f);
}

// === ИСПОЛНЕНИЕ ===

void PSEngine::Tick() {
    currentTick++;
    
    // 1. Очередь команд
    if (!executionQueue.empty()) {
        CommandID id = executionQueue.front();
        int idx = (int)id;
        
        // Замер времени
        auto start = std::chrono::high_resolution_clock::now();
        
        bool success = ExecuteCommand(id);
        
        auto end = std::chrono::high_resolution_clock::now();
        long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        if (success) {
            // Успех: убираем из очереди
            executionQueue.pop_front();
            commandRegistry[idx].isQueued = false;
            commandRegistry[idx].retryCount = 0;
        } else {
            // Неудача: логика режимов
            commandRegistry[idx].retryCount++;
            HandleError(id, "Command Execution Failed", duration);
            
            bool drop = false;
            
            if (currentMode == EngineMode::FAST) {
                drop = true; // Фаст режим - выкидываем сразу
            } 
            else if (currentMode == EngineMode::STABLE) {
                // Если критичная - даем 3 попытки, иначе выкидываем
                if (commandRegistry[idx].isCritical) {
                     if (commandRegistry[idx].retryCount >= 3) drop = true;
                } else {
                     drop = true; // Некритичные (вольтаж) в Stable тоже можно дропать, 
                                  // т.к. пользователь скорее всего еще раз дернет ползунок
                }
            }
            else if (currentMode == EngineMode::PRECISE) {
                drop = false; // Бесконечные попытки
            }

            if (drop) {
                executionQueue.pop_front();
                commandRegistry[idx].isQueued = false;
                errorLog.push(currentTick, id, "Command Dropped (Max Retries)", duration);
            }
        }
        
        // В один тик выполняем одну команду записи
        return;
    }

    // 2. Если команд нет - Телеметрия (фоном)
    // Тут очередей нет, просто Round Robin
    if (static_cast<bool>(device.GetConnectionState())) {
        auto start = std::chrono::high_resolution_clock::now();
        
        if (telemetryToggle) {
            float v = device.GetVoltage();
            if (v >= 0) cachedVoltage = v;
        } else {
            float i = device.GetCurrent();
            if (i >= 0) cachedCurrent = i;
        }
        telemetryToggle = !telemetryToggle;
        
        // Можно тоже замерять время телеметрии, если интересно
    }
}

bool PSEngine::ExecuteCommand(CommandID id) {
    if (!static_cast<bool>(device.GetConnectionState()) && id != CommandID::CMD_CONNECT) return false;

    float val = commandRegistry[(int)id].payload;
    
    switch (id) {
        case CommandID::CMD_SET_VOLTAGE: return device.SetVoltage(val);
        case CommandID::CMD_SET_CURRENT: return device.SetCurrent(val);
        case CommandID::CMD_SET_OUTPUT:  return device.SetOut(val > 0.5f);
        case CommandID::CMD_SET_LOCK:    return device.SetLock(val > 0.5f);
        case CommandID::CMD_DISCONNECT:  return device.Disconnect();
        default: return false;
    }
}

void PSEngine::HandleError(CommandID id, const std::string& msg, long long duration) {
    errorLog.push(currentTick, id, msg, duration);
    // Сюда можно добавить вывод в pso_logic::error
}

// Реализация буфера ошибок
std::vector<RuntimeError> ErrorLogBuffer::getAll() const {
    std::vector<RuntimeError> result;
    if (full) {
        for (size_t i = head; i < buffer.size(); ++i) result.push_back(buffer[i]);
        for (size_t i = 0; i < head; ++i) result.push_back(buffer[i]);
    } else {
        for (size_t i = 0; i < head; ++i) result.push_back(buffer[i]);
    }
    return result;
}