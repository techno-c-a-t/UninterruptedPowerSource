object Form1: TForm1
  Left = 0
  Top = 0
  Margins.Left = 50
  Margins.Top = 50
  Margins.Right = 10
  Margins.Bottom = 10
  BorderWidth = 1
  Caption = 'PowerSource interface'
  ClientHeight = 791
  ClientWidth = 1538
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -27
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  Scaled = False
  OnClose = FormClose
  OnCreate = FormCreate
  TextHeight = 37
  object Label7: TLabel
    Left = 889
    Top = 876
    Width = 103
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Lock off'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label8: TLabel
    Left = 749
    Top = 25
    Width = 131
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = #1057#1074#1103#1079#1100' '#1089' '#1041#1055
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    OnClick = Label8Click
  end
  object Label9: TLabel
    Left = 23
    Top = 28
    Width = 97
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = #1052#1086#1076#1077#1083#1100
  end
  object Label10: TLabel
    Left = 350
    Top = 25
    Width = 62
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = #1055#1086#1088#1090
  end
  object Label5: TLabel
    Left = 1152
    Top = 843
    Width = 128
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Stab mode'
  end
  object Label6: TLabel
    Left = 1032
    Top = 843
    Width = 78
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Queue'
  end
  object Label12: TLabel
    Left = 1428
    Top = 1032
    Width = 94
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Label12'
  end
  object Label13: TLabel
    Left = 1364
    Top = 714
    Width = 48
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'C.C.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label14: TLabel
    Left = 1451
    Top = 714
    Width = 46
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'C.V.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Button1: TButton
    Left = 1364
    Top = 647
    Width = 148
    Height = 45
    Caption = #1054#1090#1087#1088#1072#1074#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = Button1Click
  end
  object Chart1: TChart
    Left = 8
    Top = 555
    Width = 441
    Height = 228
    Legend.Visible = False
    MarginBottom = 0
    MarginLeft = 0
    MarginRight = 0
    MarginTop = 0
    MarginUnits = muPixels
    Title.Font.Color = clBlack
    Title.Text.Strings = (
      #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077)
    Title.VertMargin = 0
    Shadow.Smooth = False
    Shadow.Visible = False
    View3D = False
    BevelOuter = bvNone
    TabOrder = 1
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TLineSeries
      HoverElement = [heCurrent]
      Brush.BackColor = clDefault
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object Chart2: TChart
    Left = 912
    Top = 555
    Width = 393
    Height = 228
    Legend.Visible = False
    MarginBottom = 0
    MarginLeft = 0
    MarginRight = 0
    MarginTop = 0
    MarginUnits = muPixels
    Title.Font.Color = clBlack
    Title.Text.Strings = (
      #1058#1086#1082)
    Title.VertMargin = 0
    Shadow.Smooth = False
    Shadow.Visible = False
    View3D = False
    BevelOuter = bvNone
    TabOrder = 2
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object LineSeries1: TLineSeries
      HoverElement = [heCurrent]
      Brush.BackColor = clDefault
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object ToggleSwitch1: TToggleSwitch
    Left = 888
    Top = 25
    Width = 137
    Height = 39
    AutoSize = False
    StateCaptions.CaptionOn = #1042#1050#1051
    StateCaptions.CaptionOff = #1042#1067#1050#1051
    SwitchHeight = 30
    SwitchWidth = 60
    TabOrder = 3
    ThumbWidth = 0
    OnClick = ToggleSwitch1Click
  end
  object ComboBoxComPorts: TComboBox
    Left = 418
    Top = 25
    Width = 138
    Height = 45
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    AutoComplete = False
    TabOrder = 4
    Text = 'ComboBoxComPorts'
  end
  object ButtonRefreshPorts: TButton
    Left = 564
    Top = 27
    Width = 132
    Height = 41
    Caption = #1054#1073#1085#1086#1074#1080#1090#1100
    TabOrder = 5
    OnClick = ButtonRefreshPortsClick
  end
  object ComboBox1: TComboBox
    Left = 119
    Top = 21
    Width = 205
    Height = 45
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 6
    Text = 'SPPS3001D'
    Items.Strings = (
      'SPPS3001D')
  end
  object Button2: TButton
    Left = 1364
    Top = 568
    Width = 152
    Height = 39
    Caption = 'DropErrors'
    TabOrder = 7
    OnClick = Button2Click
  end
  object memoryGrid: TStringGrid
    Left = 859
    Top = 135
    Width = 205
    Height = 412
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    ColCount = 3
    DefaultColWidth = 70
    DefaultColAlignment = taRightJustify
    DefaultRowHeight = 36
    RowCount = 11
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goRowSelect, goFixedRowDefAlign]
    ScrollBars = ssNone
    TabOrder = 8
    OnKeyDown = memoryGridKeyDown
    OnMouseDown = memoryGridMouseDown
    RowHeights = (
      36
      36
      36
      38
      36
      36
      36
      36
      36
      36
      36)
  end
  object StringGrid2: TStringGrid
    AlignWithMargins = True
    Left = 1083
    Top = 3
    Width = 454
    Height = 474
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    DefaultColWidth = 80
    DefaultRowHeight = 36
    RowCount = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goFixedRowDefAlign]
    ScrollBars = ssVertical
    TabOrder = 9
    OnKeyDown = memoryGridKeyDown
  end
  object Button4: TButton
    Left = 1364
    Top = 512
    Width = 152
    Height = 35
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'DropState'
    TabOrder = 10
    OnClick = Button4Click
  end
  object pnlStattic: TPanel
    Left = 24
    Top = 151
    Width = 620
    Height = 396
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BevelOuter = bvNone
    BorderWidth = 1
    TabOrder = 11
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 300
      Height = 100
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      BevelWidth = 3
      TabOrder = 0
      object Label3: TLabel
        Left = 234
        Top = -10
        Width = 53
        Height = 106
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Caption = 'V'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -80
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        Layout = tlCenter
      end
      object voltageValue: TLabel
        Left = 2
        Top = -7
        Width = 222
        Height = 96
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taRightJustify
        AutoSize = False
        Caption = '0.000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -80
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        Transparent = True
        Layout = tlCenter
      end
    end
    object Panel3: TPanel
      Left = 317
      Top = 2
      Width = 300
      Height = 100
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      BevelWidth = 3
      TabOrder = 1
      DesignSize = (
        300
        100)
      object currentValue: TLabel
        Left = -3
        Top = -7
        Width = 222
        Height = 98
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taRightJustify
        Anchors = [akLeft, akTop, akBottom]
        AutoSize = False
        Caption = '0.000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -80
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        Transparent = True
        Layout = tlCenter
      end
      object Label4: TLabel
        Left = 229
        Top = -11
        Width = 56
        Height = 106
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Caption = 'A'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -80
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        Layout = tlCenter
      end
    end
    object Panel1: TPanel
      Left = 317
      Top = 109
      Width = 300
      Height = 150
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      BevelOuter = bvLowered
      BevelWidth = 3
      BorderWidth = 1
      TabOrder = 2
      DesignSize = (
        300
        150)
      object Label2: TLabel
        Left = 243
        Top = 22
        Width = 35
        Height = 74
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taCenter
        Anchors = [akTop, akRight]
        Caption = 'A'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -56
        Font.Name = 'Segoe UI'
        Font.Style = [fsItalic]
        ParentFont = False
        Layout = tlCenter
      end
      object Edit8: TEdit
        Left = -3
        Top = 6
        Width = 220
        Height = 104
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taRightJustify
        BevelOuter = bvNone
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -72
        Font.Name = 'Segoe UI'
        Font.Style = [fsItalic]
        ParentColor = True
        ParentFont = False
        TabOrder = 0
        Text = '0,000'
        OnExit = Edit8Exit
        OnKeyDown = Edit8KeyDown
      end
      object ScrollBar1: TScrollBar
        Left = 4
        Top = 111
        Width = 292
        Height = 35
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alBottom
        Ctl3D = True
        LargeChange = 50
        PageSize = 0
        ParentCtl3D = False
        TabOrder = 1
        StyleElements = [seFont, seBorder]
        OnChange = ScrollBar1Change
        ExplicitTop = 114
      end
    end
    object Panel2: TPanel
      Left = 0
      Top = 109
      Width = 300
      Height = 150
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      BevelOuter = bvLowered
      BevelWidth = 3
      TabOrder = 3
      DesignSize = (
        300
        150)
      object Label1: TLabel
        Left = 235
        Top = 13
        Width = 53
        Height = 98
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taCenter
        Anchors = [akTop, akRight]
        AutoSize = False
        Caption = 'V'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -56
        Font.Name = 'Segoe UI'
        Font.Style = [fsItalic]
        ParentFont = False
        Layout = tlCenter
      end
      object Edit9: TEdit
        Left = 5
        Top = 5
        Width = 220
        Height = 98
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taRightJustify
        AutoSelect = False
        AutoSize = False
        BevelOuter = bvNone
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -72
        Font.Name = 'Segoe UI'
        Font.Style = [fsItalic]
        ParentColor = True
        ParentFont = False
        TabOrder = 0
        Text = '0,000'
        OnExit = Edit9Exit
        OnKeyDown = Edit9KeyDown
      end
      object ScrollBar2: TScrollBar
        Left = 3
        Top = 112
        Width = 294
        Height = 35
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alBottom
        BiDiMode = bdLeftToRight
        Ctl3D = True
        LargeChange = 50
        Max = 30000
        PageSize = 0
        ParentBiDiMode = False
        ParentCtl3D = False
        TabOrder = 1
        StyleElements = [seFont, seBorder]
        OnChange = ScrollBar2Change
      end
    end
    object ToggleSwitch2: TToggleSwitch
      Left = 27
      Top = 282
      Width = 569
      Height = 103
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      AutoSize = False
      Color = clRed
      DisabledColor = clBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -42
      Font.Name = 'Segoe UI'
      Font.Style = []
      FrameColor = clMaroon
      ParentFont = False
      StateCaptions.CaptionOn = 'ON'
      StateCaptions.CaptionOff = 'OFF'
      StyleElements = [seClient, seBorder]
      SwitchHeight = 80
      SwitchWidth = 480
      TabOrder = 4
      ThumbColor = clBlack
      ThumbWidth = 0
      OnClick = ToggleSwitch2Click
    end
  end
  object Panel8: TPanel
    Left = 650
    Top = 185
    Width = 199
    Height = 398
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Panel8'
    TabOrder = 12
    object Time: TLabel
      Left = 1
      Top = 1
      Width = 197
      Height = 54
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Align = alTop
      Alignment = taCenter
      Caption = 'time'
      Color = clSlateblue
      Enabled = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -40
      Font.Name = 'Segoe UI Semibold'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      ExplicitWidth = 80
    end
    object Label15: TLabel
      Left = 156
      Top = 343
      Width = 41
      Height = 55
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      Caption = 'W'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -41
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Panel9: TPanel
      Left = 2
      Top = 68
      Width = 197
      Height = 61
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      Caption = 'Panel9'
      TabOrder = 0
      object powerValue: TLabel
        Left = 3
        Top = 8
        Width = 164
        Height = 46
        Margins.Left = 8
        Margins.Top = 8
        Margins.Right = 8
        Margins.Bottom = 8
        AutoSize = False
        BiDiMode = bdRightToLeft
        Caption = 'powerValue'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -28
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
    end
    object Panel10: TPanel
      Left = 16
      Top = 268
      Width = 166
      Height = 62
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      Caption = 'Panel10'
      TabOrder = 1
    end
  end
  object GroupBox1: TGroupBox
    AlignWithMargins = True
    Left = 487
    Top = 593
    Width = 393
    Height = 158
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'GroupBox1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
  end
  object SaveMemGrid: TButton
    Left = 859
    Top = 87
    Width = 205
    Height = 38
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'SaveMemGrid'
    TabOrder = 14
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 912
    Top = 200
  end
end
