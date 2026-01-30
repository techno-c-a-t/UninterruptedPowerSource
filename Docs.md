#### Note: таймауты в файле ComPortUtls там где идет открытие порта на уровне WinApi. Дублирование на момент 30 января 2026:
    ```cpp
    timeouts.ReadIntervalTimeout = 15;
    timeouts.ReadTotalTimeoutMultiplier = 2;
    timeouts.ReadTotalTimeoutConstant = 70; //Увеличим немного для консоли
    timeouts.WriteTotalTimeoutMultiplier = 2;
    timeouts.WriteTotalTimeoutConstant = 100;
    ```
#### Note: Параметры открытия порта полностью там же, гдк и таймауты. Дублироваине на момент 30 января 2026:
    ```cpp
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
    ```