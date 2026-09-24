#include "SessionManager.h"

bool SessionManager::SaveToFile(const String &filePath, float totalEnergyWh, float totalCapacityAh) const {
    std::string pathStr = AnsiString(filePath).c_str();
    return SessionCore::SaveToFile(pathStr, totalEnergyWh, totalCapacityAh);
}
