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
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TEdit *Edit1;
    TButton *Button1;
    TMemo *Memo1;
    TMemo *Memo2;
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
    TRadioGroup *RadioGroup1;
    TToggleSwitch *ToggleSwitch1;
    TLabel *Label7;
    TLabel *Label8;
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
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TTimer *Timer2;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label12;
    TLabel *LabelSelectedPort;
    TButton *Button2;
    TLabel *Label2;
    TLabel *Label1;
    TLabel *Label13;
    TLabel *Label14;
    TStringGrid *StringGrid1;
    TStringGrid *StringGrid2;
    //NOT INTERFACE
    void __fastcall handleDisconnection();
    void __fastcall handleConnection();
    void __fastcall PopulateComPorts();
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
    void __fastcall ComboBoxComPortsChange(TObject *Sender);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Edit9Exit(TObject *Sender);
    void __fastcall Edit9KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall Edit8Exit(TObject *Sender);
    void __fastcall Edit8KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall StringGrid1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);



private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);

    struct TStringGridCellInfo {
    int TopLeftX;     // X-координата верхнего левого угла ячейки (0,0) относительно StringGrid
    int TopLeftY;     // Y-координата верхнего левого угла ячейки (0,0) относительно StringGrid
    int CellHeight;   // Высота ячейки (0,0)
    bool IsValid;     // Флаг, указывающий, удалось ли получить информацию
};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
