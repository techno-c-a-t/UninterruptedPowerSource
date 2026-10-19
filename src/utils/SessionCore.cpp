#include "SessionCore.h"
#include "SessionExport.h"

void SessionCore::SetDiscretInterval(float sec) {
    std::lock_guard<std::mutex> lock(sessionMutex);
    if (sec <= 0.0f) {
        discretInterval = DEFAULT_DISCRET_SEC;
    } else {
        discretInterval = sec;
    }
}

float SessionCore::GetDiscretInterval() const {
    std::lock_guard<std::mutex> lock(sessionMutex);
    return discretInterval;
}

bool SessionCore::IsRecording() const {
    std::lock_guard<std::mutex> lock(sessionMutex);
    return isRecording;
}

void SessionCore::StartSession() {
    std::lock_guard<std::mutex> lock(sessionMutex);
    sessionData.clear();
    sessionTimer.start();
    lastSampleTime = 0.0;
    nextTargetGridTimeSec = 0.0;
    sessionStartUs = 0;
    hasStartUs = false;
    isRecording = true;
}

void SessionCore::StopSession() {
    std::lock_guard<std::mutex> lock(sessionMutex);
    if (isRecording) {
        sessionTimer.stop();
        isRecording = false;
    }
}

bool SessionCore::Update(float currentVoltage, float currentCurrent, uint64_t timestampUs) {
    std::lock_guard<std::mutex> lock(sessionMutex);
    if (!isRecording) {
        return false;
    }

    double elapsedSec = 0.0;
    if (timestampUs > 0) {
        if (!hasStartUs) {
            sessionStartUs = timestampUs;
            hasStartUs = true;
        }
        elapsedSec = (timestampUs >= sessionStartUs) ? static_cast<double>(timestampUs - sessionStartUs) / 1'000'000.0 : 0.0;
    } else {
        elapsedSec = sessionTimer.check() / 1'000'000.0;
    }

    if (sessionData.empty()) {
        SessionPoint point;
        point.timestampSeconds = static_cast<float>(elapsedSec);
        point.voltage = currentVoltage;
        point.current = currentCurrent;
        point.power = currentVoltage * currentCurrent;

        sessionData.push_back(point);
        lastSampleTime = elapsedSec;
        nextTargetGridTimeSec = discretInterval;
        return true;
    }

    if (elapsedSec >= nextTargetGridTimeSec) {
        SessionPoint point;
        point.timestampSeconds = static_cast<float>(elapsedSec);
        point.voltage = currentVoltage;
        point.current = currentCurrent;
        point.power = currentVoltage * currentCurrent;

        sessionData.push_back(point);
        lastSampleTime = elapsedSec;

        while (nextTargetGridTimeSec <= elapsedSec) {
            nextTargetGridTimeSec += discretInterval;
        }
        return true;
    }

    return false;
}

bool SessionCore::SaveToFile(const std::string &filePath, float totalEnergyWh, float totalCapacityAh) const {
    std::vector<SessionPoint> dataCopy;

    {
        std::lock_guard<std::mutex> lock(sessionMutex);
        if (sessionData.empty()) {
            return false;
        }
        dataCopy = sessionData;
    }

    return SessionExport::SaveSessionToJson(filePath, dataCopy, totalEnergyWh, totalCapacityAh);
}

std::vector<SessionPoint> SessionCore::GetData() const {
    std::lock_guard<std::mutex> lock(sessionMutex);
    return sessionData;
}

size_t SessionCore::GetPointCount() const {
    std::lock_guard<std::mutex> lock(sessionMutex);
    return sessionData.size();
}

void SessionCore::Clear() {
    std::lock_guard<std::mutex> lock(sessionMutex);
    sessionData.clear();
    isRecording = false;
    lastSampleTime = 0.0;
}
