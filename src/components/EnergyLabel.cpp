#include "EnergyLabel.h"
void EnergyLabelUpdater::Update (float v, float c) {
	energy += v * c * timer.stop() / 1'000'000.0f;
    timer.start();
	label->Caption = TForm1::formatValue_c(energy / 3600, 2, 2); //not J but WH
}