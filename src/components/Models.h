#ifndef MODELS_SELECT_H
#define MODELS_SELECT_H
#include <vcl.h>
#include <System.IniFiles.hpp>
#include <vector>
#include "SaveOpen.h"
#include "UPSource.h"
#include "src/core/engine.h"
namespace storage {
    struct ModelParams : public IMemData {
	float minV = 0, minC = 0, maxV = 0, maxC = 0, curveCurrentPow = 1, curveVoltagePow = 1;

    void Pack(TCustomIniFile *ini, String section) const override;
    void Depack(TCustomIniFile *ini, String section) override;
};


void LoadModels (TComboBox* list, String file);
void SetModel (PSEngine& engine, String model, String file);
}

#endif
