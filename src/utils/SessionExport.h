#ifndef SESSION_EXPORT_H
#define SESSION_EXPORT_H

#include <string>
#include <vector>
#include "SessionCore.h"

namespace SessionExport {

/**
 * @brief Сохраняет сеанс в файл формата JSON с метаданными и компактным массивом колонок.
 * 
 * @param filePath Путь к целевому файлу.
 * @param data Снимок точек сеанса (SessionPoint).
 * @param totalEnergyWh Накопленная энергия (Вт*ч).
 * @param totalCapacityAh Накопленная емкость (А*ч).
 * @param discretInterval Интервал дискретизации (сек).
 * @return true при успешной записи, false при ошибке открытия/пустых данных.
 */
bool SaveSessionToJson(const std::string &filePath,
                       const std::vector<SessionPoint> &data,
                       float totalEnergyWh = 0.0f,
                       float totalCapacityAh = 0.0f);

} // namespace SessionExport

#endif // SESSION_EXPORT_H
