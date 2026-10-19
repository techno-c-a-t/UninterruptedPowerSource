#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "src/utils/SessionCore.h"
#include <System.SysUtils.hpp>

class SessionManager : public SessionCore {
public:
    SessionManager() = default;

    // Перегрузка метода сохранения для работы с VCL-типом String (TSaveDialog)
    bool SaveToFile(const String &filePath, float totalEnergyWh = 0.0f, float totalCapacityAh = 0.0f) const;
};

#endif // SESSION_MANAGER_H
