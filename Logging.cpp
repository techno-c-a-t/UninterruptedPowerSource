#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Logging.h"

    void Logger::handleError(std::string msg)
    {
        #if defined(__BORLANDC__) || defined(__CODEGEARC__)
    // Код компилируется в C++ Builder
    #include <windows.h>
    #include <System.SysUtils.hpp> // для UnicodeString
        #ifdef _UNICODE
            ShowMessage( UnicodeString(msg.c_str()).c_str() );
        #else
            ShowMessage(errorMsg.c_str());
        #endif
        #else
        #include <iostream>
        std::cerr<<msg<<std::endl;
        #endif
    }
    std::string Logger::getCurrentTime()
    {
        //время
        auto now = std::chrono::system_clock::now();
        //время в более удобной форме time_t
        auto time_t_variant = std::chrono::system_clock::to_time_t(now);
        //time_t походу не поддерживает миллисекунды, поэтому поддерживаю их я
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch())%1000;
        std::stringstream strStream;
        struct tm tstruct;
        char buffer[80];
        #ifdef _WIN32
        localtime_s (&tstruct, &time_t_variant);
        #else
        localtime_r(&time_t_variant, &tstruct);
        #endif
        strStream << std::put_time(&tstruct, "%T");
        strStream <<'.'<<std::setw(3)<<std::setfill('0')<<ms.count();
        return strStream.str();
    }

    void Logger::log(const std::string msg)
    {
        std::ofstream ofs(filename, std::ios_base::app);
        if(!ofs.is_open())
        {
            ofs.open(filename, std::ios_base::out);
        }
        if(!ofs.is_open())
        {
            handleError("Could not open or create file: "+ filename);
            return;
        }
        for(int i = 0; i<nestLevel; i++)    ofs<<"-";
        ofs<<msg<<" -- "<<std::endl;
    }