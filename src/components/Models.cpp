#include "Models.h"
#include <vcl.h>
#include <IniFiles.hpp>
#include <memory>
#include <math.h>

namespace storage {
// Реализация упаковки/распаковки параметров модели
void ModelParams::Pack(TCustomIniFile *ini, String section) const {
	ini->WriteFloat(section, "MinVoltage", minV);
	ini->WriteFloat(section, "MinCurrent", minC);
    ini->WriteFloat(section, "MaxVoltage", maxV);
	ini->WriteFloat(section, "MaxCurrent", maxC);
	ini->WriteFloat(section, "CurveCurrentPow", curveCurrentPow);
	ini->WriteFloat(section, "CurveVoltagePow", curveVoltagePow);
}

void ModelParams::Depack(TCustomIniFile *ini, String section) {
    minV = ini->ReadFloat(section, "MinVoltage", 0.0f);
    minC = ini->ReadFloat(section, "MinCurrent", 0.0f);
    maxV = ini->ReadFloat(section, "MaxVoltage", 5.0f); // дефолт 5В
	maxC = ini->ReadFloat(section, "MaxCurrent", 0.1f);  // дефолт 0.1А
	curveCurrentPow = ini->ReadFloat(section, "CurveCurrentPow", 1.0f);
	curveVoltagePow = ini->ReadFloat(section, "CurveVoltagePow", 1.0f);
}

// Заполнение ComboBox именами секций
// void LoadModels (TComboBox* list, String file) {
//    if (!FileExists(file)) {
// 		ShowMessage("File Models.ini doesnt exist");
//        return;
//    }

// //	list->Items->BeginUpdate();
//    try {
//        std::unique_ptr<TIniFile> ini(new TIniFile(file));
// 		list->Items->Clear();
// 		ini->ReadSections(list->Items);
// 		ShowMessage("File:" + file);
// 	}
// 	catch (...) {
// //		list->Items->EndUpdate();
// 		throw;
// 	}
// //	list->Items->EndUpdate();
// }


void LoadModels(TComboBox* list, String file) {
    if (!FileExists(file)) {
        ShowMessage("Файл не найден: " + file);
        return;
    }

    list->Items->BeginUpdate();
    try {
        // TMemIniFile работает с указанным путём без авто-дополнений Windows
        std::unique_ptr<TMemIniFile> ini(new TMemIniFile(file));

        list->Items->Clear();
        ini->ReadSections(list->Items);

        if (list->Items->Count == 0) {
            ShowMessage("Секции не найдены в файле: " + file);
        }
    }
    catch (const Exception& e) {
        ShowMessage("Ошибка: " + e.Message);
    }
    list->Items->EndUpdate();
}



// Загрузка параметров конкретной модели
void SetModel (PSEngine& engine, String model, String file) {
    ModelParams data;
    std::unique_ptr<TMemIniFile> ini(new TMemIniFile(file));
    data.Depack(ini.get(), model);
    engine.SetMinVoltage(data.minV);
	engine.SetMinCurrent(data.minC);
    engine.SetMaxCurrent(data.maxC);
	engine.SetMaxVoltage(data.maxV);
	Form1->ScrollBar1->Min = static_cast<int>(round(data.minC*1000));
	Form1->ScrollBar1->Max = static_cast<int>(round(data.maxC*1000));
	Form1->ScrollBar2->Min = static_cast<int>(round(data.minV*1000));
	Form1->ScrollBar2->Max = static_cast<int>(round(data.maxV*1000));
	Form1->CCurve = data.curveCurrentPow;
    Form1->VCurve = data.curveVoltagePow;
    Form1->ScrollBar1->Position = Form1->ScrollBar1->Min;
    Form1->ScrollBar2->Position = Form1->ScrollBar2->Min;
    // ShowMessage("Updated");
}

} // namespace storage