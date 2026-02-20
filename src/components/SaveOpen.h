#ifndef SAVE_OPEN_H
#define SAVE_OPEN_H

#include <System.IniFiles.hpp>
#include <vector>

namespace storage {

// Базовая структура пресета
class IMemData {
public:
  virtual ~IMemData() = default;
  virtual void Pack(TCustomIniFile *ini, String section) const = 0;
  virtual void Depack(TCustomIniFile *ini, String section) = 0;
};

struct CVMemPair : public IMemData {
  float voltage = 0.0f;
  float current = 0.0f;

  // Только объявляем методы, реализация в .cpp
  void Pack(TCustomIniFile *ini, String section) const override;
  void Depack(TCustomIniFile *ini, String section) override;
};

// Сохранить все пресеты (Шаблоны всегда остаются в .h)
template <class T>
void SavePresetsToDisk(String fileName, std::vector<T> &data,
                       String s = "CVPreset_") {
  TMemIniFile *ini = new TMemIniFile(fileName);
  try {
    for (size_t i = 0; i < data.size(); i++) {
      String section = s + IntToStr(static_cast<int>(i));
      data[i].Pack(ini, section);
    }
    ini->UpdateFile();
  } __finally {
    delete ini;
  }
}

// Загрузить все пресеты
template <class T>
void LoadPresetsFromDisk(String fileName, std::vector<T> &data,
                         String s = "CVPreset_") {
  if (!FileExists(fileName))
    return;

  TMemIniFile *ini = new TMemIniFile(fileName);
  try {
    for (size_t i = 0; i < data.size(); i++) {
      String section = s + IntToStr(static_cast<int>(i));
      data[i].Depack(ini, section);
    }
  } __finally {
    delete ini;
  }
}

} // namespace storage

#endif