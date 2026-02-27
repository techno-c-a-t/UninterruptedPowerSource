object Form1: TForm1
  Left = 0
  Top = 0
  Margins.Left = 50
  Margins.Top = 50
  Margins.Right = 10
  Margins.Bottom = 10
  Anchors = [akLeft, akTop, akRight, akBottom]
  BorderStyle = bsSingle
  BorderWidth = 1
  Caption = 'PowerSource interface'
  ClientHeight = 791
  ClientWidth = 1538
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -28
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  Scaled = False
  OnClose = FormClose
  OnCreate = FormCreate
  TextHeight = 38
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
  object Label5: TLabel
    Left = 1152
    Top = 843
    Width = 132
    Height = 38
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Stab mode'
  end
  object Label6: TLabel
    Left = 1032
    Top = 843
    Width = 83
    Height = 38
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Queue'
  end
  object Label12: TLabel
    Left = 1428
    Top = 1032
    Width = 95
    Height = 38
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Label12'
  end
  object Chart1: TChart
    Left = 8
    Top = 523
    Width = 441
    Height = 260
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
    TabOrder = 0
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
    TabOrder = 1
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
  object Button2: TButton
    Left = 1364
    Top = 568
    Width = 152
    Height = 39
    Caption = 'DropErrors'
    TabOrder = 2
    OnClick = Button2Click
  end
  object memoryGrid: TStringGrid
    Left = 868
    Top = 36
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
    TabOrder = 3
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
    TabOrder = 4
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
    TabOrder = 5
    OnClick = Button4Click
  end
  object pnlStattic: TPanel
    Left = 16
    Top = 119
    Width = 620
    Height = 396
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BevelOuter = bvNone
    BorderWidth = 1
    TabOrder = 6
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
      object Label2: TLabel
        Left = 226
        Top = 4
        Width = 70
        Height = 107
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Align = alRight
        Alignment = taCenter
        AutoSize = False
        Caption = 'A'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -56
        Font.Name = 'Segoe UI'
        Font.Style = [fsItalic]
        ParentFont = False
        Layout = tlCenter
        ExplicitHeight = 109
      end
      object Edit8: TEdit
        Left = 5
        Top = 5
        Width = 215
        Height = 98
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
      object Label1: TLabel
        Left = 227
        Top = 3
        Width = 70
        Height = 109
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Align = alRight
        Alignment = taCenter
        AutoSize = False
        Caption = 'V'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -56
        Font.Name = 'Segoe UI'
        Font.Style = [fsItalic]
        ParentFont = False
        Layout = tlCenter
        ExplicitHeight = 107
      end
      object Edit9: TEdit
        Left = 5
        Top = 5
        Width = 215
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
      Left = 13
      Top = 282
      Width = 590
      Height = 103
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      AutoSize = False
      Color = clGray
      DisabledColor = clBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -40
      Font.Name = 'Segoe UI'
      Font.Style = []
      FrameColor = clGray
      ParentFont = False
      StateCaptions.CaptionOn = '   '#1042#1050#1051
      StateCaptions.CaptionOff = #1042#1067#1050#1051
      StyleElements = [seClient, seBorder]
      SwitchHeight = 80
      SwitchWidth = 480
      TabOrder = 4
      ThumbColor = clBlack
      ThumbWidth = 0
      OnClick = ToggleSwitch2Click
    end
  end
  object pnlCalkParam: TPanel
    Left = 640
    Top = 119
    Width = 223
    Height = 386
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BevelInner = bvLowered
    BevelWidth = 2
    TabOrder = 7
    object Time: TLabel
      Left = 4
      Top = 4
      Width = 215
      Height = 54
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Align = alTop
      Alignment = taCenter
      Caption = 'time'
      Color = clBlack
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
    object Power: TPanel
      Left = 4
      Top = 58
      Width = 215
      Height = 63
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      Align = alTop
      TabOrder = 0
      object powerValue: TLabel
        Left = 1
        Top = 1
        Width = 128
        Height = 61
        Margins.Left = 18
        Margins.Top = 18
        Margins.Right = 18
        Margins.Bottom = 18
        Align = alLeft
        AutoSize = False
        BiDiMode = bdRightToLeft
        Caption = 'powerValue'
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -40
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
      object W: TLabel
        Left = 152
        Top = 1
        Width = 40
        Height = 54
        Margins.Left = 12
        Margins.Top = 12
        Margins.Right = 12
        Margins.Bottom = 12
        BiDiMode = bdLeftToRight
        Caption = 'W'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -40
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
    end
    object Panel10: TPanel
      Left = 4
      Top = 121
      Width = 215
      Height = 62
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      Align = alTop
      TabOrder = 1
      object EnergyLabel: TLabel
        Left = 1
        Top = 1
        Width = 128
        Height = 60
        Margins.Left = 18
        Margins.Top = 18
        Margins.Right = 18
        Margins.Bottom = 18
        Align = alLeft
        AutoSize = False
        BiDiMode = bdRightToLeft
        Caption = 'Energy'
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -40
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
        ExplicitLeft = 9
        ExplicitTop = 2
      end
      object Label9: TLabel
        Left = 144
        Top = 0
        Width = 64
        Height = 54
        Margins.Left = 12
        Margins.Top = 12
        Margins.Right = 12
        Margins.Bottom = 12
        BiDiMode = bdLeftToRight
        Caption = 'Wh'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -40
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
    end
  end
  object gbDeviceSelect: TGroupBox
    Left = 24
    Top = 3
    Width = 244
    Height = 92
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BiDiMode = bdLeftToRight
    Caption = #1042#1099#1073#1086#1088' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -28
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentBackground = False
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 8
    DesignSize = (
      244
      92)
    object ComboBox1: TComboBox
      Left = 33
      Top = 38
      Width = 193
      Height = 46
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      Anchors = [akLeft, akTop, akRight, akBottom]
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -28
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = 'SPPS3001D'
      Items.Strings = (
        'SPPS3001D')
    end
  end
  object SaveMemGrid: TButton
    Left = 868
    Top = 447
    Width = 205
    Height = 38
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'SaveMemGrid'
    TabOrder = 11
    OnClick = SaveMemGridClick
  end
  object gbCommPortSelect: TGroupBox
    Left = 288
    Top = 3
    Width = 340
    Height = 92
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BiDiMode = bdLeftToRight
    Caption = #1042#1099#1073#1086#1088' '#1087#1086#1088#1090#1072' '
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentBackground = False
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 9
    DesignSize = (
      340
      92)
    object ComboBoxComPorts: TComboBox
      Left = 8
      Top = 38
      Width = 162
      Height = 46
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      AutoComplete = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -28
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = 'ComboBoxComPorts'
    end
    object ButtonRefreshPorts: TButton
      Left = 182
      Top = 38
      Width = 147
      Height = 46
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      Anchors = [akLeft, akTop, akRight, akBottom]
      Caption = #1054#1073#1085#1086#1074#1080#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -28
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = ButtonRefreshPortsClick
    end
  end
  object gbConnecction: TGroupBox
    Left = 647
    Top = 3
    Width = 202
    Height = 92
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BiDiMode = bdLeftToRight
    Caption = #1055#1086#1076#1082#1083#1102#1095#1077#1085#1080#1077
    Color = clBtnFace
    DefaultHeaderFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = []
    HeaderFont.Charset = DEFAULT_CHARSET
    HeaderFont.Color = clWindowText
    HeaderFont.Height = -28
    HeaderFont.Name = 'Segoe UI'
    HeaderFont.Style = []
    ParentBackground = False
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 10
    object ToggleSwitch1: TToggleSwitch
      Left = 17
      Top = 41
      Width = 185
      Height = 46
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      AutoSize = False
      Color = clGray
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -28
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      StateCaptions.CaptionOn = #1055#1054#1044#1050#1051
      StateCaptions.CaptionOff = '   '#1054#1058#1050#1051
      SwitchHeight = 38
      SwitchWidth = 70
      TabOrder = 0
      ThumbWidth = 0
      OnClick = ToggleSwitch1Click
    end
  end
  object GroupBox1: TGroupBox
    Left = 491
    Top = 568
    Width = 278
    Height = 158
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'GroupBox1'
    TabOrder = 12
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 680
    Top = 160
  end
end
