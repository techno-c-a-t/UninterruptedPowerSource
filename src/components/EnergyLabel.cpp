#include "EnergyLabel.h"
void EnergyLabelUpdate (float v, float c) {
    energy += v * c * timer.stop() / 1'000'000.0f;
    timer.start();
    label->Caption = TForm1::formatValue_c(energy, 8, 2);
}