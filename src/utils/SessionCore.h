#ifndef SESSION_CORE_H
#define SESSION_CORE_H

#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <mutex>
#include "src/utils/time_manager.h"

constexpr float DEFAULT_DISCRET_SEC = 0.1f;

struct SessionPoint {
    float timestampSeconds; // Время от начала сеанса (в секундах)
    float voltage;          // Напряжение (В)
    float current;          // Ток (А)
    float power;            // Мощность (Вт)
};

class SessionCore {
protected:
    std::vector<SessionPoint> sessionData;
    Timer sessionTimer;
    double lastSampleTime = 0.0;
    double nextTargetGridTimeSec = 0.0; // Целевое идеальное время следующей отсечки
    uint64_t sessionStartUs = 0;
    bool hasStartUs = false;
    float discretInterval = DEFAULT_DISCRET_SEC;
    bool isRecording = false;
    mutable std::mutex sessionMutex;

public:
    SessionCore() = default;
    virtual ~SessionCore() = default;

    void StartSession();
    void StopSession();
    bool IsRecording() const;

    void SetDiscretInterval(float sec);
    float GetDiscretInterval() const;

    // Принимает данные телеметрии. Возвращает true, если создана новая отсечка лога
    bool Update(float currentVoltage, float currentCurrent, uint64_t timestampUs = 0);

    // Сохраняет сформированный сеанс в лог-файл (чистый C++ std::string)
    bool SaveToFile(const std::string &filePath, float totalEnergyWh = 0.0f, float totalCapacityAh = 0.0f) const;

    std::vector<SessionPoint> GetData() const;
    size_t GetPointCount() const;
    void Clear();
};

#endif // SESSION_CORE_H
