#ifndef ENERGY_LABEL_H
#define ENERGY_LABEL_H
#include "UPSource.h"
#include "src/utils/time_manager.h"
struct EnergyLabelUpdater {
TLabel* label = nullptr;
Timer timer;
float energy = 0; // in J
void Update (float v, float c);
EnergyLabelUpdater() = default;
inline void CleanValue() {
    energy = 0;
}
~EnergyLabelUpdater() = default;

};

#endif