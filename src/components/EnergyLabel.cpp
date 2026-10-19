#include "EnergyLabel.h"

void EnergyLabelUpdater::Update(float v, float c) {
	value += v * c * timer.stop() / 1'000'000.0f;
    timer.start();
	label->Caption = TForm1::formatValue_c(value / 3600, 5, 3); //not J but WH

};

void CapacityLabelUpdater::Update(float v, float c)  {
	value += c * timer.stop() / 1'000'000.0f;
    timer.start();
	label->Caption = TForm1::formatValue_c(value / 3600, 5, 3); //not J but WH
};