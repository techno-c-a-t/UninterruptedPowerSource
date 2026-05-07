#ifndef HybridGroupBoxH
#define HybridGroupBoxH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <UxTheme.h>
#pragma comment(lib, "UxTheme.lib")

// Режим отрисовки рамок
enum TGBDrawMode { gbSingle, gbDouble, gbHybrid };

class PACKAGE THybridGroupBox : public TGroupBox
{
private:
    // Внешняя рамка
    int FBorderWidth;
    int FCornerRadius;
    TColor FBorderColor;

    // Внутренняя рамка
    int FBorderWidth2;
    int FCornerRadius2;
    TColor FBorderColor2;

    // Режим отрисовки
    TGBDrawMode FDrawMode;

protected:
    void __fastcall Paint() override;

public:
    __fastcall THybridGroupBox(TComponent* Owner);

__published:
    // Внешняя рамка
    __property int BorderWidthCustom = {read=FBorderWidth, write=FBorderWidth, default=2};
    __property int CornerRadius = {read=FCornerRadius, write=FCornerRadius, default=12};
    __property TColor BorderColor = {read=FBorderColor, write=FBorderColor, default=clBlack};

    // Внутренняя рамка
    __property int BorderWidthCustom2 = {read=FBorderWidth2, write=FBorderWidth2, default=1};
    __property int CornerRadius2 = {read=FCornerRadius2, write=FCornerRadius2, default=8};
    __property TColor BorderColor2 = {read=FBorderColor2, write=FBorderColor2, default=clGray};

    // Режим отрисовки
    __property TGBDrawMode DrawMode = {read=FDrawMode, write=FDrawMode, default=gbSingle};
};

#endif
