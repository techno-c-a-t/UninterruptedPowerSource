#include <algorithm>    // std::transform
#include <string>       // Для std::to_string
#include <stdexcept>    // Для std::runtime_error и базового std::exception
#include <system_error> // Для std::system_error, std::error_code и std::errc
#include <cctype>
#include <sstream> // sstringstream
#include <iomanip> // для полученя адреса как строки
#pragma hdrstop
#include "ComPortUtils.h" // Если используете отдельный .h
#include "ComPortUtilsLogging.h"
// Вспомогательная функция для отображения последней ошибки WinAPI
void win_api_port::ShowLastComError(const std::string &prefixMessage)
{
    DWORD dwError = GetLastError();
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        NULL);
    std::string errorMsg = prefixMessage + ": (" + std::to_string(dwError) + ") " + (char *)lpMsgBuf;
    LocalFree(lpMsgBuf);
    error(errorMsg);
    throw ComPortError(errorMsg);
}

// открывает порт через WinAPI. Умеет пожирать только COMX. Если форат не такой, бросит исколючение.
HANDLE win_api_port::OpenComPort(const ComPortSettings &settings)
{
    HANDLE hCom;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    std::string fullPortName = settings.PortName;
    // Преобразование PortName к верхнему регистру для сравнения
    std::string portNameUpper = settings.PortName;
    std::transform(portNameUpper.begin(), portNameUpper.end(), portNameUpper.begin(), [](unsigned char c)
                   { return std::toupper(c); });

    // Для COM10 и выше нужен префикс "\\\\.\\"
    // Проверяем, что начинается с "COM" и длина > 4 ("COMx")
    // и что после "COM" идут цифры, и число >= 10
    if (portNameUpper.rfind("COM", 0) == 0 && settings.PortName.length() > 4)
    {
        std::string numPart = settings.PortName.substr(3);
        bool allDigits = true;
        for (char c : numPart)
        {
            if (!std::isdigit(c))
            {
                allDigits = false;
                break;
            }
        }
        if (allDigits)
        {
            try
            {
                if (std::stoi(numPart) >= 10)
                {
                    fullPortName = "\\\\.\\" + settings.PortName;
                }
            }
            catch (const std::invalid_argument &)
            {
                // Не число, оставляем как есть
            }
            catch (const std::out_of_range &)
            {
                // Слишком большое число, оставляем как есть
            }
        }
    }

    hCom = CreateFileA( // Используем CreateFileA для std::string / const char*
        fullPortName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0, // FILE_FLAG_OVERLAPPED для асинхронного режима, 0 для синхронного
        NULL);

    if (hCom == INVALID_HANDLE_VALUE)
    {
        win_api_port::ShowLastComError("Error opening COM port " + settings.PortName);
        return INVALID_HANDLE_VALUE; // Исключение уже брошено
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hCom, &dcbSerialParams))
    {
        ShowLastComError("Error getting COM port state");
        CloseHandle(hCom);
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = settings.BaudRate;
    dcbSerialParams.ByteSize = settings.ByteSize;
    dcbSerialParams.Parity = settings.Parity;
    dcbSerialParams.StopBits = settings.StopBits;
    dcbSerialParams.fBinary = TRUE;
    dcbSerialParams.fParity = (settings.Parity == NOPARITY) ? FALSE : TRUE;
    dcbSerialParams.fOutxCtsFlow = FALSE;
    dcbSerialParams.fOutxDsrFlow = FALSE;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_ENABLE;
    dcbSerialParams.fInX = FALSE;
    dcbSerialParams.fOutX = FALSE;
    dcbSerialParams.fErrorChar = FALSE;
    dcbSerialParams.fNull = FALSE;
    dcbSerialParams.fAbortOnError = FALSE;

    if (!SetCommState(hCom, &dcbSerialParams))
    {
        ShowLastComError("Error setting COM port state");
        CloseHandle(hCom);
        return INVALID_HANDLE_VALUE;
    }

    timeouts.ReadIntervalTimeout = 15;
    timeouts.ReadTotalTimeoutMultiplier = 2;
    timeouts.ReadTotalTimeoutConstant = 70; // Увеличим немного для консоли
    timeouts.WriteTotalTimeoutMultiplier = 2;
    timeouts.WriteTotalTimeoutConstant = 100;

    if (!SetCommTimeouts(hCom, &timeouts))
    {
        ShowLastComError("Error setting COM port timeouts");
        CloseHandle(hCom);
        return INVALID_HANDLE_VALUE;
    }

    if (!PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR))
    {
        ShowLastComError("Error purging COM port buffers");
        // Не критично, чтобы закрывать порт, но стоит знать
    }

    return hCom;
}

// Перехватчик ошибки закрытия для ее обработки
void win_api_port::CloseComPort(HANDLE hCom)
{
    if (hCom != INVALID_HANDLE_VALUE && hCom != NULL)
    {
        if (!CloseHandle(hCom))
        {
            ShowLastComError("Error closing COM port");
        }
    }
}

// побитово пишет в порт, начиная с data и заканчивая data+data_size, меняет указатель pBytesWritten
bool win_api_port::WriteToComPort(HANDLE hCom, const char *data, DWORD dataSize, DWORD *pBytesWritten)
{
    DWORD bytesWrittenInternal;
    DWORD *ptrBytesWritten = pBytesWritten ? pBytesWritten : &bytesWrittenInternal;

    if (!WriteFile(hCom, data, dataSize, ptrBytesWritten, NULL))
    {
        ShowLastComError("Error writing to COM port");
        return false;
    }
    if (!FlushFileBuffers(hCom))
    {
        ShowLastComError("Error flushing COM port write buffer");
        // Не всегда критично
    }
    return true;
}

bool win_api_port::ReadFromComPort(HANDLE hCom, char *buffer, DWORD bufferSize, DWORD *bytesRead)
{
    *bytesRead = 0;
    // win_api_port::error("In function ReadFromComPort came Buffer with size " +  std::string(strlen(buffer))+ " and DWORD bufferSize is "+ std::string(bufferSize));
    if (!ReadFile(hCom, buffer, bufferSize, bytesRead, NULL))
    {
        DWORD dwError = GetLastError();
        // ReadFile может вернуть FALSE при таймауте, если не было прочитано ни одного байта.
        // Однако, если ReadTotalTimeoutConstant и ReadTotalTimeoutMultiplier не равны нулю,
        // и ReadIntervalTimeout равен MAXDWORD, то ReadFile вернет TRUE, даже если *bytesRead == 0.
        // Для более простой обработки, считаем ошибкой, если ReadFile вернул FALSE и ничего не прочитано.
        if (dwError != ERROR_SUCCESS && *bytesRead == 0)
        {
            // Не бросаем исключение для таймаута, чтобы можно было продолжить
            // Если это не таймаут, то это проблема
            if (dwError != ERROR_TIMEOUT)
            { // ERROR_TIMEOUT не определен в <windows.h> напрямую, но это общий случай
              // Обычно таймауты не приводят к GetLastError() отличному от 0, ReadFile просто возвращает 0 байт.
              // Проверяем, что ошибка не просто таймаут (который мы ожидаем)
                ShowLastComError("Error reading from COM port (not a simple timeout)");
                return false;
            }
            // Если это был таймаут (ReadFile вернул FALSE, *bytesRead=0, GetLastError() может быть 0 или специфичен)
            // это не ошибка в смысле сбоя, просто данных нет.
        }
        // Если ReadFile вернул FALSE, но что-то прочитал, это тоже нештатно, но данные есть.
        // Если ReadFile вернул FALSE, ничего не прочитал, и GetLastError() != 0 -> проблема.
        // В нашем случае, ShowLastComError бросит исключение если GetLastError() != 0 и это не таймаут
    }
    return true; // Возвращаем true, даже если прочитано 0 байт (таймаут)
}

ComfPort::ComfPort(std::string portName, bool createOpened) noexcept
{
    // Можно добавить запрос других настроек (BaudRate и т.д.)
    settings.BaudRate = CBR_9600; // Пример
    settings.ByteSize = 8;
    settings.Parity = NOPARITY;
    settings.StopBits = ONESTOPBIT;
    settings.PortName = portName;
    Opened = 0;
    if (createOpened)
        Open();
}
ComfPort::~ComfPort()
{
    Close();
}

// Пытается открыть порт, ловит ComPortError, std::exception, и все что угодно
// при попытке поткрыть порт. возвращает 1 или 0.
// Можно открывать открытый порт, ничего не произойдет (по идее)
bool ComfPort::Open(std::string portN) noexcept
{
    if (portN != "")
        settings.PortName = portN;
    if (!Opened)
    {
        try
        {
            hMyComPort = win_api_port::OpenComPort(settings);
            Opened = 1;
            // win_api_port::trace("COM port " +UnicodeString(settings.PortName.c_str()) + " opened successfully.");
            return 1;
        }
        catch (const win_api_port::ComPortError &ex)
        {
            win_api_port::error("COM Port Error whille opening: " + std::string(ex.what()));
            return 0;
        }
        catch (const std::exception &ex)
        {
            win_api_port::error("Standard Error while opening: " + std::string(ex.what()));
            return 0;
        }
        catch (...)
        {
            win_api_port::critical("Other error in ComfPort::Open. It is not std::exception");
            return 0;
        }
    }
    else
        return 1;
    return 0;
}

// Делает 10 попыток закрыть порт, в промежутках слип на 10 мс. Если за 10 не получилось, бросает
//  throw std::system_error(
//      std::::make_error_code(std::errc::io_error),
//      "Failed to close port after 10 attempts"
//   );
bool ComfPort::Close()
{
    int i = 0;
    while (Opened)
    {

        try
        {
            if (hMyComPort != INVALID_HANDLE_VALUE)
            {
                win_api_port::CloseComPort(hMyComPort);
                Opened = 0;
                hMyComPort = INVALID_HANDLE_VALUE;
                return 1;
            }
        }
        catch (const win_api_port::ComPortError &ex)
        {
            win_api_port::debug("COM Port Error whille closing: " + std::string(ex.what()) + "\nRetrying");
        }
        catch (const std::exception &ex)
        {
            win_api_port::debug("Standard Error while closing: " + std::string(ex.what()) + "\nRetrying");
        }
        if (i++ > 10)
        {
            win_api_port::critical("Can't close opened port. Port not closed yet. Operation of closing terminated after 10 attempts");
            throw std::system_error(
                std::make_error_code(std::errc::io_error),
                "Failed to close port after 10 attempts");
            return 0;
        };
        Sleep(10);
    }
    return 0;
}

// Отправляет arg.length() байт, без добавления \0. noexcept. Проверяет, что порт открыт. Если длина строки 0 то сразу возвращет 1
bool ComfPort::Write(std::string arg) noexcept
{
    bytesWritten = 0;
    if (!Opened)
    {
        win_api_port::error("Port not opened.");
        return 0;
    }

    if (arg.length() == 0)
    {
        win_api_port::debug("In ComfPortWrite (std::string) length of string is 0, returning 1");
        return 1;
    }
    bool ans;
    win_api_port::debug("Sending as string: " + arg);
    try
    {
        ans = win_api_port::WriteToComPort(hMyComPort, arg.c_str(), static_cast<DWORD>(arg.length()), &bytesWritten);
    }
    catch (const win_api_port::ComPortError &ex)
    {
        win_api_port::error("COM Port Error whille writing: " + std::string(ex.what()));
        win_api_port::debug(
            std::string("Writing string failed. Message: \n") + arg +
            std::string(" \nError\n") + std::string(ex.what()) +
            "\nBytes written: " + std::to_string(bytesWritten));
        return 0;
    }
    catch (const std::exception &ex)
    {
        win_api_port::error("Standard Error while writing: " + std::string(ex.what()));
        win_api_port::debug(
            std::string("Writing string failed. Message:\n") + arg +
            std::string("\nerror\n") + std::string(ex.what()));
        return 0;
    }
    catch (...)
    {
        win_api_port::critical("Other error in ComfPort::Write. It is not std::exception");
        return 0;
    }
    win_api_port::debug(std::string("Wrote: ") + std::to_string(bytesWritten));
    return ans;
}

// пишет DataSize байт насичная с указателя arg. не кидает ошибку нуллпоинтера или закрытого потра, просто возвращает 0.
// Обрабатывает только ComPortError и std::exception
bool ComfPort::Write(char *arg, size_t dataSize)
{
    bytesWritten = 0;
    if (!Opened)
    {
        win_api_port::error("Port not opened");
        return 0;
    }
    if (arg == nullptr)
    {
        win_api_port::error("Attempt to write " + std::to_string(dataSize) + " bytes from nullptr");
        return 0;
    }
    std::string information = "Sending as bytes: ";
    for (int i = 0; i < dataSize; i++)
        information += std::to_string(static_cast<unsigned>(arg[i])) + " ";
    win_api_port::debug(information);
    bool ans;
    try
    {
        ans = win_api_port::WriteToComPort(hMyComPort, arg, static_cast<DWORD>(dataSize), &bytesWritten);
    }
    catch (const win_api_port::ComPortError &ex)
    {
        win_api_port::error("COM Port Error whille writing: " + std::string(ex.what()));
        win_api_port::debug(std::string("Writing bytes failed. Message:\n") + std::string(arg) + std::string("\nComPortError:\n") + std::string(ex.what()));
        return 0;
    }
    catch (const std::exception &ex)
    {
        win_api_port::error("Standard Error while writing: " + std::string(ex.what()));
        win_api_port::debug(std::string("Writing bytes failed. Message:\n") + std::string(arg) + std::string("\nstd::exception:\n") + std::string(ex.what()));
        return 0;
    }
    win_api_port::debug(std::string("Wrote: ") + std::string(arg));
    return ans;
}

// Читает DataLength байт из порта или считает DataLength из буффера, если DataLength = 0.
//  Проверяет readBuffer на nullptr и что порт открыт. ловит std::exception и comporterror
bool ComfPort::Read(char *readBuffer, size_t DataLength)
{
    bytesRead = 0;
    if (readBuffer == nullptr)
    {
        win_api_port::error("Attempt to read " + std::to_string(DataLength) + " bytes from nullptr");
        return 0;
    }
    if (!Opened)
    {
        win_api_port::error("Port not opened");
        return 0;
    }
    if (DataLength == 0)
    {
        win_api_port::debug("DataLength is 0 in Read(char* ) function. return 0 ");
        return 0;
    }
    bool ans;
    std::stringstream ss;
    ss << std::hex << std::showbase << reinterpret_cast<uintptr_t>(readBuffer);
    std::string result = ss.str();
    win_api_port::debug("Reding " + std::to_string(DataLength) + " bytes in " + result);
    try
    {
        ans = win_api_port::ReadFromComPort(hMyComPort, readBuffer, DataLength, &bytesRead);
        // win_api_port::debug("Receiving");
        // for(int i = 0; i<DataLength; i++)	win_api_port::debug(int(readBuffer[i]));
    }
    catch (const win_api_port::ComPortError &ex)
    {
        win_api_port::error("COM Port Error whille reading: " + std::string(ex.what()));
        return 0;
    }
    catch (const std::exception &ex)
    {
        win_api_port::error("Standard Error while reading: " + std::string(ex.what()));
        return 0;
    }
    win_api_port::debug(std::string("Was read: ") + std::string(readBuffer));
    return ans;
}

// Читает DataLength байт из порта в буфер, а затем делает из буфера std::string
//  Проверяет new на exception и что порт открыт.
// ловит std::exception,  comporterror и все остальное. noexcept.
std::string ComfPort::Read(size_t DataLength) noexcept
{
    bytesRead = 0;
    if (!Opened)
    {
        win_api_port::error("Port not opened");
        return "";
    }
    if (DataLength == 0)
    {
        win_api_port::debug("DataLength is 0 in Read - noexcept function. Returning");
        return "";
    }
    char *readBuffer;
    try
    {
        readBuffer = new char[DataLength];
    }
    catch (...)
    {
        win_api_port::critical("New dropped error in Read-noexcept. Probably out of memory.");
        return "";
    }
    bool ans;
    std::stringstream ss;
    ss << std::hex << std::showbase << reinterpret_cast<uintptr_t>(readBuffer);
    std::string result = ss.str();
    win_api_port::debug("Reding " + std::to_string(DataLength) + " bytes in " + result);
    try
    {
        ans = win_api_port::ReadFromComPort(hMyComPort, readBuffer, DataLength, &bytesRead);
        // win_api_port::debug("Receiving");
        // for(int i = 0; i<DataLength; i++)	win_api_port::debug(int(readBuffer[i]));
    }
    catch (const win_api_port::ComPortError &ex)
    {
        win_api_port::error("COM Port Error whille reading: " + std::string(ex.what()));
        delete[] readBuffer;
        return "";
    }
    catch (const std::exception &ex)
    {
        win_api_port::error("Standard Error while reading: " + std::string(ex.what()));
        delete[] readBuffer;
        return "";
    }
    catch (...)
    {
        win_api_port::error("Not-derrived from std::exception error while reading.");
        delete[] readBuffer;
        return "";
    }
    std::string ans_str(readBuffer, bytesRead);
    win_api_port::debug(std::string("Was read: ") + ans_str);
    delete[] readBuffer;
    return ans_str;
}
