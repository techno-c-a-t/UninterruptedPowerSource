#ifndef ENERGY_LABEL_H
#define ENERGY_LABEL_H
#include "UPSource.h"
#include "src/utils/time_manager.h"

class LabelUpdater {
    public:
TLabel* label = nullptr;
float value = 0; // in J
virtual void Update (float v, float c) = 0;
// EnergyLabelUpdater(TLabel* l): label(l) {};
inline void CleanValue() {
    value = 0;
}
virtual ~LabelUpdater() = default;
    protected:
    Timer timer;
};

class EnergyLabelUpdater: public LabelUpdater {
    public:
void Update(float v, float c) override;
virtual ~EnergyLabelUpdater() = default;

};

class CapacityLabelUpdater: public LabelUpdater {
    public:
void Update(float v, float c) override;
virtual ~CapacityLabelUpdater() = default;
};

#endif