#ifndef SAVE_OPEN_H
#define SAVE_OPEN_H
#include <System.IniFiles.hpp>
#include <vector>

namespace storage {
// Базовая структура пресета (POD-like)
class IMemData {
public:

    virtual ~IMemData() = default;
    virtual void Pack(TCustomIniFile* ini, String section) const  = 0;
    // {
    //     ini->WriteFloat(section, "V", Voltage);
    //     ini->WriteFloat(section, "C", Current);
    //     ini->WriteBool(section, "Active", Enabled);
    // }

    virtual void Depack(TCustomIniFile* ini, String section) = 0;
    // {
    //     // Второе значение — это default (если в файле пусто)
    //     Voltage = ini->ReadFloat(section, "V", 0.0);
    //     Current = ini->ReadFloat(section, "C", 0.0);
    //     Enabled = ini->ReadBool(section, "Active", false);
    // }
};

struct CVMemPair : public IMemData {
    float voltage = 0.0f;
    float current = 0.0f;

    void Pack(TCustomIniFile* ini, String section) const override {
        ini->WriteFloat(section, "Voltage", voltage);
        ini->WriteFloat(section, "Current", current);
    }

    virtual void Depack(TCustomIniFile* ini, String section) override {
        voltage = ini->ReadFloat(section, "Voltage", 0.0f);
        current = ini->ReadFloat(section, "Current", 0.0f);
    }

};

// Сохранить все пресеты
template <class T>
void SavePresetsToDisk(String fileName, std::vector<T>& data, String s = "CVPreset_") {
    // TMemIniFile работает быстрее обычного, так как копирует всё в ОЗУ
    TMemIniFile* ini = new TMemIniFile(fileName); 
    try {
        for (size_t i = 0; i < data.size(); i++) {
            // Формируем имя секции, например [Preset_0], [Preset_1]...
            String section = s + IntToStr(static_cast<int>(i));
            data[i].Pack(ini, section);
        }
        ini->UpdateFile(); // Физическая запись на диск
    }
    __finally {
        delete ini;
    }
}

// Загрузить все пресеты
template <class T>
void LoadPresetsFromDisk(String fileName, std::vector<T>& data, String s = "CVPreset_") {
    if (!FileExists(fileName)) return;

    TCustomIniFile* ini = new TMemIniFile(fileName);
    try {
        for (size_t i = 0; i < data.size(); i++) {
            String section = s + IntToStr(static_cast<int>(i));
            data[i].Depack(ini, section);
        }
    }
    __finally {
        delete ini;
    }
}
} // namespace storage

#endif