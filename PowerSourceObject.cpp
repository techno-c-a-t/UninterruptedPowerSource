#include <cmath>
#include <string>
#pragma hdrstop
#include "PowerSourceObject.h"
#if defined(__BORLANDC__) || defined(__CODEGEARC__)
    // Код компилируется в C++ Builder
    #include <windows.h>
    #include <System.SysUtils.hpp> // для UnicodeString
    void handleStringError(const std::string& errorMsg) {
        #ifdef _UNICODE
            OutputDebugString( UnicodeString(errorMsg.c_str()).c_str() );
        #else
            OutputDebugStringA(errorMsg.c_str());
        #endif
    }
#else
    #include <iostream>
    void handleStringError(std::string errorMsg)
    {
        std::cerr<<errorMsg<<std::endl;
    }
#endif

//Пытается открыть порт из задать LOCK 1. Если что то неуспешно, возвращает 0 
bool PowerSource::Connect(std::string comPort)
{
bool success = Port.Open(comPort);
if(success) 
{Sleep(1500);
return SetLock(1);}
else return 0;
}; 
bool PowerSource::Disconnect()//повторяет Close из класса ComfPort
{

try
{
    return Port.Close();
}
catch(const std::exception& e)
{
    handleStringError("Unexpected COM port error while closing: " + std::string(e.what()));
}
catch(const ComPortError& e)
{
    handleStringError("Expected COM port error while closing " + std::string(e.what()));
};
return 0;
};
float PowerSource::GetVoltage()
{
Port.Write("<02000000000>");
char* ans = new char[14];
Port.Read(ans,14);
if((ans[2]=='2'))
{
float answer;
for(int i = 3; i<9; i++)
{
    answer+=(int(ans[i])-48)*pow(10,5-i);
}
//ShowMessage("Value from PSobj is"+FloatToStrF(answer, ffFixed, 6, 3));
return answer; 
}
else 
handleStringError("MSU returned incorrect value after reading Voltage");
return -1;
};
bool PowerSource::SetVoltage(float Voltage)
{
if(Voltage<0) {ShowMessage("Voltage less than 0"); return  0;}
int tmp = Voltage*1000;
std::string value = "000000";
int divisor = 100000; 
for(int i = 0; i < 6; i++) {
    int digit = tmp / divisor;
    value[i] = digit + '0';
    tmp = tmp % divisor;
    divisor /= 10;
    }
std::string tempValue =  "<01" + value +"000>";
Port.Write(tempValue);
char* ans = new char[14];
Port.Read(ans,14);
if((ans[2]=='1')&&(ans[3]=='O')&&(ans[4]=='K'))  return 1;
else 
handleStringError("MSU returned incorrect value after seting Voltage");
return 0;
};
float PowerSource::GetCurrent()
{
Port.Write("<04000000000>");
char* ans = new char[14];
Port.Read(ans,14);
if(ans[1] == '1')   Mode = ConstantValue::VOLTAGE;
if(ans[1] == 'C')   Mode = ConstantValue::CURRENT;
if((ans[2]=='4'))
{
float answer;
for(int i = 3; i<9; i++)
{
    answer+=(int(ans[i])-48)*pow(10,5-i);
}
return answer; 
}
else 
handleStringError("MSU returned incorrect value after getting Current");
return 0;
}
bool PowerSource::SetCurrent(float Current)
{
if(Current<0) {ShowMessage("Current less than 0"); return  0;}
int tmp = Current*1000;
std::string value = "000000";
int divisor = 100000;
for(int i = 0; i < 6; i++) {
    int digit = tmp / divisor;
    value[i] = digit + '0';
    tmp = tmp % divisor;
    divisor /= 10;
    } 
//bool DisableOut = 0, DisableLock = 0;
//if(LockState == State::DISABLED)  {SetLock(1); Sleep(50);  DisableLock = 1;}
//if(OutState == State::ENABLED)  {SetOut(0); Sleep(50); DisableOut = 1;}    
Port.Write("<03" + value +"000>");
char* ans = new char[14];
Port.Read(ans,14);
if(ans[1] == '1')   Mode = ConstantValue::VOLTAGE;
if(ans[1] == 'C')   Mode = ConstantValue::CURRENT;
//if(DisableOut)  {Sleep(50); SetOut(1);}
//if(DisableLock)  {Sleep(50); SetLock(0);}
if((ans[2]=='3')&&(ans[3]=='O')&&(ans[4]=='K')) return 1;
else 
handleStringError("MSU returned incorrect value after seting Current");
return 0;
};
bool PowerSource::SetOut(bool setOn)
{
if(setOn== bool(OutState)) return 1;
if (setOn)
{
bool success = Port.Write("<07000000000>");
if(success == 0)    return 0;
OutState =  State::ENABLED;
}
else
{
bool success = Port.Write("<08000000000>");
if(success == 0)    return 0;
OutState =  State::DISABLED;
}
return 1;
};
bool PowerSource::SetLock(bool setOn)
{
if(setOn== bool(LockState)) return 1;
if(setOn)
{
if(!Port.Write("<09100000000>")) return 0;
}
else
{
if(!Port.Write("<09200000000>")) return 0;
}
char* ans = new char[13];
if(!Port.Read(ans,13)) return 0;
if((ans[2]=='9')&&(ans[3]=='O')&&(ans[4]=='K'))
{
LockState = setOn?State::ENABLED:State::DISABLED;
return 1;
}
return 0;
};