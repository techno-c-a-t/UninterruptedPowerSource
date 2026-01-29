
//---------------------------------------------------------------------------
#include <vcl.h>
#include <cstring>   // Для strlen
#include <stdexcept> // Для исключений
#include <random>
#include <ctime>
#include <deque>
#include <Registry.hpp>        // Для работы с реестром
#include <vector>              // Для временного хранения и сортировки
#include <algorithm>           // Для std::sort
#include <System.SysUtils.hpp> // Для StrToIntDef
#include <queue>
#pragma hdrstop
#include "UPSource.h"
#include "PowerSourceObject.h"
#include "DataProcessing.cpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
// #include <System.SysUtils.hpp> // TBytes и AnsiString обычно определены здесь

/**
 * @brief Преобразует динамический массив байтов TBytes в AnsiString.
 * @param bytes Входной массив TBytes.
 * @return AnsiString, созданная из байтов.
 * @note Эта функция предполагает, что байты в TBytes представляют собой
 *       корректную последовательность символов для AnsiString
 *       (обычно в кодировке Windows-1251 или другой ANSI-кодировке,
 *       зависящей от локали системы). Она НЕ выполняет перекодировку
 *       из UTF-8 или других многобайтовых кодировок.
 *       Нулевые байты внутри TBytes будут включены в AnsiString.
 */
// ComfPort TCP("COM1");

//---------------------------------------------------------------------------
// NOT interface
//---------------------------------------------------------------------------

struct command
{
    int index = 0;
    float value = 0;
};

PowerSource UPS;
AnsiString receivedData = "";
TForm1 *Form1;
std::deque<float> voltageData = {}; // resize(n, value); push_back(val); pop_front();
std::queue<command> commandQueue = {};
long emulatedTime = 100; // эмуляция времени
bool timerStarted = 1;

//---------------------------------------------------------------------------
void addPointToVoltageGraphic(float x)
{
    Form1->Chart1->Series[0]->Clear();
    voltageData.push_back(++emulatedTime);
    voltageData.push_back(x);
    if (voltageData.size() > 2)
    {
        for (short i = voltageData.size() - 2; i > 0; i -= 2)
        {
            Form1->Chart1->Series[0]->AddXY(voltageData[i], voltageData[i + 1]);
            // if((voltageData[i]==0)&&(emulatedTime>200))                       ShowMessage(i);
            // Memo2->Lines->Add(AnsiString(i));
        }
    };
    /*
    Form1->Chart1->Series[0]->AddXY(
    voltageData[voltagePositionRewrite] = ++emulatedTime,
    voltageData[voltagePositionRewrite+1] = x
    );
    voltagePositionRewrite+=2;
    if(voltagePositionRewrite>98)      voltagePositionRewrite=0;*/
}
//---------------------------------------------------------------------------

bool __fastcall CompareComPorts(const String &s1, const String &s2)
{
    int num1 = StrToIntDef(s1.SubString(4, s1.Length() - 3), -1);
    int num2 = StrToIntDef(s2.SubString(4, s2.Length() - 3), -1);
    if (num1 == -1 && num2 != -1)
        return true; // s1 "меньше" (идет раньше)
    if (num2 == -1 && num1 != -1)
        return false; // s2 "меньше"
    if (num1 == -1 && num2 == -1)
        return s1 < s2; // оба не стандартные, алфавитно

    return num1 < num2;
}
//---------------------------------------------------------------------------
/**
 * @brief Заполняет выпадающий список ComboBoxComPorts доступными в системе COM-портами.
 */
void __fastcall TForm1::PopulateComPorts()
{
    ComboBoxComPorts->Clear();
    std::vector<String> ports; // Временный вектор для сортировки
    TRegistry *Reg = new TRegistry;
    try
    {
        Reg->RootKey = HKEY_LOCAL_MACHINE;
        if (Reg->OpenKeyReadOnly("HARDWARE\\DEVICEMAP\\SERIALCOMM"))
        {
            TStringList *ValueNames = new TStringList;
            try
            {
                Reg->GetValueNames(ValueNames); // Получаем имена значений (например, \Device\Serial0)
                for (int i = 0; i < ValueNames->Count; i++)
                {
                    // Читаем значение, которое является именем COM-порта (например, COM1)
                    String PortName = Reg->ReadString(ValueNames->Strings[i]);
                    if (!PortName.IsEmpty())
                    {
                        ports.push_back(PortName);
                    }
                }
            }
            __finally
            {
                delete ValueNames;
            }
            Reg->CloseKey();
        }
    }
    __finally
    {
        delete Reg;
    }

    if (ports.empty())
    {
        ComboBoxComPorts->Items->Add("COM-порты не найдены");
        ComboBoxComPorts->ItemIndex = 0;
        ComboBoxComPorts->Enabled = false;
        // ButtonConnect->Enabled = false; // Если используете кнопку подключения
    }
    else
    {
        // Сортируем порты
        std::sort(ports.begin(), ports.end(), CompareComPorts);

        // Заполняем ComboBox отсортированными портами
        for (size_t i = 0; i < ports.size(); ++i)
        {
            ComboBoxComPorts->Items->Add(ports[i]);
        }
        ComboBoxComPorts->ItemIndex = 0; // Выбираем первый порт по умолчанию
        ComboBoxComPorts->Enabled = true;
        // ButtonConnect->Enabled = true;
        // ComboBoxComPortsChange(ComboBoxComPorts); // Обновить LabelSelectedPort
    }
}

void __fastcall TForm1::handleConnection()
{
    /*
    Panel1->Enabled = True;
    Panel2->Enabled = True;
    ToggleSwitch2->Enabled = True;
    Label7->Enabled = True;
    Timer1->Enabled = True;
    */
}

void __fastcall TForm1::handleDisconnection()
{
    /*
    Panel1->Enabled = False;
    Panel2->Enabled = False;
    ToggleSwitch2->Enabled = False;
    Label7->Enabled = False;
    Timer1->Enabled = False;
    */
}

//---------------------------------------------------------------------------
// INTERFACE
//---------------------------------------------------------------------------
TButton *MemoryButtons[10] = {};
TForm1::TStringGridCellInfo GetGridDetails(TStringGrid *AStringGrid)
{
    TForm1::TStringGridCellInfo info;
    info.TopLeftX = -1;
    info.TopLeftY = -1;
    info.CellHeight = -1;
    info.IsValid = false;

    // Проверяем, что указатель на компонент не NULL
    if (AStringGrid == nullptr)
    {
        return info; // Возвращаем невалидные данные
    }

    // Проверяем, что в Grid есть хотя бы одна строка и один столбец
    if (AStringGrid->ColCount == 0 || AStringGrid->RowCount == 0)
    {
        return info; // Возвращаем невалидные данные, если нет ячеек
    }

    // Получаем прямоугольник (TRect) для ячейки (0,0)
    // CellRect(0,0) возвращает координаты и размеры самой первой ячейки,
    // которая может быть фиксированной, если FixedCols > 0 или FixedRows > 0.
    TRect cellRect = AStringGrid->CellRect(0, 0);

    info.TopLeftX = cellRect.Left;
    info.TopLeftY = cellRect.Top;
    info.CellHeight = cellRect.Bottom - cellRect.Top; // Высота ячейки
    info.IsValid = true;

    return info;
}
/*
void handleOutMessage(AnsiString arg)
{
    Form1->Memo1->Lines->Add(arg);
}
inline void handleIncomeMessage(char* receivedHandleData)
{

    Form1->Memo2->Lines->Add(AnsiString(receivedHandleData));
}
*/
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent *Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
/**
 * @brief Обрабатывает нажатие на кнопку Button1 для отправки данных через COM-порт.
 */
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    /*AnsiString msg = AnsiString(Edit1->Text);
    if(strlen(msg.c_str())!=13)
    {
    if (MessageDlg("Длина строки, которую необходимо отправить, \n не равна 13. Все равно отправить?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo,0) != mrYes)
    {return;}
    }
    handleOutMessage(msg);
    TCP.Write(msg.c_str(),13);
    char* readBuffer = new char[13];
    TCP.Read(readBuffer, 13);
    handleIncomeMessage(readBuffer);
    //ShowMessage(UnicodeString(readBuffer));
    //TCP.Read( */
}
//---------------------------------------------------------------------------
/**
 * @brief Закрывает COM-порт при закрытии формы.
 */
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    UPS.Disconnect();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/**
 * @brief Обновляет текстовое поле Edit8 значением из ScrollBar1.
 */
void __fastcall TForm1::ScrollBar1Change(TObject *Sender)
{
    float value = (ScrollBar1->Max - ScrollBar1->Position) / 100.0;
    Edit8->Text = FloatToStrF(value, ffFixed, 6, 2);
    if (value < 100)
    {
        ScrollBar1->SmallChange = 1;
    }
    else
    {
        ScrollBar1->SmallChange = 10;
    }
}
//---------------------------------------------------------------------------
/**
 * @brief Обновляет текстовое поле Edit9 инвертированным значением из ScrollBar2.
 */
void __fastcall TForm1::ScrollBar2Change(TObject *Sender)
{
    float value = (ScrollBar2->Max - ScrollBar2->Position) / 100.0;
    Edit9->Text = FloatToStrF(value, ffFixed, 6, 2);
    if (value < 100)
    {
        ScrollBar2->SmallChange = 1;
    }
    else
    {
        ScrollBar2->SmallChange = 10;
    }
}
//---------------------------------------------------------------------------

/**
 * @brief Обновляет положение ScrollBar2 в соответствии со значением в Edit9.
 */

/**
 * @brief По таймеру добавляет случайную точку на график и управляет размером очереди данных.
 */
bool currentWasRequested = 0;
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    /*addPointToVoltageGraphic(rand()*1.0/1000);
    if(voltageData.size()>200)
    {
        voltageData.pop_front();
        voltageData.pop_front();
    }
    */
    if (timerStarted)

    {
        timerStarted = 0;
        return;
    } // иначе попытка чтения сразу после комманды открыть вход
    if (UPS.Port.Opened == 0)
        return;
    if (commandQueue.empty())
    {
        if (currentWasRequested)
        {
            float tmp = UPS.GetVoltage();
            if (tmp < 0)
                tmp = 0;
            voltageValue->Caption = FormatFloat(tmp);
            currentWasRequested = 0;
        }
        else
        {
            float tmp = UPS.GetCurrent();
            if (tmp < 0)
                tmp = 0;
            currentValue->Caption = FormatFloat(tmp);
            currentWasRequested = 1;
        }
    }
    else
    {
        command toDo = commandQueue.front();
        int success = 0;
        switch (toDo.index)
        {
        case 1:
            success = UPS.SetVoltage(toDo.value);
            break;
        case 3:
            success = UPS.SetCurrent(toDo.value);
            timerStarted = 1;
            break;
        case 7:
        case 8:
            success = UPS.SetOut(!(toDo.value == 0.0f));
            break;
        default:
            if (!commandQueue.empty())
                commandQueue.pop();
            break;
        }
        // Label5->Caption = success;
        if (success && (!commandQueue.empty()))
            commandQueue.pop();
    }
    Label5->Caption = bool(UPS.Mode);
    if (UPS.Mode == ConstantValue::VOLTAGE)
    {
        Label13->Visible = False;
        Label14->Visible = True;

        Label4->Font->Color = clBlack;
        Label3->Font->Color = clRed;
    }
    if (UPS.Mode == ConstantValue::CURRENT)
    {
        Label14->Visible = False;
        Label13->Visible = True;

        Label4->Font->Color = clRed;
        Label3->Font->Color = clBlack;
    }
}
//---------------------------------------------------------------------------

/**
 * @brief Инициализирует генератор случайных чисел и обновляет список COM-портов при создании формы.
 */
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    srand(time(NULL));
    ButtonRefreshPortsClick(this);
    ScrollBar1->Position = ScrollBar1->Max;
    ScrollBar2->Position = ScrollBar2->Max;
    TStringGridCellInfo tmpInf = GetGridDetails(StringGrid1);
    TButton *tmpButtonData = nullptr;
    for (int i = 0; i < 0; i++)
    {
        tmpButtonData = new TButton(this);
        MemoryButtons[i] = tmpButtonData;
        tmpButtonData->Top = tmpInf.TopLeftX + i * tmpInf.CellHeight;
        tmpButtonData->Top = tmpInf.TopLeftY;
    }
    // handleDisconnection();
    // Timer1->Enabled = True;
}
//---------------------------------------------------------------------------

/**
 * @brief Изменяет цвет шрифта переключателя ToggleSwitch1 в зависимости от его состояния.
 */
void __fastcall TForm1::ToggleSwitch1Click(TObject *Sender)
{
    // state can be tssOff or tssOn
    bool stateBefore = UPS.Port.Opened; // для отслеживания изменений
    std::string TempPortName = AnsiString(ComboBoxComPorts->Text).c_str();
    if (TempPortName.length() > 6)
    {
        if ((ToggleSwitch2->State == tssOn) && (stateBefore))
        {
            ToggleSwitch2->State = tssOff;
            ToggleSwitch2Click(this);
        }
        ToggleSwitch1->State = tssOff;
        Label8->Font->Color = clRed;
        return;
    }
    if (stateBefore)
    {
        if (UPS.SetLock(0) && UPS.Disconnect())
        {
            if (ToggleSwitch2->State == tssOn)
            {
                ToggleSwitch2->State = tssOff;
                ToggleSwitch2Click(this);
            }
            Label8->Font->Color = clRed;
            ToggleSwitch1->State = tssOff;
            handleDisconnection();
        }
        else
        {
            ShowMessage("Connection error. Force device disconection applied.");
            UPS.Port.Opened = false;
            UPS.LockState = State::DISABLED;
            if (ToggleSwitch2->State == tssOn)
            {
                ToggleSwitch2->State = tssOff;
                ToggleSwitch2->Color = clRed;
                Timer1->Enabled = False;
                Timer2->Enabled = False;
                timerStarted = 1;
                commandQueue = {};
            }
            Label8->Font->Color = clRed;
            ToggleSwitch1->State = tssOff;
            handleDisconnection();
        }
    }
    else
    {
        if (UPS.Connect(TempPortName))
        {
            Label8->Font->Color = clGreen;
            ToggleSwitch1->State = tssOn;
            handleConnection();
        }
        else
        {
            Label8->Font->Color = clRed;
            ToggleSwitch1->State = tssOff;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToggleSwitch2Click(TObject *Sender)
{
    // state can be tssOff or tssOn
    if (UPS.SetOut(ToggleSwitch2->State == tssOn) == 0)
        ToggleSwitch2->State = (ToggleSwitch2->State == tssOn) ? tssOff : tssOn;
    if (ToggleSwitch2->State == tssOn)
    {
        ToggleSwitch2->Color = clGreen;
        Timer1->Enabled = True;
        Timer2->Enabled = True;
    }
    else
    {
        ToggleSwitch2->Color = clRed;
        Timer1->Enabled = False;
        Timer2->Enabled = False;
        timerStarted = 1;
    }
}
//--------------------------------------------------------------------------

/**
 * @brief Инвертирует состояние переключателя ToggleSwitch1 при клике на метку Label8.
 */
void __fastcall TForm1::Label8Click(TObject *Sender)
{
    ToggleSwitch1Click(this);
}
//---------------------------------------------------------------------------
/**
 * @brief Обновляет метку LabelSelectedPort при выборе нового COM-порта в ComboBoxComPorts.
 */
void __fastcall TForm1::ComboBoxComPortsChange(TObject *Sender)
{
    if (ComboBoxComPorts->ItemIndex != -1 && ComboBoxComPorts->Items->Count > 0 && ComboBoxComPorts->Enabled)
    {
        LabelSelectedPort->Caption = "Выбран порт: " + ComboBoxComPorts->Text;
    }
    else
    {
        LabelSelectedPort->Caption = "Порт не выбран";
    }
}
/*---------------------------------------------------------------------------
void __fastcall TForm1::ButtonConnectClick(TObject *Sender)
{
    if (ComboBoxComPorts->ItemIndex != -1 && ComboBoxComPorts->Enabled)
    {
        String selectedPort = ComboBoxComPorts->Text;
        if (selectedPort == "COM-порты не найдены") {
             ShowMessage("Нет доступных COM-портов для подключения.");
             return;

        ShowMessage("Попытка подключения к: " + selectedPort);
    }
    else
    {
        ShowMessage("Пожалуйста, выберите COM-порт.");
    }
}
*/

//---------------------------------------------------------------------------

/**
 * @brief Обновляет список доступных COM-портов по нажатию на кнопку ButtonRefreshPorts.
 */
void __fastcall TForm1::ButtonRefreshPortsClick(TObject *Sender)
{
    PopulateComPorts(); // Обновляем список по нажатию кнопки
}
//---------------------------------------------------------------------------

float previousCurrent = 0;
float previousVoltage = 0;
void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
    float currentValue = -1;
    float voltageValue = -1;
    if (Edit8->Text != "")
    {
        try
        {
            currentValue = StrToFloat(Edit8->Text);
        }
        catch (EConvertError &e)
        {
            ShowMessage("Currrent field incorrect value. Error: " + e.Message);
            currentValue = -1; // Значение по умолчанию
        }
    }
    if (Edit9->Text != "")
    {
        try
        {
            voltageValue = StrToFloat(Edit9->Text);
        }
        catch (EConvertError &e)
        {
            ShowMessage("Voltage field incorrect value. Error: " + e.Message);
            voltageValue = -1; // Значение по умолчанию
        }
    }
    if (UPS.Port.Opened == 0)
    {
        commandQueue = {};
    }
    if ((abs(previousVoltage - voltageValue) > 0.0001) && (voltageValue >= 0))
    {
        /*command tmpComm;
        tmpComm.index = 1;
        tmpComm.value =*/
        commandQueue.push({1, voltageValue});
        previousVoltage = voltageValue;
        // ShowMessage("Push voltage");
    }
    if ((abs(previousCurrent - currentValue) > 0.0001) && (currentValue >= 0))
    {
        commandQueue.push({3, currentValue});
        previousCurrent = currentValue;
        // ShowMessage("Push current");
    }
    Label6->Caption = IntToStr(int(commandQueue.size()));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    logfile.log(std::string("Logging started"));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit9Exit(TObject *Sender)
{
    String tmp_data = Edit9->Text;
    if (tmp_data == "")
        return;
    ScrollBar2->Position = ScrollBar2->Max - StrToFloat(tmp_data) * 100;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit9KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Key == 13)
        Edit9Exit(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit8Exit(TObject *Sender)
{
    String tmp_data = Edit8->Text;
    if (tmp_data == "")
        return;
    ScrollBar1->Position = ScrollBar1->Max - StrToFloat(tmp_data) * 100;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit8KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Key == 13)
        Edit8Exit(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::StringGrid1KeyDown(TObject *Sender, WORD &Key,
                                           TShiftState Shift)
{
    // Получаем указатель на наш грид для удобства
    TStringGrid *grid = dynamic_cast<TStringGrid *>(Sender);
    if (!grid)
        return; // На всякий случай

    switch (Key)
    {
    case VK_LEFT: // Нажата стрелка ВЛЕВО
        // Если мы не в самом левом столбце, сдвигаемся влево
        if (grid->Col > 0)
        {
            grid->Col = grid->Col - 1;
        }
        break; // Выходим

    case VK_RIGHT: // Нажата стрелка ВПРАВО
        // Если мы не в самом правом столбце, сдвигаемся вправо
        if (grid->Col < grid->ColCount - 1)
        {
            grid->Col = grid->Col + 1;
        }
        break;

    case VK_UP: // Нажата стрелка ВВЕРХ
        // Если мы не в первой строке данных (учитывая заголовок)
        if (grid->Row > grid->FixedRows)
        {
            grid->Row = grid->Row - 1;
        }
        break;

    case VK_DOWN: // Нажата стрелка ВНИЗ
        // Если мы не в самой последней строке
        if (grid->Row < grid->RowCount - 1)
        {
            grid->Row = grid->Row + 1;
        }
        break;

    // Если нажата любая другая клавиша (буквы, цифры, Enter),
    // то мы ничего не делаем, и она обрабатывается как обычно.
    default:
        return;
    }

    // !!! САМЫЙ ВАЖНЫЙ ШАГ !!!
    // Обнуляем код клавиши. Это сигнал для VCL, что мы полностью
    // обработали это нажатие, и стандартный обработчик грида
    // вызывать не нужно. Именно это и предотвращает прокрутку.
    Key = 0;
}
//---------------------------------------------------------------------------
