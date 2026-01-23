#ifndef COMPORTUTILS_H
#define COMPORTUTILS_H
#define NEEDLOG
#include <windows.h>
#include <string>
#include <stdexcept> // Для std::runtime_error
//#include <iostream>  // Для вывода в консоль
//#include <vector>    // Для буфера чтения, если нужно
#pragma hdrstop
#include "Logging.h"
// Структура для хранения параметров порта
struct ComPortSettings {
    std::string PortName; // "COM1", "COM2", etc.
    DWORD BaudRate;       // CBR_9600, CBR_115200, etc.
    BYTE  ByteSize;       // 4-8
    BYTE  Parity;         // NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
    BYTE  StopBits;       // ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS

    // Конструктор по умолчанию
    ComPortSettings() :
        PortName("COM1"),
        BaudRate(CBR_9600),
        ByteSize(8),
        Parity(NOPARITY),
        StopBits(ONESTOPBIT)
    {}
};
Logger logfile;

// Класс исключения для ошибок COM-порта
class ComPortError : public std::runtime_error {
public:
    ComPortError(const std::string& message) : std::runtime_error(message) {}
};

// Прототипы функций
HANDLE OpenComPort(const ComPortSettings& settings);
void CloseComPort(HANDLE hCom);
bool WriteToComPort(HANDLE hCom, const char* data, DWORD dataSize, DWORD* bytesWritten = nullptr);
bool ReadFromComPort(HANDLE hCom, char* buffer, DWORD bufferSize, DWORD* bytesRead);
void ShowLastComError(const std::string& prefixMessage); // Вспомогательная



class ComfPort
{
// моё
public:
ComPortSettings settings;
HANDLE hMyComPort = INVALID_HANDLE_VALUE;
DWORD bytesWritten;
DWORD bytesRead;
bool Opened = 0;
ComfPort(std::string portName, bool createOpened = 0);
~ComfPort();
bool Open(std::string portN = "");
bool Close();
bool Write(std::string arg); //должно менять bytesWritten, но пока что не сделано
bool Write(char* arg, int dataSize = -1);
bool Read(char* readBuffer, int DataLength);
};
#endif /// COMPORTUTILS_H