#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <conio.h>    // Для _kbhit() и _getch()
#include <windows.h>  // Для Sleep()
#include "src/core/engine.h" // Путь к твоему движку

using namespace std;

// Функция для отрисовки интерфейса
void DrawUI(const PSEngine& eng, const string& currentPort) {
    // system("cls"); // Очистка экрана
    cout << "====================================================" << endl;
    cout << "          POWER SOURCE CONTROL (CONSOLE TEST)       " << endl;
    cout << "====================================================" << endl;

    // СТАТУС
    cout << " STATUS: " << (eng.Connected() ? "[CONNECTED]" : "[DISCONNECTED]") 
         << " | PORT: " << currentPort << endl;
    
    // ТЕЛЕМЕТРИЯ
    float v = eng.GetLastVoltage();
    float i = eng.GetLastCurrent();
    double vAge = eng.GetVoltageAgeUs() / 1000000.0; // в секунды
    double iAge = eng.GetCurrentAgeUs() / 1000000.0;

    cout << fixed << setprecision(3);
    cout << " VOLTAGE: " << setw(7) << v << " V  (Age: " << setprecision(1) << vAge << "s)" << endl;
    cout << " CURRENT: " << setw(7) << i << " A  (Age: " << setprecision(1) << iAge << "s)" << endl;
    cout << "====================================================" << endl;

    // МЕНЮ
    cout << " 1: Set Volts (0-10000)  | 2: Set Amps (0-10000)" << endl;
    cout << " 3: Output ON            | 4: Output OFF" << endl;
    cout << " 5: Lock ON              | 6: Lock OFF" << endl;
    cout << " 7: CONNECT              | 8: DISCONNECT" << endl;
    
    if (!eng.Connected()) {
        cout << " 9: CHANGE PORT NAME     |0: EXIT PROGRAM" << endl;
    }
    cout << " -: Show Menu            | +: Show ErrorBuffer" << endl;
    cout << "====================================================" << endl;
    
    // ПОСЛЕДНЯЯ ОШИБКА (если есть)
    auto errors = eng.GetErrors();
    if (!errors.empty()) {
        cout << " LAST ERROR: " << errors.back().message << endl;
    }
    cout << " Press key (0-9) to send command..." << endl;
}

int main() {
    PSEngine engine;
    string portName = "COM5";
    bool running = true;
    DrawUI(engine, portName);
    while (running) {
        // 1. ТИК ДВИЖКА (100 мс)
        engine.Tick();

        // 2. ОТРИСОВКА
        

        // 3. ОБРАБОТКА ВВОДА (неблокирующая)
        if (_kbhit()) {
            char key = _getch();
            int inputVal = 0;

            if (key == '0') running = false;
            
            // Если нужны числовые параметры (V или A)
            if (key == '1' || key == '2') {
                cout << "\n Enter value (0-10000): ";
                cin >> inputVal;
                if (inputVal < 0) inputVal = 0;
                if (inputVal > 10000) inputVal = 10000;
                
                if (key == '1') engine.SetTargetVoltage(inputVal / 1000.0f);
                else engine.SetTargetCurrent(inputVal / 1000.0f);
            }
            // Команды переключения
            else if (key == '3') engine.SetOutput(true);
            else if (key == '4') engine.SetOutput(false);
            else if (key == '5') engine.SetLock(true);
            else if (key == '6') engine.SetLock(false);
            else if (key == '7') engine.Connect(portName);
            else if (key == '8') engine.Disconnect();
            else if (key == '9' && !engine.Connected()) {
                cout << "\n Enter new port name (e.g. COM3): ";
                cin >> portName;
            }
            else if (key == '-') {
                DrawUI(engine, portName);
            } else if (key == '=') {
                engine.flush_errors();
            }
        }

        Sleep(100); // Пауза между тиками
    }

    return 0;
}