#include "SaveOpen.h"
#include "UPSource.h" // Теперь можно подключать здесь

namespace storage {

void CVMemPair::Pack(TCustomIniFile *ini, String section) const {
    // Теперь компилятор знает, что такое TForm1 и его статический метод formatValue
    ini->WriteString(section, "Voltage", 
                     String(TForm1::formatValue(voltage, 4, 2).c_str()));
    ini->WriteString(section, "Current", 
                     String(TForm1::formatValue(current, 4, 2).c_str()));
}

void CVMemPair::Depack(TCustomIniFile *ini, String section) {
    // Используем ReadString + заменяем запятую на точку, если нужно, 
    // либо просто ReadFloat (но ReadFloat зависит от системного разделителя)
    voltage = ini->ReadFloat(section, "Voltage", 0.0f);
    current = ini->ReadFloat(section, "Current", 0.0f);
}

} // namespace storage