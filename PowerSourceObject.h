#ifndef POWERSOURCEOBJECT_H
#define POWERSOURCEOBJECT_H
#include "ComPortUtils.h"
enum class State
{
    DISABLED,
    ENABLED
};
enum class ConstantValue
{
    VOLTAGE,
    CURRENT
};

class PowerSource //не должен бросать исключения, только ошибки в коммандную строку
{
public:
    ComfPort Port;
    // состояния блокировки и выхода
    State OutState = State::DISABLED, LockState = State::DISABLED;
    ConstantValue Mode = ConstantValue::VOLTAGE;
    //конструктор инициализирует порт по умолчанию com5
    PowerSource():Port("COM5") //Port(new ComfPort("COM5"))
    {};

    //подключение (начало связи) с БП, повторяет Open из класса ComfPort
    bool Connect(std::string comPort);  

    //разрыв связи с БП, повторяет Close из класса ComfPort
    bool Disconnect();

    /*принимает значение тока, если выход открыт:
        1. вызывает функцию LockOn, 
        2. блокирует выход,
        3. устанавливает значение тока
        4. Дожидается ответа
        5. разблокировывает выход
    Если выход закрыт, все то же самое без 1 и 5
    !!!Если был выключен режим Lock то быключает его обратно*/
    bool SetCurrent(float value);

    /*принимает значение напряжения, затем:
        1. устанавливает значение тока
        2. Дожидается ответа*/
    bool SetVoltage(float voltage); 

    float GetCurrent();
    float GetVoltage();
    bool SetOut(bool setOn);
    bool SetLock(bool setOn);

};
#endif