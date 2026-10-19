#include "SessionExport.h"
#include <fstream>
#include <iomanip>
#include <ctime>

namespace SessionExport {

bool SaveSessionToJson(const std::string &filePath,
                       const std::vector<SessionPoint> &data,
                       float totalEnergyWh,
                       float totalCapacityAh) {
    if (data.empty()) {
        return false;
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        return false;
    }

    // Текущее локальное время в формате ISO 8601 (YYYY-MM-DDTHH:MM:SS)
    std::time_t now = std::time(nullptr);
    std::tm *tm_now = std::localtime(&now);
    char timeBuf[64] = {0};
    if (tm_now) {
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%dT%H:%M:%S", tm_now);
    } else {
        std::snprintf(timeBuf, sizeof(timeBuf), "unknown");
    }

    float durationSec = data.back().timestampSeconds;

    outFile << "{\n";
    outFile << "  \"format_version\": \"1.0\",\n";
    outFile << "  \"device\": {\n";
    outFile << "    \"type\": \"Laboratory Power Supply\",\n";
    outFile << "    \"description\": \"UPSource Session Export\"\n";
    outFile << "  },\n";
    outFile << "  \"session_info\": {\n";
    outFile << "    \"start_time\": \"" << timeBuf << "\",\n";
    outFile << "    \"total_points\": " << data.size() << ",\n";
    outFile << "    \"duration_seconds\": " << std::fixed << std::setprecision(2) << durationSec << "\n";
    outFile << "  },\n";
    outFile << "  \"summary\": {\n";
    outFile << "    \"total_energy_wh\": " << std::fixed << std::setprecision(3) << totalEnergyWh << ",\n";
    outFile << "    \"total_capacity_ah\": " << std::fixed << std::setprecision(3) << totalCapacityAh << "\n";
    outFile << "  },\n";
    outFile << "  \"columns\": [\n";
    outFile << "    \"time_s\",\n";
    outFile << "    \"voltage_v\",\n";
    outFile << "    \"current_a\",\n";
    outFile << "    \"power_w\"\n";
    outFile << "  ],\n";
    outFile << "  \"data\": [\n";

    outFile << std::fixed << std::setprecision(3);
    for (size_t i = 0; i < data.size(); ++i) {
        const auto &pt = data[i];
        outFile << "    ["
                << pt.timestampSeconds << ", "
                << pt.voltage << ", "
                << pt.current << ", "
                << pt.power << "]";
        if (i + 1 < data.size()) {
            outFile << ",";
        }
        outFile << "\n";
    }

    outFile << "  ]\n";
    outFile << "}\n";

    outFile.close();
    return true;
}

} // namespace SessionExport
