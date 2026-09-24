#include "ChartManager.h"
#include <cmath>

void ChartManager::Init(TChart *chart) {
    targetChart = chart;
    if (!targetChart) return;

    if (targetChart->SeriesCount() > 0) {
		targetChart->Series[0]->VertAxis = aLeftAxis;
    }
    if (targetChart->SeriesCount() > 1) {
        targetChart->Series[1]->VertAxis = aRightAxis;
    }

    targetChart->LeftAxis->Visible = true;
    targetChart->RightAxis->Visible = true;
    targetChart->BottomAxis->Visible = true;
}

void ChartManager::Update(const std::vector<SessionPoint> &data) {
    if (!targetChart) return;

    if (targetChart->SeriesCount() > 0) targetChart->Series[0]->Clear();
    if (targetChart->SeriesCount() > 1) targetChart->Series[1]->Clear();

    if (data.empty()) return;

    float minV = data[0].voltage;
    float maxV = data[0].voltage;
    float minC = data[0].current;
    float maxC = data[0].current;

    for (size_t i = 0; i < data.size(); ++i) {
        float v = data[i].voltage;
        float c = data[i].current;
        float t = data[i].timestampSeconds;

        if (v < minV) minV = v;
        if (v > maxV) maxV = v;
        if (c < minC) minC = c;
        if (c > maxC) maxC = c;

        if (targetChart->SeriesCount() > 0 && voltageVisible) {
            targetChart->Series[0]->AddXY(t, v);
        }
        if (targetChart->SeriesCount() > 1 && currentVisible) {
            targetChart->Series[1]->AddXY(t, c);
        }
    }

    // Динамический расчёт 10% отступов осей Y
    float rangeV = maxV - minV;
    float marginV = (rangeV > 0.001f) ? (rangeV * 0.10f) : ((std::abs(maxV) > 0.001f) ? (std::abs(maxV) * 0.10f) : 1.0f);
    targetChart->LeftAxis->Automatic = false;
    targetChart->LeftAxis->SetMinMax(minV - marginV, maxV + marginV);

    if (targetChart->SeriesCount() > 1) {
        float rangeC = maxC - minC;
        float marginC = (rangeC > 0.0001f) ? (rangeC * 0.10f) : ((std::abs(maxC) > 0.0001f) ? (std::abs(maxC) * 0.10f) : 0.1f);
        targetChart->RightAxis->Automatic = false;
        targetChart->RightAxis->SetMinMax(minC - marginC, maxC + marginC);
    }
}

void ChartManager::SetVoltageVisible(bool visible) {
    voltageVisible = visible;
    if (targetChart && targetChart->SeriesCount() > 0) {
        targetChart->Series[0]->Active = visible;
    }
}

void ChartManager::SetCurrentVisible(bool visible) {
    currentVisible = visible;
    if (targetChart && targetChart->SeriesCount() > 1) {
        targetChart->Series[1]->Active = visible;
    }
}

void ChartManager::ResetZoom() {
    if (!targetChart) return;
    targetChart->UndoZoom();
    targetChart->LeftAxis->Automatic = true;
    targetChart->RightAxis->Automatic = true;
    targetChart->BottomAxis->Automatic = true;
}
