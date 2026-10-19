//---------------------------------------------------------------------------

#ifndef UPSourceH
#define UPSourceH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include "src/components/MemTable.h"
#include "src/utils/time_manager.h"
#include "src/components/SessionManager.h"
#include "src/components/ChartManager.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TEdit *Edit8;
	TScrollBar *ScrollBar1;
    TPanel *Panel2;
    TEdit *Edit9;
	TScrollBar *ScrollBar2;
    TChart *Chart1;
    TLineSeries *Series1;
    TTimer *Timer1;
    TPanel *Panel3;
	TLabel *currentValue;
	TLabel *Label4;
    TPanel *Panel4;
    TLabel *Label3;
    TLabel *voltageValue;
    TToggleSwitch *ToggleSwitch2;
    TButton *Button2;
    TLabel *Label2;
    TLabel *Label1;
	TStringGrid *memoryGrid;       //таблица предустановленных режимов
    TStringGrid *StringGrid2;
    TButton *Button4;
    TLabel *powerValue;
	TLabel *W;
    TLabel *Time;
	TPanel *pnlStattic;
	TPanel *pnlCalkParam;
	TPanel *Power;
	TPanel *Panel10;
	TButton *SaveMemGrid;
	TLabel *batteryCapacity;
	TLabel *Label9;
	TPanel *pnlComm;
	TGroupBox *gbConnection;
	TToggleSwitch *ToggleSwitch1;
	TGroupBox *gbDeviceSelect;
	TGroupBox *gbCommPortSelect;
	TComboBox *ComboBoxComPorts;
    TComboBox *ComboBoxDevice;
	TButton *ButtonRefreshPorts;
	TPanel *Panel5;
	TLabel *energyLabel;
	TLabel *Wh;
	TLabel *CycleTimeLabel;
	TEdit *discret;
	TButton *SaweSeans;
	TSaveDialog *sdSaveSeans;
	TCheckBox *CheckBoxCurrent;
	TCheckBox *CheckBoxVoltage;
	TButton *ButtonResetZoom;
	TLineSeries *Series2;
	TPanel *Panel6;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Date;

    // NOT INTERFACE

    void __fastcall PopulateComPorts();
	void __fastcall requestVAUpdate(bool forceUpdate);
	void __fastcall ApplyPreset(size_t row);
    //INTERFACE
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ScrollBar1Change(TObject *Sender);
    void __fastcall ScrollBar2Change(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);      // <-- Добавил
	void __fastcall ToggleSwitch1Click(TObject *Sender);
	void __fastcall ToggleSwitch2Click(TObject *Sender);
    void __fastcall ButtonRefreshPortsClick(TObject *Sender);
//    void __fastcall ComboBoxComPortsChange(TObject *Sender);
    void __fastcall Edit9Exit(TObject *Sender);
    void __fastcall Edit9KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall Edit8Exit(TObject *Sender);
    void __fastcall Edit8KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall memoryGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
	void __fastcall memoryGridMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
    void __fastcall SaveMemGridClick(TObject *Sender);
	void __fastcall ComboBoxDeviceChange(TObject *Sender);
	void __fastcall SaweSeansClick(TObject *Sender);
	void __fastcall discretKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall discretExit(TObject *Sender);
	void __fastcall updateSessionCharts();
	void __fastcall CheckBoxVoltageClick(TObject *Sender);
	void __fastcall CheckBoxCurrentClick(TObject *Sender);
	void __fastcall ButtonResetZoomClick(TObject *Sender);
	void __fastcall Chart1Scroll(TObject *Sender);
	void __fastcall Chart1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
public:
	SessionManager sessionManager;
	ChartManager chartManager;




private:	// User declarations

	float lastValidDiscret = 1.0f; // Переменная типа float для хранения
									// валидного значения дискретности обновления графика


public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

			/**
 * Форматирует float в строку с жестким контролем лимитов.
 * @param value - само число
 * @param maxTotalDigits - сколько всего цифр (значимых) может быть в строке
 * @param maxDecimals - потолок знаков после запятой
 */

	float VCurve = 1.0f;
	float CCurve = 1.0f;
inline static std::string formatValue(float value, int maxTotalDigits, int maxDecimals) {
	if (std::isnan(value)) return "NaN";
    if (std::isinf(value)) return "Inf";

    float absValue = std::abs(value);

    // 1. Проверка на превышение максимально допустимого целого числа
    // Например, если maxTotalDigits = 4, то макс. число 9999
    double maxLimit = std::pow(10, maxTotalDigits) - 1;
    if (absValue > maxLimit) {
        std::string overflow(maxTotalDigits, '9');
		return (value < 0 ? "-" : "") + overflow;
    }

    // 2. Считаем количество цифр в целой части для определения точности
    int intPartDigits = (absValue < 1.0f) ? 1 : static_cast<int>(std::log10(absValue)) + 1;

    // 3. Вычисляем точность (сколько знаков влезет после запятой)
    int allowedDecimals = maxTotalDigits - intPartDigits;
    int finalPrecision = std::clamp(allowedDecimals, 0, maxDecimals);

    // 4. Форматирование
    std::stringstream ss;
    ss << std::fixed << std::setprecision(finalPrecision) << value;
    std::string result = ss.str();

    // Заменяем точку на запятую
    std::replace(result.begin(), result.end(), '.', ',');

    return result;
}

inline static String formatValue_c(float value, int maxTotalDigits, int maxDecimals) {
        return formatValue(value, maxTotalDigits , maxDecimals ).c_str();
}


    struct TStringGridCellInfo {
    int TopLeftX;     // X-координата верхнего левого угла ячейки (0,0) относительно StringGrid
    int TopLeftY;     // Y-координата верхнего левого угла ячейки (0,0) относительно StringGrid
    int CellHeight;   // Высота ячейки (0,0)
    bool IsValid;     // Флаг, указывающий, удалось ли получить информацию

};

void __fastcall UpdateCycleTimeLabel();
	bool disableHandlers = false;
	Timer CycleTimeLabelTimer;
	std::vector<int> LastCycleDurationValue {0, 0, 0};



 void __fastcall TForm1::SaveCurrentToPreset(size_t index);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
