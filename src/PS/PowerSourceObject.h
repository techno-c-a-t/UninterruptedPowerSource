#ifndef POWERSOURCEOBJECT_H
#define POWERSOURCEOBJECT_H
#include "src/hw/ComPortUtils.h"
#include "src/utils/common_namespace.h"
#include "PSO_log.h"

class PowerSource // не должен бросать исключения, только ошибки в коммандную строку
{
    using State = utils::State;
    using Mode = utils::Mode;
// С++ Builder не тянет
//    using enum utils::State;
//    using enum utils::Mode;
public:

    // конструктор, инициализирует порт по умолчанию com5
    // Port(init_port);
    PowerSource(std::string init_port = "COM5") noexcept : Port(init_port) {};



    // возвращает  Connect(comPort) && SetLock(true)
    bool Setup(std::string comPort) noexcept;

    // пытается SetLock(False) и возвращает !Disconnect
    bool Teardown() noexcept;



    /* Устанавливает значение тока
       Дожидается ответа
    Для предотвращения излищней интеллектуальности просто отправляет комманду на изменение тока.
    Если раскомментить часть кода внутри PowerSourceObject.cpp то сможет работать следующим образом:

    принимать значение тока, если выход открыт:
        1. вызывает функцию LockOn,
        2. блокирует выход,
        3. устанавливает значение тока
        4. Дожидается ответа
        5. разблокировывает выход
    Если выход закрыт, все то же самое без 1 и 5
    Если был выключен режим Lock то быключает его обратно
    */
    bool SetCurrent(float value) noexcept;

    /*принимает значение напряжения, затем:
        1. устанавливает значение тока
        2. Дожидается ответа*/
    bool SetVoltage(float voltage) noexcept;

    float GetCurrent() noexcept;
    float GetVoltage() noexcept;
    bool SetOut(bool setOn) noexcept;
    bool SetLock(bool setOn) noexcept;
    win_api_port::ComPortSettings GetSettings() const noexcept
    {
        return Port.settings;
    }
    State GetOutState() const noexcept
    {
        return out_state;
    }
    State GetLockState() const noexcept
    {
        return lock_state;
    }
    State GetConnectionState() const noexcept
    {
        return connection_state;
    }
    State GetPortState() const noexcept
    {
        return port_state;
    }
    Mode GetMode() const noexcept
    {
        return mode;
    }

private:
    // собственно сам порт
    ComfPort Port;
    // состояния блокировки, подключения и выхода, а так же режим стабилизации
    State out_state = State::DISABLED, lock_state = State::DISABLED;

    // connection - определяет буквально подключение
    State connection_state = State::DISABLED;
    State port_state = State::DISABLED;
    Mode mode = Mode::VOLTAGE;

    // разрыв связи с БП, повторяет Close из класса ComfPort
    bool Disconnect() noexcept;

    // подключение (начало связи) с БП, повторяет Open из класса ComfPort
    bool Connect(std::string comPort) noexcept;


    // нужны для того чтобы не писать два раза в подсвете напряжения и тока
    static constexpr int ten_pows[6] = {100'000, 10'000, 1'000, 100, 10, 1};
};
#endif