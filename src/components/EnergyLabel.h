#ifndef ENERGY_LABEL_H
#define ENERGY_LABEL_H
#include "UPSource.h"
#include "src/utils/time_manager.h"
struct EnergyLabel {
TLabel* label = nullptr;
float energy = 0; // in J
void EnergyLabelUpdate (float v, float c);
EnergyLabel(TLabel* l, Timer& t) : label(l), tick_time(t);
inline void CleanValue() {
    energy = 0;
}
~EnergyLabel() = default;

}

#endif