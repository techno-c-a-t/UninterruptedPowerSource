#ifndef COMMON_NAMESPACE_H
#define COMMON_NAMESPACE_H
#include <windows.h>
#include <string>
namespace utils {
    // Структура для хранения параметров порта
    struct ComPortSettings
    {
        std::string PortName; // "COM1", "COM2", etc.
        DWORD BaudRate;       // CBR_9600, CBR_115200, etc.
        BYTE ByteSize;        // 4-8
        BYTE Parity;          // NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
        BYTE StopBits;        // ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS

        // Конструктор по умолчанию
        ComPortSettings() : PortName("COM1"),
                            BaudRate(CBR_9600),
                            ByteSize(8),
                            Parity(NOPARITY),
                            StopBits(ONESTOPBIT)
        {
        }
    };

    enum class State
    {
        DISABLED = 0,
        ENABLED = 1
    };
    enum class Mode
    {
        VOLTAGE,
        CURRENT
    };
}; // namespace utils
#endif