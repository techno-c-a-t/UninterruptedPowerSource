
//---------------------------------------------------------------------------
#include <vcl.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "UPSource.h"
#pragma hdrstop

#include <cstring>   // Для strlen
#include <stdexcept> // Для исключений
#include <random>
#include <ctime>
#include <deque>
#include <Registry.hpp>        // Для работы с реестром
#include <vector>              // Для временного хранения и сортировки
#include <algorithm>           // Для std::sort
#include <System.SysUtils.hpp> // Для StrToIntDef
#include <string>
#include <sstream>
#include <iomanip>
#include "src/core/engine.h"
#include "src/utils/time_manager.h"
#include "src/components/MemTable.h"
#include "PowerSourceObject.h"
#include "DataProcessing.cpp"



#define MAX_VOLTAGE 300.0f
#define MAX_CURRENT 1.0f
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



PSEngine engine(MAX_VOLTAGE, MAX_CURRENT);
TForm1 *Form1;
std::deque<float> voltageData = {}; // resize(n, value); push_back(val); pop_front();
long emulatedTime = 100; // эмуляция времени
std::vector<storage::CVMemPair> CVMemoryArray;

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
void __fastcall setConnectionSwitchOn() {
     Form1->disableHandlers = true;
//     Form1->Label8->Font->Color = clGreen;
     Form1->ToggleSwitch1->Color = clGreen;
     Form1->ToggleSwitch1->State = tssOn;
     Form1->disableHandlers = false;
}

void __fastcall setConnectionSwitchOff() {
     Form1->disableHandlers = true;
     Form1->ToggleSwitch1->State = tssOff;
//     Form1->Label8->Font->Color = clRed;
     Form1->ToggleSwitch1->Color = clGray;
     Form1->disableHandlers = false;
}

void __fastcall setOutSwitchOn() {
     Form1->disableHandlers = true;
     Form1->ToggleSwitch2->State = tssOn;
	 Form1->ToggleSwitch2->Color = clLime;
     Form1->disableHandlers = false;
}

void __fastcall setOutSwitchOff () {
     Form1->disableHandlers = true;
	 Form1->ToggleSwitch2->State = tssOff;
	 Form1->ToggleSwitch2->Color = clGray;
	 Form1->disableHandlers =false;
}
//-----------------------------------------------


//---------------------------------------------------------------------------

std::string formatFloat(float value, int totalWidth, int precision) {
    std::stringstream ss;

    ss << std::fixed                 // Использовать фиксированную запятую (не экспоненциальную форму)
       << std::setprecision(precision) // Сколько знаков ПОСЛЕ запятой
       << std::setw(totalWidth)      // Минимальная ОБЩАЯ ширина строки
       << value;

    return ss.str();
}
float previousCurrent = 0;
float previousVoltage = 0;
void __fastcall TForm1::requestVAUpdate(bool forceUpdate = false)
{
    float c_val = -1;
    float v_val = -1;
    if (Edit8->Text != "")
    {
        try
        {

            c_val = StrToFloat(StringReplace(Edit8->Text, ".", ",", TReplaceFlags() << rfReplaceAll));
        }
        catch (EConvertError &e)
        {
            c_val = -1; // Значение по умолчанию
        }
    }
    if (Edit9->Text != "")
    {
        try
        {
            v_val = StrToFloat(StringReplace(Edit9->Text, ".", ",", TReplaceFlags() << rfReplaceAll));
        }
        catch (EConvertError &e)
        {
            v_val = -1; // Значение по умолчанию
        }
    }
    if (((abs(previousVoltage - v_val) > 0.0001) || forceUpdate) && (v_val >= 0))
    {
        /*command tmpComm;
        tmpComm.index = 1;
        tmpComm.value =*/
        engine.SetTargetVoltage(v_val);
        previousVoltage = v_val;
        // ShowMessage("Push voltage");
    }
    if (((abs(previousCurrent - c_val) > 0.0001) || forceUpdate) && (c_val >= 0))
    {
        engine.SetTargetCurrent(c_val);
        previousCurrent = c_val;
        // ShowMessage("Push current");
    }
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
    storage::SavePresetsToDisk("AutomaticallySavedData.ini", CVMemoryArray);
    engine.LockDisconnectSync();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/**
 * @brief Обновляет текстовое поле Edit8 значением из ScrollBar1.
 */
void __fastcall TForm1::ScrollBar1Change(TObject *Sender)
{
    float value = (/*ScrollBar1->Max - */ScrollBar1->Position) / 100.0;
    Edit8->Text = formatValue_c(value, 4, 2);
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
    float value = (/*ScrollBar2->Max - */ScrollBar2->Position) / 100.0;
    Edit9->Text = formatValue_c(value, 4, 2);
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
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    /*addPointToVoltageGraphic(rand()*1.0/1000);
    if(voltageData.size()>200)
    {
        voltageData.pop_front();
        voltageData.pop_front();
    }
//    */
//    if (timerStarted)
//
//    {
//        timerStarted = 0;
//        return;
//    } // иначе попытка чтения сразу после комманды открыть вход
    if (engine.IsVoltageStab() && engine.GetOutState())
    {
//        Label13->Visible = False;
//        Label14->Visible = True;

        Label4->Font->Color = clBlack;
        Label3->Font->Color = clRed;
    }
    else if (engine.GetOutState())
    {
//        Label14->Visible = False;
//        Label13->Visible = True;

        Label4->Font->Color = clRed;
        Label3->Font->Color = clBlack;
    }  else {
//       Label14->Visible = False;
//        Label13->Visible = False;

        Label4->Font->Color = clBlack;
        Label3->Font->Color = clBlack;
    }
    if (engine.GetOutState() != (ToggleSwitch2->State == tssOn)) {
           engine.GetOutState() ? setOutSwitchOn() : setOutSwitchOff();
    }
    if (engine.Connected()) {
       requestVAUpdate();
    }
    engine.Tick();
    float cached_v = engine.GetLastVoltage();
    float cached_c = engine.GetLastCurrent();
    voltageValue->Caption = formatValue_c(cached_v, 4, 2);
    currentValue->Caption = formatValue_c(cached_c, 4, 2);
    float power =  cached_v * cached_c;
    powerValue->Caption = formatValue_c(power, 4, 2);

    Time->Caption = TimeNowString().c_str();
}
//---------------------------------------------------------------------------

/**
 * @brief Инициализирует генератор случайных чисел и обновляет список COM-портов при создании формы.
 */
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    srand(time(NULL));
    ButtonRefreshPortsClick(this);
    ScrollBar1->Position = /*ScrollBar1->Max*/ 0;
    ScrollBar2->Position = /*ScrollBar2->Max*/ 0;
//    TStringGridCellInfo tmpInf = GetGridDetails(StringGrid1);
//    TButton *tmpButtonData = nullptr;
//    for (int i = 0; i < 0; i++)
//    {
//        tmpButtonData = new TButton(this);
//        MemoryButtons[i] = tmpButtonData;
//        tmpButtonData->Top = tmpInf.TopLeftX + i * tmpInf.CellHeight;
//        tmpButtonData->Top = tmpInf.TopLeftY;
//    }


    // 3. Прописываем заголовки (Row 0)
    memoryGrid->Cells[0][0] = "№";
	memoryGrid->Cells[1][0] = "V";
	memoryGrid->Cells[2][0] = "A";

    // Можно задать ширину колонок кодом, чтобы было красиво
    memoryGrid->ColWidths[0] = 40;  // Узкая для номера
    memoryGrid->ColWidths[1] = 80;  // Для вольт
    memoryGrid->ColWidths[2] = 80;  // Для ампер
      CVMemoryArray.resize(10);

    String fileName = "ManuallySavedData.ini";

    if (FileExists(fileName)) {
       storage::LoadPresetsFromDisk("ManuallySavedData.ini", CVMemoryArray); // Загрузили файл в память
    } else {
        storage::LoadPresetsFromDisk("AutomaticallySavedData.ini", CVMemoryArray); // Загрузили файл в память
    }


    VectorToGrid(CVMemoryArray, memoryGrid, 0, 10);         // Отобразили на экране

}
//---------------------------------------------------------------------------

/**
 * @brief Изменяет цвет шрифта переключателя ToggleSwitch1 в зависимости от его состояния.
 */
void __fastcall TForm1::ToggleSwitch1Click(TObject *Sender)
{
    // state can be tssOff or tssOn
    if (disableHandlers) {
       return;
    }
    std::string PortName = AnsiString(ComboBoxComPorts->Text).c_str();
    if (ToggleSwitch1->State == tssOn) {
         setConnectionSwitchOff();
         if (engine.LockConnectSync(PortName) == false) {
            engine_logic::debug("LockConnectSync returned false");
             return;
         }
	     setConnectionSwitchOn();
         setOutSwitchOff();
         requestVAUpdate(true);
         engine.SetOutput(false);
         Sleep(100); // ??Поможет?? Не помогло
    } else {
         setConnectionSwitchOn();
         engine.SetOutput(false);
         setOutSwitchOff();
         bool success =  engine.LockDisconnectSync();
         if (!success) {
            ShowMessage("Cant disconnect. Probably, connection was lost. This may cause undefined behaviour");
         }
         setConnectionSwitchOff();
         }
    }

//---------------------------------------------------------------------------
void __fastcall TForm1::ToggleSwitch2Click(TObject *Sender)
{
 if (disableHandlers) {
       return;
    }
if (!engine.Connected()) {
           setOutSwitchOff();
           engine_logic::debug("ENgine not connected");
           return;
}
bool now_enabled = ToggleSwitch2->State == tssOn ;
engine_logic::trace("Now out switch is " + std::string(now_enabled? "on" : "off"));
 engine.SetOutput(now_enabled);
 engine_logic::trace(
                     std::string("Output has been set in state ") +
                     std::string(now_enabled ? "on" : "off")
                     );
 now_enabled? setOutSwitchOff() : setOutSwitchOn();  // АПДЕЙТ В ТАЙМЕРЕ !!!
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
//void __fastcall TForm1::ComboBoxComPortsChange(TObject *Sender)
//{
//    if (ComboBoxComPorts->ItemIndex != -1 && ComboBoxComPorts->Items->Count > 0 && ComboBoxComPorts->Enabled)
//    {
//        LabelSelectedPort->Caption = "Выбран порт: " + ComboBoxComPorts->Text;
//    }
//    else
//    {
//        LabelSelectedPort->Caption = "Порт не выбран";
//    }
//}

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

void __fastcall TForm1::Edit9Exit(TObject *Sender)
{
    Edit9->Text = StringReplace(Edit9->Text, ".", ",", TReplaceFlags() << rfReplaceAll);
    String tmp_data =  Edit9->Text;
    if (tmp_data == "")
        return;
        try {
            ScrollBar2->Position = /*ScrollBar2->Max - */StrToFloat(tmp_data) * 100;
            Edit9->Text = formatValue_c(ScrollBar2->Position / 100.0f, 4, 2);
        } catch (...) {
          ShowMessage("Incorrect  voltage value");
        }
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
    Edit8->Text = StringReplace(Edit8->Text, ".", ",", TReplaceFlags() << rfReplaceAll);
    String tmp_data = Edit8->Text;
    if (tmp_data == "")
        return;
    try {
               ScrollBar1->Position = /*ScrollBar1->Max - */ StrToFloat(tmp_data) * 100;
               Edit8->Text = formatValue_c(ScrollBar1->Position / 100.0f, 4, 2);
    } catch (...) {
      ShowMessage("Invalid current value");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit8KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Key == 13)
        Edit8Exit(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::memoryGridKeyDown(TObject *Sender, WORD &Key,
                                           TShiftState Shift)
{
    // ffffuuuck????
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


void __fastcall TForm1::Button2Click(TObject *Sender)
{
     engine.flush_errors();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
 engine.DropState();
}
//---------------------------------------------------------------------------




void __fastcall TForm1::SaveMemGridClick(TObject *Sender)
{
storage::SavePresetsToDisk("ManuallySavedData.ini", CVMemoryArray);
}
//---------------------------------------------------------------------------

