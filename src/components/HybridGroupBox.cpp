#include <vcl.h>
#pragma hdrstop
#include "HybridGroupBox.h"
#pragma package(smart_init)

__fastcall THybridGroupBox::THybridGroupBox(TComponent* Owner)
    : TGroupBox(Owner)
{
    // Внешняя рамка
    FBorderWidth = 2;
    FCornerRadius = 12;
    FBorderColor = clBlack;

    // Внутренняя рамка
    FBorderWidth2 = 1;
    FCornerRadius2 = 8;
    FBorderColor2 = clGray;

    // Режим
    FDrawMode = gbSingle;
}

void __fastcall THybridGroupBox::Paint()
{
    TCanvas *C = Canvas;
    C->Brush->Color = Color;
    C->FillRect(ClientRect);

    int textHeight = C->TextHeight(Caption);
    int textOffset = 8;

    // ===== Внешняя рамка =====
    HTHEME theme = OpenThemeData(Handle, L"BUTTON");
    if (theme)
    {
        TRect R = ClientRect;
        if (FDrawMode != gbHybrid) // в гибриде внутренняя рамка сплошная
            R.Top += textHeight / 2;
        else
            R.Top += textHeight / 2; // всё равно прерываем внешнюю
        DrawThemeBackground(theme, C->Handle, BP_GROUPBOX, GBS_NORMAL, &R, NULL);
        CloseThemeData(theme);
    }

    // Рисуем внешнюю рамку
    C->Pen->Color = FBorderColor;
    C->Pen->Width = FBorderWidth;
    C->Brush->Style = bsClear;
    TRect R1 = ClientRect;
    R1.Top += textHeight / 2; // разрыв под текст
    C->RoundRect(R1.Left, R1.Top, R1.Right, R1.Bottom, FCornerRadius, FCornerRadius);

    // ===== Внутренняя рамка =====
    if (FDrawMode != gbSingle)
    {
        C->Pen->Color = FBorderColor2;
        C->Pen->Width = FBorderWidth2;
        TRect R2 = ClientRect;
        int offset = FBorderWidth + 4;
        InflateRect(&R2, -offset, -offset);

        if (FDrawMode == gbDouble)
            R2.Top += textHeight / 2; // двойная рамка тоже прерывается
        // в gbHybrid — без сдвига, рамка сплошная

        C->RoundRect(R2.Left, R2.Top, R2.Right, R2.Bottom, FCornerRadius2, FCornerRadius2);
    }

    // ===== Текст =====
    C->Brush->Style = bsSolid;
    C->Brush->Color = Color;
    C->TextOut(textOffset, 0, Caption);
}
