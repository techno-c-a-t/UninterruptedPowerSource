
//---------------------------------------------------------------------------
#include <vcl.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "UPSource.h"
#pragma hdrstop

#include <cstring>   // Для strlen
#include <stdexcept> // Для исключений
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
#include "src/components/EnergyLabel.h"
#include "src/components/Models.h"
#include <cmath>  // Нужно для std::fmod (замена Delphi'евскому frac)


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
std::vector<storage::CVMemPair> CVMemoryArray;
EnergyLabelUpdater Energy;
CapacityLabelUpdater Capacity;

size_t emulatedTime = 100;


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

void __fastcall ChangeGroupState(TGroupBox* Group, bool State) {
	Group->Enabled = State;
    Group->Font->Color = State ? clBlack : clSilver;
    for (int i = 0; i < Group->ControlCount; i++) {
        Group->Controls[i]->Enabled = State;
    }
}

//---------------------------------------------------------------------------

void __fastcall setConnectionSwitchOn() {
	 Form1->disableHandlers = true;
     Form1->ToggleSwitch1->Color = clGreen;
     Form1->ToggleSwitch1->State = tssOn;
	 Form1->disableHandlers = false;
	 ChangeGroupState(Form1->gbCommPortSelect, false);
	 ChangeGroupState(Form1->gbDeviceSelect, false);
}

void __fastcall setConnectionSwitchOff() {
     Form1->disableHandlers = true;
	 Form1->ToggleSwitch1->State = tssOff;
     Form1->ToggleSwitch1->Color = clGray;
	 Form1->disableHandlers = false;
	 ChangeGroupState(Form1->gbCommPortSelect, true);
	 ChangeGroupState(Form1->gbDeviceSelect, true);
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

void __fastcall handleOutConnection () {
	Form1->CycleTimeLabelTimer.start();
    Energy.CleanValue();
	Capacity.CleanValue();
	Form1->sessionManager.StartSession();

}

void __fastcall handleOutDisconnection () {
	Form1->CycleTimeLabelTimer.stop();
	Form1->sessionManager.StopSession();

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
 * @brief Закрывает COM-порт при закрытии формы.
 */
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	engine.StopLoop();
    storage::SavePresetsToDisk("AutomaticallySavedData.ini", CVMemoryArray);
    engine.LockDisconnectSync();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/**
 * @brief Обновляет текстовое поле Edit8 значением из ScrollBar1.
 */
// Current
void __fastcall TForm1::ScrollBar1Change(TObject *Sender)
{
	float min = ScrollBar1->Min, max = ScrollBar1->Max, pos = ScrollBar1->Position;
	float norm_value = (pos - min)/(max - min);
	float value = pow(norm_value, CCurve);
	float final_value = (min + value * (max - min)) / 1000;
	Edit8->Text = formatValue_c(final_value, 4, 3);
	if (final_value < 1000)
    {
		ScrollBar1->SmallChange = 10;
	}
    else
    {
        ScrollBar1->SmallChange = 100;
    }
}
//---------------------------------------------------------------------------
/**
 * @brief Обновляет текстовое поле Edit9 инвертированным значением из ScrollBar2.
 */
 // Voltage
void __fastcall TForm1::ScrollBar2Change(TObject *Sender)
{
	float min = ScrollBar2->Min, max = ScrollBar2->Max, pos = ScrollBar2->Position;
	float norm_value = (pos - min)/(max - min);
	float value = pow(norm_value, VCurve);
	float final_value = (min + value * (max - min)) / 1000;
	Edit9->Text = formatValue_c(final_value, 4, 2);
	if (final_value < 1000)
	{
		ScrollBar2->SmallChange = 10;
	}
	else
	{
		ScrollBar2->SmallChange = 100;
	}
}
//---------------------------------------------------------------------------

/**
 * @brief Обновляет положение ScrollBar2 в соответствии со значением в Edit9.
 */


//---------------------------------------------------------------------------




void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	if (engine.IsVoltageStab() && engine.GetOutState())
	{
		Label4->Font->Color = clBlack;
		Label3->Font->Color = clRed;
	}

	else if (engine.GetOutState())
	{
		Label4->Font->Color = clRed;
		Label3->Font->Color = clBlack;
	}

	  else {
		Label4->Font->Color = clBlack;
		Label3->Font->Color = clBlack;
	}

	if (engine.GetOutState() != (ToggleSwitch2->State == tssOn)) {
		if (engine.GetOutState()) {
			 setOutSwitchOn();
			 handleOutConnection();

		} else {
			setOutSwitchOff();
			handleOutDisconnection();

		}
	}

	if (engine.Connected()) { // Если связь с устройством установлена (engine.Connected())
	   requestVAUpdate();	  //каждый тик таймера считываются текущие значения тока и напряжения для вывода на экран.
	}
    //---------------------------------------------------------------------------


	float cached_v = engine.GetLastVoltage();
	float cached_c = engine.GetLastCurrent();
	voltageValue->Caption = formatValue_c(cached_v, 4, 2);
	currentValue->Caption = formatValue_c(cached_c, 4, 3);
    float power =  cached_v * cached_c;
	powerValue->Caption = formatValue_c(power, 4, 2);
	Energy.Update(cached_v, cached_c);
	Capacity.Update(cached_v, cached_c);

	updateSessionCharts();  // перерисовка графиков



	//---------------------------------------------------------------------------
	auto now_vector = TimeNowVector(); // взяли массив текущ времени
	now_vector.pop_back();  //    убрали милисек
	Time->Caption = TimeNowString(now_vector).c_str();  //вывод текущего времени
    // Вывод текущей даты в формате DD.MM.GG (две цифры года)
	Date->Caption = FormatDateTime("dd.mm.yy", TDateTime::CurrentDate());
	UpdateCycleTimeLabel();   //   обновление времени сеанса
}
//---------------------------------------------------------------------------

/*
 * @brief Инициализирует генератор случайных чисел и обновляет список COM-портов при создании формы.
 */
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	// ------------------------------------------------------------------------
	// 1. ИНИЦИАЛИЗАЦИЯ И НАСТРОЙКА UI-КОМПОНЕНТОВ
	// ------------------------------------------------------------------------

	// Заголовки и ширина колонок таблицы предустановок
	memoryGrid->Cells[0][0] = "№";
	memoryGrid->Cells[1][0] = "V";
	memoryGrid->Cells[2][0] = "A";

	memoryGrid->ColWidths[0] = 40;  // Номер
	memoryGrid->ColWidths[1] = 80;  // Вольты
	memoryGrid->ColWidths[2] = 80;  // Амперы

	// Привязка UI-меток к структурам/классам расчета (без использования Form1->)
	Energy.label   = energyLabel;
	Capacity.label = batteryCapacity;

	// 1) Исключаем элемент seFont из стилей Windows, чтобы разблокировать Font->Color
   //	CheckBoxVoltage->StyleElements = CheckBoxVoltage->StyleElements >> seFont;
  //	CheckBoxCurrent->StyleElements = CheckBoxCurrent->StyleElements >> seFont;

	// 2) Цвет текста CheckBox (Примечание: при включенных Runtime Themes
	// системный стиль Windows переопределяет Font->Color для TCheckBox) НЕ СРАБОТАЛО!!!
  //	CheckBoxVoltage->Font->Color = clNavy;
  //	CheckBoxCurrent->Font->Color = clMaroon;

	// ------------------------------------------------------------------------
	// 2. ИНИЦИАЛИЗАЦИЯ МЕНЕДЖЕРОВ И СТРУКТУР ДАННЫХ
	// ------------------------------------------------------------------------

	// Заготавливает память для таблицы предустановок
	CVMemoryArray.resize(10);

// Выбор и загрузка пресетов памяти
	const String primaryFile   = "ManuallySavedData.ini";
    const String fallbackFile  = "AutomaticallySavedData.ini";
	const String activeFile    = FileExists(primaryFile) ? primaryFile : fallbackFile;

	storage::LoadPresetsFromDisk(activeFile, CVMemoryArray);
	VectorToGrid(CVMemoryArray, memoryGrid, 0, 10);

 // Инициализация графиков и менеджера сессий
	chartManager.Init(Chart1);
	engine.SetSessionCore(&sessionManager); //инициализация массива для логфайла

	// ------------------------------------------------------------------------
	// 3. ЗАГРУЗКА И ПРИМЕНЕНИЕ НАСТРОЕК
	// ------------------------------------------------------------------------

	// Загрузка доступных моделей устройств в комбобокс
	storage::LoadModels(ComboBoxDevice, "Models.ini");
	// передаем в движок параметры выбранного устройства
	storage::SetModel(engine, ComboBoxDevice->Text, "Models.ini");

	// Заполняем выпадающий список ComboBoxComPorts доступными в системе COM-портами
	 PopulateComPorts();





	ComboBoxDeviceChange(this);

   //	discret->Text = "1,0";
	discretExit(this);     //похоже добавка после обновления обработки ввода данных







}
void __fastcall TForm1::FormShow(TObject *Sender)
{
	// Запускаем основной цикл/опрос только тогда, когда окно полностью показано на экране
	static bool isFirstShow = true;
	if (isFirstShow) {
		engine.StartLoop();
		isFirstShow = false;
	}
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
    } else {
         setOutSwitchOff();
         bool success = engine.LockDisconnectSync();
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
 now_enabled ? setOutSwitchOff() : setOutSwitchOn();

 	// Блокируем или разрешаем зум и скролл графика
    if (now_enabled) {
        Chart1->AllowPanning = (TPanningMode)0; // pmNone (полный запрет сдвига)
        Chart1->Zoom->Active = false;          // Отключаем зум
    } else {
        Chart1->AllowPanning = (TPanningMode)1; // pmHorizontal (разрешен сдвиг по X)
        Chart1->Zoom->Active = true;           // Включаем зум обратно
    }
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


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
				float min = ScrollBar2->Min, max = ScrollBar2->Max;
				float final_value = StrToFloat(tmp_data);
				// 1. Убираем деление на 1000
				float value_scaled = final_value * 1000;

				// 2. Обратное линейное преобразование (находим value после pow)
				float value_after_pow = (value_scaled - min) / (max - min);

				// 3. Обратная степень (извлекаем корень степени VCurve)
				float norm_value = pow(value_after_pow, 1.0f / VCurve);

				// 4. Находим исходный pos
				int pos = norm_value * (max - min) + min;
				ScrollBar2->Position =  pos;
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
//			   Text = formatValue_c(ScrollBar1->Position / 100.0f, 4, 2);
				float min = ScrollBar1->Min, max = ScrollBar1->Max;
				float final_value = StrToFloat(tmp_data);
				// 1. Убираем деление на 1000
				float value_scaled = final_value * 1000;

				// 2. Обратное линейное преобразование (находим value после pow)
				float value_after_pow = (value_scaled - min) / (max - min);

				// 3. Обратная степень (извлекаем корень степени лVCurve)
				float norm_value = pow(value_after_pow, 1.0f / VCurve);

				// 4. Находим исходный pos
				int pos = norm_value * (max - min) + min;
				ScrollBar1->Position =  pos;

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



// Сброс времени цикла
void __fastcall TForm1::UpdateCycleTimeLabel()
{
	if (CycleTimeLabelTimer.check()) {
		LastCycleDurationValue = CycleTimeLabelTimer.TimeToVector();
		LastCycleDurationValue.pop_back();
	}
    CycleTimeLabel->Caption = TimeNowString(LastCycleDurationValue).c_str();
	return ;


}



void __fastcall TForm1::ComboBoxDeviceChange(TObject *Sender)
{
    // Проверяем, что в комбобоксе что-то выбрано
	if (ComboBoxDevice->ItemIndex != -1)
	{
		storage::SetModel(engine, ComboBoxDevice->Text, "Models.ini");

	}

}

//---------------------------------------------------------------------------

void __fastcall TForm1::updateSessionCharts()
{
	if (!CycleTimeLabelTimer.check()) {
        return;
	}
	const std::vector<SessionPoint> raw_data = sessionManager.GetData();
	std::vector<SessionPoint> chart_data{};
	const unsigned frequency = static_cast<unsigned>(StrToFloatDef(discret->Text, 0.0f) * 10.0f);
	if(raw_data.size() < frequency || frequency == 0) {
		return;
	}
	chart_data.reserve(raw_data.size() / frequency);
	auto chart_interator = raw_data.begin();
	chart_data.push_back(*chart_interator);
	std::advance(chart_interator, frequency - 1);
	chart_data.push_back(*chart_interator);
	for(int i = 1; i < raw_data.size() / frequency; i++) {
		std::advance(chart_interator, frequency);
		chart_data.push_back(*chart_interator);
	}
    chartManager.Update(chart_data);
}
//---------------------------------------------------------------------------

 //---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxVoltageClick(TObject *Sender)
{
	TCheckBox *cb = dynamic_cast<TCheckBox*>(Sender);
	if (cb) {
		chartManager.SetVoltageVisible(cb->Checked);
		updateSessionCharts();
	}
}

void __fastcall TForm1::CheckBoxCurrentClick(TObject *Sender)
{
	TCheckBox *cb = dynamic_cast<TCheckBox*>(Sender);
	if (cb) {
		chartManager.SetCurrentVisible(cb->Checked);
		updateSessionCharts();
	}
}

void __fastcall TForm1::ButtonResetZoomClick(TObject *Sender)
{
	chartManager.ResetZoom();
	updateSessionCharts();
}

//---------------------------------------------------------------------------
//  Валидатор для поля discret (TEdit) в C++ Builder. Используется концепция
// White-list (симуляция итогового состояния) вместо прямого фильтрования символов.
//  Допустимые диапазоны:
//Дробный: 0,1 ... 0,9 (строго 1 знак после запятой).
//Целочисленный: 1 ... 3600 (без лидирующих нулей вроде 010).
//  Промежуточные состояния:
//Строки "0" и "0," разрешены исключительно в процессе набора с клавиатуры.
//
// ============================================================================
// 1. ВВОД С КЛАВИАТУРЫ (KeyPress) — Симуляция и White-list
// discretKeyPress: Формирует futureText (с учетом выделения SelLength и
// курсора SelStart). Если результат не проходит по «белому списку» — нажатие
// блокируется (Key = 0).
// ============================================================================
void __fastcall TForm1::discretKeyPress(TObject *Sender, System::WideChar &Key)
{
    // 1. Блокировка ввода при включенном ToggleSwitch2
    if (ToggleSwitch2->State == TToggleSwitchState::tssOn) {
        Key = 0;
        return;
    }

    // 2. Системные клавиши (Backspace, Enter) пропускаем
    if (Key == VK_BACK || Key == VK_RETURN) return;

    // Автоматическая замена точки на запятую
    if (Key == '.') Key = ',';

    // Разрешаем только цифры и запятую
    if ((Key < '0' || Key > '9') && Key != ',') {
        Key = 0;
        return;
    }

    // 3. СИМУЛЯЦИЯ: Строим futureText (строка, которая получится ПОСЛЕ нажатия)
    String currentText = discret->Text;
    int selStart = discret->SelStart;   // 0-based индекс курсора
    int selLength = discret->SelLength; // Длина выделения

    String futureText = currentText;
    if (selLength > 0) {
        futureText.Delete(selStart + 1, selLength); // В UnicodeString индексы с 1
    }
    futureText.Insert(String(Key), selStart + 1);
    futureText = futureText.Trim();

    // 4. БЕЛЫЙ СПИСОК (Проверяем только допустимые шаблоны)

    // А) Промежуточные состояния
    if (futureText == "0" || futureText == "0,") {
        return; // Разрешено
    }

    // Б) Дробный диапазон: строго "0,1" ... "0,9"
    if (futureText.Length() == 3 && futureText.SubString(1, 2) == "0,") {
        System::WideChar decDigit = futureText[3];
        if (decDigit >= '1' && decDigit <= '9') {
            return; // Разрешено
        }
    }

    // В) Запрет лидирующих нулей ("010", "05")
    if (futureText.SubString(1, 1) == "0" && futureText.Length() > 1) {
        Key = 0; // Блокируем
        return;
    }

    // Г) Целочисленный диапазон: [1 ... 3600]
    int intVal = StrToIntDef(futureText, -1);
    if (intVal >= 1 && intVal <= 3600) {
        return; // Разрешено
    }

    // Всё остальное блокируем
    Key = 0;
}

// ============================================================================
// 2. ВЫХОД ИЗ ПОЛЯ (Exit) — Автокоррекция и Форматирование
// discretExit: Обрабатывает вставку через буфер (Ctrl+V), незавершенный
//ввод ("0,") и выполняет автокоррекцию/откат к lastValidDiscret.
// ============================================================================
void __fastcall TForm1::discretExit(TObject *Sender)
{
    String text = discret->Text.Trim();

    TFormatSettings fs = TFormatSettings::Create();
    fs.DecimalSeparator = ',';

    // Заменяем точку на запятую (если вставили из буфера)
    text = StringReplace(text, ".", ",", TReplaceFlags() << rfReplaceAll);

    float val = StrToFloatDef(text, -1.0f, fs);

    // Проверяем, является ли число целым через C++ std::fmod
    bool isInteger = (std::fmod(val, 1.0f) == 0.0f);

    bool isValidFloat = (val >= 0.1f && val < 1.0f);
    bool isValidInt   = (val >= 1.0f && val <= 3600.0f) && isInteger;

    // Если значение некорректно (пусто, "0,", вне диапазонов)
    if (!isValidFloat && !isValidInt) {
        // Восстанавливаем из float-переменной lastValidDiscret
        if (lastValidDiscret < 1.0f) {
            discret->Text = FormatFloat("0.0", lastValidDiscret, fs);
        } else {
            discret->Text = IntToStr((int)lastValidDiscret);
        }
        return;
    }

    // Приведение к каноническому виду
    if (val < 1.0f) {
        discret->Text = FormatFloat("0.0", val, fs); // "0,1" ... "0,9"
    } else {
        discret->Text = IntToStr((int)val);          // "1" ... "3600"
    }

    // Запоминаем новое успешное float-значение
    lastValidDiscret = val;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaweSeansClick(TObject *Sender)
{
	if (sessionManager.GetPointCount() == 0) {
		ShowMessage("Нет записанных данных сеанса для сохранения.");
		return;
	}

	sdSaveSeans->Filter = "Лог файлы (*.log)|*.log|Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*";
	sdSaveSeans->DefaultExt = "log";
	sdSaveSeans->Title = "Сохранение лога сеанса";

	if (sdSaveSeans->Execute()) {
		float totalEnergyWh = Energy.value / 3600.0f;
		float totalCapacityAh = Capacity.value / 3600.0f;

		bool success = sessionManager.SaveToFile(sdSaveSeans->FileName, totalEnergyWh, totalCapacityAh);
		if (success) {
			ShowMessage("Лог сеанса успешно сохранён!");
		} else {
            ShowMessage("Ошибка при сохранении лог-файла.");
        }
    }
}

//----------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TForm1::Chart1Scroll(TObject *Sender)
{
	// Если переключатель в состоянии tssOn, прерываем обработку
    if (ToggleSwitch2->State == tssOn) {
        return;
	}

	// 1. Проверяем, ушел ли левый край оси X левее 0
	if (Chart1->BottomAxis->Minimum < 0)
	{
		// Вычисляем текущую видимую ширину графика (разницу между правым и левым краем)
		double currentRange = Chart1->BottomAxis->Maximum - Chart1->BottomAxis->Minimum;

		// Фиксируем левый край на 0, а правый сдвигаем так, чтобы ширина окна просмотра не сжалась
		Chart1->BottomAxis->SetMinMax(0.0, currentRange);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Chart1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled)
{
// Вызываем нашу функцию проверки границы
	Chart1Scroll(Sender);
}
//---------------------------------------------------------------------------

