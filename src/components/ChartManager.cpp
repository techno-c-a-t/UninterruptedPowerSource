#pragma hdrstop
#include "ChartManager.h"
#include "src/config.h"
#include <cmath>
#pragma package(smart_init)

void ChartManager::Init(TChart *chart) {
    targetChart = chart;
    if (!targetChart) return;

    if (targetChart->SeriesCount() > 0) {
        targetChart->Series[0]->VerticalAxis = aLeftAxis;
    }
    if (targetChart->SeriesCount() > 1) {
        targetChart->Series[1]->VerticalAxis = aRightAxis;
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

    // Динамический расчёт отступов осей Y
    float rangeV = maxV - minV;
    float marginV = (rangeV > config::charts::FLAT_GRAPH_EPSILON) 
        ? (rangeV * config::charts::AXIS_MARGIN_PERCENT) 
        : ((std::abs(maxV) > config::charts::FLAT_GRAPH_EPSILON) ? (std::abs(maxV) * config::charts::AXIS_MARGIN_PERCENT) : 1.0f);
    targetChart->LeftAxis->Automatic = false;
    targetChart->LeftAxis->SetMinMax(minV - marginV, maxV + marginV);

    if (targetChart->SeriesCount() > 1) {
        float rangeC = maxC - minC;
        float marginC = (rangeC > (config::charts::FLAT_GRAPH_EPSILON / 10.0f)) 
            ? (rangeC * config::charts::AXIS_MARGIN_PERCENT) 
            : ((std::abs(maxC) > (config::charts::FLAT_GRAPH_EPSILON / 10.0f)) ? (std::abs(maxC) * config::charts::AXIS_MARGIN_PERCENT) : 0.1f);
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
