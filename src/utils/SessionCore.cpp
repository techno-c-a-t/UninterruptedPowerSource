#include "SessionCore.h"
#include <fstream>
#include <sstream>
#include <iomanip>

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
    std::lock_guard<std::mutex> lock(sessionMutex);
    if (sessionData.empty()) {
        return false;
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        return false;
    }

    // Локальное время записи
    std::time_t now = std::time(nullptr);
    std::tm *tm_now = std::localtime(&now);

    char timeBuf[64] = {0};
    if (tm_now) {
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", tm_now);
    }

    outFile << "==================================================\n";
    outFile << "Session Log - Laboratory Power Supply\n";
    outFile << "Date/Time: " << timeBuf << "\n";
    outFile << "Total Points: " << sessionData.size() << "\n";
    if (!sessionData.empty()) {
        outFile << "Total Duration: " << std::fixed << std::setprecision(2) 
                << sessionData.back().timestampSeconds << " s\n";
    }
    outFile << "Total Energy: " << std::fixed << std::setprecision(3) << totalEnergyWh << " Wh\n";
    outFile << "Total Capacity: " << std::fixed << std::setprecision(3) << totalCapacityAh << " Ah\n";
    outFile << "Discretization Interval: " << std::fixed << std::setprecision(2) << discretInterval << " s\n";
    outFile << "==================================================\n";
    outFile << "Time(s)\tVoltage(V)\tCurrent(A)\tPower(W)\n";

    outFile << std::fixed << std::setprecision(3);
    for (const auto &pt : sessionData) {
        outFile << pt.timestampSeconds << "\t"
                << pt.voltage << "\t"
                << pt.current << "\t"
                << pt.power << "\n";
    }

    outFile.close();
    return true;
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
