#ifndef CHART_MANAGER_H
#define CHART_MANAGER_H

#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <vector>
#include "src/utils/SessionCore.h"

class ChartManager {
private:
    TChart *targetChart = nullptr;
    bool voltageVisible = true;
    bool currentVisible = true;

public:
    ChartManager() = default;
    ~ChartManager() = default;

    // Инициализация двух осей (Левая Y = Напряжение, Правая Y = Ток)
    void Init(TChart *chart);

    // Полная перерисовка сессии (все точки сессии от 0 до N)
    void Update(const std::vector<SessionPoint> &data);

    // Управление видимостью серий
    void SetVoltageVisible(bool visible);
    void SetCurrentVisible(bool visible);
    bool IsVoltageVisible() const { return voltageVisible; }
    bool IsCurrentVisible() const { return currentVisible; }

    // Сброс зума и автоматический масштаб
    void ResetZoom();
};

#endif // CHART_MANAGER_H
