#ifndef COMPORTUTILS_H
#define COMPORTUTILS_H
#define NEEDLOG
#include <windows.h>
#include <string>
#include <stdexcept> // Для std::runtime_error
// #include <iostream>  // Для вывода в консоль
// #include <vector>    // Для буфера чтения, если нужно
#pragma hdrstop
#include "Logging.h"

namespace win_api_port
{
    // НЕЙРОКОД начался
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
    Logger logfile;

    // Класс исключения для ошибок COM-порта
    class ComPortError : public std::runtime_error
    {
    public:
        ComPortError(const std::string &message) : std::runtime_error(message) {}
    };

    // Прототипы функций
    HANDLE OpenComPort(const ComPortSettings &settings);
    void CloseComPort(HANDLE hCom);
    bool WriteToComPort(HANDLE hCom, const char *data, DWORD dataSize, DWORD *bytesWritten = nullptr);
    bool ReadFromComPort(HANDLE hCom, char *buffer, DWORD bufferSize, DWORD *bytesRead);
    void ShowLastComError(const std::string &prefixMessage); // Вспомогательная

} // namespace win_api_port

// НЕЙРОКОД закончился

/**
 * @brief Класс-обертка над WinAPI для работы с COM-портом.
 * Обеспечивает RAII (?), логирование и обработку ошибок через исключения.
 */
class ComfPort
{
    // моё
public:
    win_api_port::ComPortSettings settings; // стркутура настроек порта
    HANDLE hMyComPort = INVALID_HANDLE_VALUE; // Дескрипотор порта
    DWORD bytesWritten;
    DWORD bytesRead;
    bool Opened = 0; // меняется при открытии и закрытии

    /**
     * @brief Конструктор порта.
     * @param portName Имя порта (например, "COM1" или "COM10").
     * @param createOpened Если true, пытается открыть порт сразу после создания объекта.
     */
    ComfPort(std::string portName, bool createOpened = 0) noexcept; 
   
    ~ComfPort();

    /**
     * @brief Открывает COM-порт.
     * 
     * Пытается открыть порт с заданными настройками. 
     * Перехватывает системные исключения и пишет ошибки в лог.
     * 
     * @param portN Новое имя порта (опционально). Если пустая строка, используется сохраненное ранее.
     * @return true если порт успешно открыт (или уже был открыт).
     * @return false если произошла ошибка при открытии.
     */
    bool Open(std::string portN = "") noexcept;

    /**
     * @brief Закрывает COM-порт.
     * 
     * Делает 10 попыток закрыть порт с паузой 10мс.
     * Если закрыть не удалось, выбрасывает std::system_error.
     * 
     * @return true если успешно закрыт.
     * @return false (или исключение) если не удалось.
     */
    bool Close();

     /**
     * @brief Отправляет строку в порт.
     * 
     * Безопасен по исключением
     * 
     * @param arg Строка для отправки. Нуль-терминатор НЕ отправляется.
     * @return true если запись прошла успешно.
     */
    bool Write(std::string arg) noexcept;

    /**
     * @brief Отправляет массив байт в порт.
     * 
     * Может кидать исключения
     * 
     * @param arg Указатель на буфер данных.
     * @param dataSize Количество байт для отправки.
     * @return true если запись прошла успешно.
     */
    bool Write(char *arg, size_t dataSize = 0);

    /**
     * @brief Читает данные в предоставленный буфер.
     * 
     * @param readBuffer Указатель на буфер, куда будут записаны данные.
     * @param DataLength Максимальное количество байт для чтения.
     * @return true если чтение прошло без ошибок API (даже если прочитано 0 байт).
     */
    bool Read(char *readBuffer, size_t DataLength);

        /**
     * @brief Читает данные и возвращает их в виде строки.
     * 
     * Выделяет память вручную. Обрабатывает ошибки new и возвращает "" с записью в лог.
     * 
     * @param DataLength Максимальное количество байт для чтения.
     * @return std::string Содержит прочитанные данные. Пустая строка при ошибке.
     */
    std::string Read(size_t DataLength) noexcept;
};
#endif /// COMPORTUTILS_H