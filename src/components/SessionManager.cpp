#pragma hdrstop
#include "SessionManager.h"
#pragma package(smart_init)

bool SessionManager::SaveToFile(const String &filePath, float totalEnergyWh, float totalCapacityAh) const {
    std::string pathStr = AnsiString(filePath).c_str();
    return SessionCore::SaveToFile(pathStr, totalEnergyWh, totalCapacityAh);
}
