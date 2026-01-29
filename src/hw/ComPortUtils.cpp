#include <string> // Для std::to_string
#include <stdexcept>    // Для std::runtime_error и базового std::exception
#include <system_error>  // Для std::system_error, std::error_code и std::errc
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
    // std::transform(portNameUpper.begin(), portNameUpper.end(), portNameUpper.begin(), ::toupper);

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
    // ShowMessage("In function ReadFromComPort came Buffer with size " +  AnsiString(strlen(buffer))+ " and DWORD bufferSize is "+ AnsiString(bufferSize));
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

ComfPort::ComfPort(std::string portName, bool createOpened)
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

// Пытается открыть порт, ловит std::exception, возвращает 1 или 0
bool ComfPort::Open(std::string portN)
{
    if (portN != "")
        settings.PortName = portN;
    if (!Opened)
    {
        try
        {
            hMyComPort = win_api_port::OpenComPort(settings);
            Opened = 1;
            // ShowMessage("COM port " +UnicodeString(settings.PortName.c_str()) + " opened successfully.");
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
    }
    else
        return 1;
    return 0;
}
bool ComfPort::Close()
{
    while (Opened)
    {
        int i = 0;
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
            win_api_port::debug("Standard Error while closing: " + std::string(ex.what())+ "\nRetrying");
        }
        if (i++ > 10)
        {
            win_api_port::critical("Can't close opened port. Port not closed yet. Operation of closing terminated after 10 attempts");
            throw std::system_error(
            std::make_error_code(std::errc::io_error), 
            "Failed to close port after 10 attempts"); // Ваш доп. текст
            return 0;
        };
        Sleep(10);
    }
    return 0;
}

//Дальше не проверял код!!


bool ComfPort::Write(std::string arg) // должно менять bytesWritten, но пока что не сделано
{

    if (!Opened)
    {
        ShowMessage("Port not opened.");
        return 0;
    }
    short length = arg.length();
    char *buffer = new char[length + 1];
    for (int i = 0; i < length; i++)
        buffer[i] = arg[i];
    buffer[length] = '\0';
    bool ans;
    bytesWritten = static_cast<DWORD>(0);
    logfile.nestLevel++;
    try
    {
        ans = WriteToComPort(hMyComPort, buffer, static_cast<DWORD>(length + 2), &bytesWritten);
    }
    catch (const ComPortError &ex)
    {
        ShowMessage("COM Port Error whille writing: " + AnsiString(ex.what()));
        logfile.log(std::string("Writing string failed. Message: ") + std::string(buffer) + std::string(" error") + std::string(ex.what()));
        logfile.nestLevel--;
        return 0;
    }
    catch (const std::exception &ex)
    {
        ShowMessage("Standard Error while writing: " + AnsiString(ex.what()));
        logfile.log(std::string("Writing string failed. Message: ") + std::string(buffer) + std::string(" error") + std::string(ex.what()));
        logfile.nestLevel--;
        return 0;
    }
    logfile.log(std::string("Wrote: ") + std::string(buffer));
    logfile.nestLevel--;
    return ans;
}

bool ComfPort::Write(char *arg, int dataSize)
{
    if (!Opened)
    {
        ShowMessage("Port not opened");
        return 0;
    }
    if (dataSize = -1)
        dataSize = strlen(arg);
    char *buffer = new char[dataSize + 1];
    for (int i = 0; i < dataSize; i++)
        buffer[i] = arg[i];
    buffer[dataSize] = '\0';
    // ShowMessage("Sending:");
    // for(int i = 0; i<dataSize;i++)  ShowMessage(int(buffer[i]));
    bool ans;
    bytesWritten = static_cast<DWORD>(0);
    logfile.nestLevel++;
    try
    {
        ans = WriteToComPort(hMyComPort, buffer, static_cast<DWORD>(dataSize + 2), &bytesWritten);
    }
    catch (const ComPortError &ex)
    {
        ShowMessage("COM Port Error whille writing: " + AnsiString(ex.what()));
        logfile.log(std::string("Writing string failed. Message: ") + std::string(buffer) + std::string(" error") + std::string(ex.what()));
        logfile.nestLevel--;
        return 0;
    }
    catch (const std::exception &ex)
    {
        ShowMessage("Standard Error while writing: " + AnsiString(ex.what()));
        logfile.log(std::string("Writing string failed. Message: ") + std::string(buffer) + std::string(" error") + std::string(ex.what()));
        logfile.nestLevel--;
        return 0;
    }
    logfile.log(std::string("Wrote: ") + std::string(buffer));
    logfile.nestLevel--;
    return ans;
}
bool ComfPort::Read(char *readBuffer, int DataLength)
{
    if (!Opened)
    {
        ShowMessage("Port not opened");
        return 0;
    }
    if (DataLength == -1)
        DataLength = strlen(readBuffer);
    bool ans;
    // if(DataLength = 0)  DataLength = sizeof(readBuffer);
    // std::cout<<"Data length is "<<DataLength<<std::endl;
    // ShowMessage("In ComfPort::Read DataLength is " + AnsiString(DataLength));
    bytesRead = static_cast<DWORD>(0);
    logfile.nestLevel++;
    try
    {
        ans = ReadFromComPort(hMyComPort, readBuffer, DataLength, &bytesRead);
        // ShowMessage("Receiving");
        // for(int i = 0; i<DataLength; i++)	ShowMessage(int(readBuffer[i]));
    }
    catch (const ComPortError &ex)
    {
        ShowMessage("COM Port Error whille reading: " + AnsiString(ex.what()));
        logfile.log(std::string("Reading string failed. Error") + std::string(ex.what()));
        logfile.nestLevel--;
        return 0;
    }
    catch (const std::exception &ex)
    {
        ShowMessage("Standard Error while reading: " + AnsiString(ex.what()));
        logfile.log(std::string("Reading string failed. Error") + std::string(ex.what()));
        logfile.nestLevel--;
        return 0;
    }
    // for(int i = 0; i< strlen(readBuffer); i++)  ShowMessage(int(readBuffer[i]));
    logfile.log(std::string("Read: ") + std::string(readBuffer));
    logfile.nestLevel--;
    return ans;
}
