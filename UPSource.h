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
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include "src/components/MemTable.h"
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
    TChart *Chart2;
    TLineSeries *LineSeries1;
    TToggleSwitch *ToggleSwitch1;
    TLabel *Label7;
    TPanel *Panel3;
    TLabel *currentValue;
    TLabel *Label4;
    TPanel *Panel4;
    TLabel *Label3;
    TLabel *voltageValue;
    TToggleSwitch *ToggleSwitch2;
    TComboBox *ComboBoxComPorts;
    TButton *ButtonRefreshPorts;
    TComboBox *ComboBox1;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label12;
    TButton *Button2;
    TLabel *Label2;
    TLabel *Label1;
    TStringGrid *memoryGrid;
    TStringGrid *StringGrid2;
    TButton *Button4;
    TLabel *powerValue;
	TLabel *W;
    TLabel *Time;
	TPanel *pnlStattic;
	TPanel *pnlCalkParam;
	TPanel *Power;
	TPanel *Panel10;
	TGroupBox *gbDeviceSelect;
	TButton *SaveMemGrid;
	TGroupBox *gbCommPortSelect;
	TGroupBox *gbConnecction;
	TGroupBox *GroupBox1;
	TLabel *Label8;
	TLabel *Label9;
    // NOT INTERFACE

    void __fastcall PopulateComPorts();
    void __fastcall requestVAUpdate(bool forceUpdate);
    void __fastcall ApplyPreset(size_t row);
    //INTERFACE
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ScrollBar1Change(TObject *Sender);
    void __fastcall ScrollBar2Change(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ToggleSwitch1Click(TObject *Sender);
    void __fastcall ToggleSwitch2Click(TObject *Sender);
    void __fastcall Label8Click(TObject *Sender);
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






private:	// User declarations
public:		// User declarations
            /**
 * Форматирует float в строку с жестким контролем лимитов.
 * @param value - само число
 * @param maxTotalDigits - сколько всего цифр (значимых) может быть в строке
 * @param maxDecimals - потолок знаков после запятой
 */
inline static std::string formatValue(float value, int maxTotalDigits, int maxDecimals) {
    if (std::isnan(value)) return "NaN";
    if (std::isinf(value)) return "Inf";

    // 1. Считаем количество цифр в целой части
    float absValue = std::abs(value);
    int intPartDigits = 0;

    if (absValue < 1.0f) {
        intPartDigits = 1; // Для чисел типа 0.123 целая часть — это одна цифра '0'
    } else {
        intPartDigits = static_cast<int>(std::log10(absValue)) + 1;
    }

    // 2. Вычисляем, сколько места осталось на дробную часть
    // Вычитаем из общего лимита целую часть
    int allowedDecimals = maxTotalDigits - intPartDigits;

    // Дробных знаков не может быть меньше нуля и больше нашего "потолка" (maxDecimals)
    int finalPrecision = std::max(0, std::min(allowedDecimals, maxDecimals));

    // 3. Форматируем через stringstream
    std::stringstream ss;
    ss << std::fixed << std::setprecision(finalPrecision) << value;

    std::string result = ss.str();
    std::replace(result.begin(), result.end(), '.', ',');
    // 4. Тонкий момент: если число слишком большое и даже без дробной части
    // превышает maxTotalDigits, можно либо обрезать, либо вернуть как есть.
    // Обычно в приборах в таких случаях выводят "----" или ограничивают.

    return result;
}
inline static String formatValue_c(float value, int maxTotalDigits, int maxDecimals) {
        return formatValue(value, maxTotalDigits , maxDecimals ).c_str();
}

    __fastcall TForm1(TComponent* Owner);

    struct TStringGridCellInfo {
    int TopLeftX;     // X-координата верхнего левого угла ячейки (0,0) относительно StringGrid
    int TopLeftY;     // Y-координата верхнего левого угла ячейки (0,0) относительно StringGrid
    int CellHeight;   // Высота ячейки (0,0)
    bool IsValid;     // Флаг, указывающий, удалось ли получить информацию

};
    bool disableHandlers = false;



 void __fastcall TForm1::SaveCurrentToPreset(size_t index);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
