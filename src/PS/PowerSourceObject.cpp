#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
#pragma hdrstop
#include "PowerSourceObject.h"

// Пытается открыть порт, если не уже
bool PowerSource::Connect(std::string comPort) noexcept
{
    pso_logic::trace(std::string("Called Connect with port = \"") + comPort + "\", connection state is");
    if (port_state == State::ENABLED)
    {
        pso_logic::trace("state::enabled");
        return true;
    }
    pso_logic::trace("state::disabled. Attempt to connect");
    port_state = Port.Open(comPort) ? State::ENABLED : State::DISABLED;
    pso_logic::trace(port_state == State::ENABLED ? "Success" : "Failed");
    return static_cast<bool>(port_state);
};

bool PowerSource::Setup(std::string comPort) noexcept
{
	connection_state = static_cast<State>(Connect(comPort) && SetLock(true));
    if (connection_state == State::DISABLED) {
        Teardown();
        return false;
    }
    return true;
	// return static_cast<bool>(connection_state);
}

bool PowerSource::Teardown() noexcept
{
    SetOut(false);
    Sleep(50);
    bool unlocked = SetLock(false);
    if (!unlocked) {
        Sleep(50);
        SetLock(false); // retry
    }
    Sleep(150);
	connection_state = static_cast<State>(!Disconnect());
	return static_cast<bool>(connection_state == State::DISABLED);
}

bool PowerSource::Disconnect() noexcept // повторяет Close из класса ComfPort
{
    if (port_state == State::DISABLED)
    {
        return true;
    };
    try
    {
        if (Port.Close())
        {
            connection_state = State::DISABLED;
            port_state = State::DISABLED;
            return true;
        }
        return false;
    }
    catch (const std::exception &e)
    {
        pso_logic::critical(" COM port std::exception error while PS::Disconnect: " + std::string(e.what()));
    }
    catch (const win_api_port::ComPortError &e)
    {
        pso_logic::critical("Expected COM port error while PS::Disconnect" + std::string(e.what()));
    }
    catch (...)
    {
        pso_logic::critical("Not std::exception unexpected error while PS::Disconnect");
    }
    return false;
};

// Возвращает -1 в качестве ошибки. Не выбрасыват исключений.
float PowerSource::GetVoltage() noexcept
{
    Port.Write(std::string("<02000000000>"));
    std::string response = Port.Read(14);
    if (response.length() < 9)
    {
        pso_logic::error("MSU returned incorrect value after reading Voltage in PS::GetVoltage");
        return -1.0f;
    }
    // Почему то криво работает
    // if (response[1] == '1')
    // {
    //     mode = Mode::VOLTAGE;
    // }
    // else if (response[1] == 'C')
    // {
    //     mode = Mode::CURRENT;
    // }

    if ((response[2] == '2'))
    {
        int value = 0;
        for (int i = 0; i < 6; i++)
        {
            if (!isdigit(response[i + 3]))
            {
                pso_logic::error("MSU returned incorrect value after reading Voltage in PS::GetVoltage");
                return -1.0f;
            }
            value += (int(response[i + 3]) - 48) * ten_pows[i]; // извлекаем цифры, начиная с третей при i = 0 и заканчивая восьмой при i = 5
        }
        pso_logic::debug("Voltage value from PS::GetVoltage is " + std::to_string(value) + "mV");
        return value / 1000.0f;
    }
    else
    {
        pso_logic::error("MSU returned incorrect value after reading Voltage in PS::GetVoltage");
    }
    return -1.0f;
};

bool PowerSource::SetVoltage(float voltage) noexcept
{
    if (voltage < 0 )
    {
        pso_logic::error("Attempt to set voltage less than 0 in PS::SetVoltage");
        return false;
    }
    if (voltage > 999.998f) {
        pso_logic::error("Attempt to set voltage more than 999.998 in PS::SetVoltage");
        return false;
    }
    int int_voltage = static_cast<int>(voltage * 1000);
    std::ostringstream ss;
    ss << "<01" << std::setfill('0') << std::setw(6) << int_voltage << "000>";
    std::string value = ss.str();
    pso_logic::debug("Setting voltage to " + value + "mV in PS::SetVoltage");
    Port.Write(value);
    value = Port.Read(14);
    if (value.length() < 9)
    {
        pso_logic::error("MSU returned incorrect value after seting Voltage in PS::SetVoltage");
        return false;
    }
    if ((value[2] == '1') && (value[3] == 'O') && (value[4] == 'K'))
    {
        return 1;
    }
    pso_logic::error("MSU returned incorrect value after seting Voltage in PS::SetVoltage");
    return false;
};

// Возвращает -1 в качестве ошибки. Не выбрасыват исключений.
float PowerSource::GetCurrent() noexcept
{
    Port.Write(std::string("<04000000000>"));
    std::string response = "";
    response = Port.Read(14);
    if (response.length() < 9)
    {
        pso_logic::error("MSU returned incorrect value after getting Current in PS::GetCurrent");
        return -1.0f;
    }

    if (response[1] == '1')
    {
        mode = Mode::VOLTAGE;
    }
    else if (response[1] == 'C')
    {
        mode = Mode::CURRENT;
    }

    if (response[2] == '4')
    {
        int value = 0;
        for (int i = 0; i < 6; i++)
        {
            if (!isdigit(response[i + 3]))
            {
                pso_logic::error("MSU returned incorrect value after getting Current in PS::GetCurrent");
                return -1.0f;
            }
            value += (int(response[i + 3]) - 48) * ten_pows[i]; // извлекаем цифры, начиная с третей при i = 0 и заканчивая восьмой при i = 5
        }
        pso_logic::debug("Current value from PS::GetCurrent is " + std::to_string(value) + "mA");
        return value / 1000.0f;
    }
    pso_logic::error("MSU returned incorrect value after getting Current in PS::GetCurrent");
    return -1.0f;
}

bool PowerSource::SetCurrent(float current) noexcept
{
    if (current < 0)
    {
        pso_logic::error("Attempt to set current less than 0 in PS::SetCurrent");
        return false;
    }
    if (current > 999.998f) {
        pso_logic::error("Attempt to set current more than 999.998 in PS::SetCurrent");
        return false;
    }
    int int_current = static_cast<int>(current * 1000);
    std::ostringstream ss;
    ss << "<03" << std::setfill('0') << std::setw(6) << int_current << "000>";
    std::string value = ss.str();
    pso_logic::debug("Setting current to " + value + "mA in PS::SetCurrent");
    Port.Write(value);

    // bool DisabledOut = 0, EnabledLock = 0;
    // if(LockState == State::DISABLED)  {SetLock(1); Sleep(50);  EnabledLock = 1;}
    // if(OutState == State::ENABLED)  {SetOut(0); Sleep(50); DisabledOut = 1;}

    value = Port.Read(14);
    if (value.length() < 5)
    {
        pso_logic::error("MSU returned incorrect value after seting Current");
        return false;
    }

    // if(DisableOut)  {Sleep(50); SetOut(1);}
    // if(DisableLock)  {Sleep(50); SetLock(0);}

    if ((value[2] == '3') && (value[3] == 'O') && (value[4] == 'K'))
    {
        return true;
    }
    pso_logic::error("MSU returned incorrect value after seting Current");
    return false;
};

bool PowerSource::SetOut(bool setOn) noexcept
{
    std::string cmd = setOn ? "<07000000000>" : "<08000000000>";
    pso_logic::trace("[SetOut] Sending fire-and-forget command: " + cmd);
    if (!Port.Write(cmd))
    {
        pso_logic::error("[SetOut] Port.Write failed for " + cmd);
        return false;
    }
    out_state = setOn ? State::ENABLED : State::DISABLED;
    pso_logic::trace("[SetOut] SUCCESS! out_state is now " + std::string(setOn ? "ENABLED" : "DISABLED"));
    return true;
}

bool PowerSource::SetLock(bool setOn) noexcept
{
    std::string cmd = setOn ? "<09100000000>" : "<09200000000>";
    pso_logic::trace("[SetLock] Sending: " + cmd);
    if (!Port.Write(cmd))
    {
        pso_logic::error("[SetLock] Port.Write failed for " + cmd);
        return false;
    }
    std::string response = Port.Read(14);
    pso_logic::trace("[SetLock] Received (" + std::to_string(response.length()) + " bytes): \"" + response + "\"");

    if (response.length() < 4)
    {
        pso_logic::error("[SetLock] Short response: \"" + response + "\"");
        return false;
    }

    char c2 = response.length() > 2 ? response[2] : ' ';
    char c3 = response.length() > 3 ? response[3] : ' ';
    char c4 = response.length() > 4 ? response[4] : ' ';

    if ((c2 == '9' || c2 == '0' || c2 == '1') &&
        ((c3 == 'O' && c4 == 'K') || c3 == '0' || c3 == '1' || c3 == 'K'))
    {
        lock_state = setOn ? State::ENABLED : State::DISABLED;
        pso_logic::trace("[SetLock] SUCCESS! lock_state is now " + std::string(setOn ? "ENABLED" : "DISABLED"));
        return true;
    }

    pso_logic::error("[SetLock] MSU returned unexpected response: \"" + response + "\"");
    return false;
};