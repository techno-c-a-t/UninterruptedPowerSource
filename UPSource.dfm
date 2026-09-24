object Form1: TForm1
  Left = 0
  Top = 10
  Margins.Left = 50
  Margins.Top = 50
  Margins.Right = 10
  Margins.Bottom = 10
  Anchors = [akLeft, akTop, akRight, akBottom]
  BorderWidth = 3
  Caption = 'PowerSource interface'
  ClientHeight = 865
  ClientWidth = 1394
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -24
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  Scaled = False
  OnClose = FormClose
  OnCreate = FormCreate
  DesignSize = (
    1394
    865)
  TextHeight = 32
  object Label5: TLabel
    Left = 930
    Top = 672
    Width = 62
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Label5'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Chart1: TChart
    Left = 8
    Top = 569
    Width = 892
    Height = 288
    Border.Width = 0
    Legend.Visible = False
    MarginBottom = 0
    MarginLeft = 0
    MarginRight = 0
    MarginTop = 0
    MarginUnits = muPixels
    Title.Font.Color = clBlack
    Title.Text.Strings = (
      'TChart')
    Title.VertMargin = 0
    LeftAxis.Title.Caption = #1042
    LeftAxis.Title.Position = tpStart
    LeftAxis.TitleSize = 8
    Shadow.Smooth = False
    Shadow.Visible = False
    View3D = False
    BevelOuter = bvNone
    TabOrder = 0
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object CheckBox1: TCheckBox
      Left = 500
      Top = 1290
      Width = 146
      Height = 26
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      Caption = 'CheckBox1'
      TabOrder = 0
    end
    object Series1: TLineSeries
      HoverElement = [heCurrent]
      Brush.BackColor = clDefault
      LinePen.Width = 3
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series2: TLineSeries
      HoverElement = [heCurrent]
      Brush.BackColor = clDefault
      LinePen.Width = 3
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object Button2: TButton
    Left = 1284
    Top = 826
    Width = 100
    Height = 25
    Caption = 'DropErrors'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = Button2Click
  end
  object StringGrid2: TStringGrid
    AlignWithMargins = True
    Left = 930
    Top = 10
    Width = 454
    Height = 615
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    DefaultColWidth = 80
    DefaultRowHeight = 36
    RowCount = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goFixedRowDefAlign]
    ScrollBars = ssVertical
    TabOrder = 2
    OnKeyDown = memoryGridKeyDown
  end
  object Button4: TButton
    Left = 1284
    Top = 800
    Width = 100
    Height = 25
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'DropState'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = Button4Click
  end
  object pnlStattic: TPanel
    Left = 10
    Top = 133
    Width = 890
    Height = 420
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BevelWidth = 6
    TabOrder = 4
    DesignSize = (
      890
      420)
    object CycleTimeLabel: TLabel
      Left = 25
      Top = 362
      Width = 137
      Height = 45
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      Alignment = taCenter
      Anchors = []
      AutoSize = False
      Caption = '00:00:12'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold, fsItalic]
      ParentFont = False
    end
    object Panel4: TPanel
      Left = 8
      Top = 8
      Width = 300
      Height = 100
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      BevelWidth = 3
      TabOrder = 0
      DesignSize = (
        300
        100)
      object Label3: TLabel
        Left = 234
        Top = 6
        Width = 48
        Height = 76
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taRightJustify
        Anchors = []
        AutoSize = False
        Caption = 'V'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -72
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        Layout = tlCenter
        StyleElements = []
      end
      object voltageValue: TLabel
        Left = 2
        Top = -8
        Width = 222
        Height = 94
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taRightJustify
        AutoSize = False
        Caption = '0.000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -80
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        Transparent = True
        Layout = tlCenter
      end
    end
    object Panel3: TPanel
      Left = 320
      Top = 8
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
        Left = 2
        Top = -8
        Width = 222
        Height = 94
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taRightJustify
        Anchors = [akLeft, akTop, akBottom]
        AutoSize = False
        Caption = '0.000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMaroon
        Font.Height = -80
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentFont = False
        Transparent = True
        Layout = tlCenter
      end
      object Label4: TLabel
        Left = 229
        Top = 6
        Width = 52
        Height = 76
        Margins.Left = 5
        Margins.Top = 5
        Margins.Right = 5
        Margins.Bottom = 5
        Alignment = taRightJustify
        AutoSize = False
        BiDiMode = bdLeftToRight
        Caption = 'A'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -72
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
        Layout = tlCenter
        StyleElements = []
      end
    end
    object Panel1: TPanel
      Left = 320
      Top = 120
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
        ExplicitLeft = 230
        ExplicitTop = 6
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
      Left = 8
      Top = 120
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
      Left = 33
      Top = 279
      Width = 272
      Margins.Left = 12
      Margins.Top = 12
      Margins.Right = 12
      Margins.Bottom = 12
      AutoSize = False
      Color = clGray
      DisabledColor = clBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -28
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      FrameColor = clGray
      ParentFont = False
      StateCaptions.CaptionOn = '   '#1042#1050#1051
      StateCaptions.CaptionOff = #1042#1067#1050#1051
      StyleElements = [seBorder]
      SwitchHeight = 50
      SwitchWidth = 180
      TabOrder = 4
      ThumbColor = clBlack
      ThumbWidth = 0
      OnClick = ToggleSwitch2Click
    end
    object Power: TPanel
      Left = 320
      Top = 274
      Width = 300
      Height = 40
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      TabOrder = 5
      object powerValue: TLabel
        Left = 40
        Top = 1
        Width = 150
        Height = 40
        Margins.Left = 27
        Margins.Top = 27
        Margins.Right = 27
        Margins.Bottom = 27
        AutoSize = False
        BiDiMode = bdRightToLeft
        Caption = 'powerValue'
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
      object W: TLabel
        Left = 220
        Top = 1
        Width = 24
        Height = 32
        Margins.Left = 18
        Margins.Top = 18
        Margins.Right = 18
        Margins.Bottom = 18
        BiDiMode = bdLeftToRight
        Caption = 'W'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
    end
    object Panel5: TPanel
      Left = 320
      Top = 320
      Width = 300
      Height = 40
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      TabOrder = 6
      object energyLabel: TLabel
        Left = 40
        Top = 1
        Width = 150
        Height = 40
        Margins.Left = 27
        Margins.Top = 27
        Margins.Right = 27
        Margins.Bottom = 27
        AutoSize = False
        BiDiMode = bdRightToLeft
        Caption = 'Energy'
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
      object Wh: TLabel
        Left = 216
        Top = 1
        Width = 38
        Height = 32
        Margins.Left = 18
        Margins.Top = 18
        Margins.Right = 18
        Margins.Bottom = 18
        BiDiMode = bdLeftToRight
        Caption = 'Wh'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
    end
    object Panel10: TPanel
      Left = 320
      Top = 366
      Width = 300
      Height = 40
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      TabOrder = 7
      object batteryCapacity: TLabel
        Left = 40
        Top = 1
        Width = 150
        Height = 40
        Margins.Left = 27
        Margins.Top = 27
        Margins.Right = 27
        Margins.Bottom = 27
        AutoSize = False
        BiDiMode = bdRightToLeft
        Caption = 'batt capacity'
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
      object Label9: TLabel
        Left = 222
        Top = 1
        Width = 31
        Height = 32
        Margins.Left = 18
        Margins.Top = 18
        Margins.Right = 18
        Margins.Bottom = 18
        BiDiMode = bdLeftToRight
        Caption = 'Ah'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentFont = False
      end
    end
    object memoryGrid: TStringGrid
      Left = 644
      Top = 8
      Width = 204
      Height = 366
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      BevelWidth = 6
      ColCount = 3
      DefaultColWidth = 70
      DefaultColAlignment = taRightJustify
      DefaultRowHeight = 32
      RowCount = 11
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goRowSelect, goFixedRowDefAlign]
      ScrollBars = ssNone
      TabOrder = 8
      OnKeyDown = memoryGridKeyDown
      OnMouseDown = memoryGridMouseDown
      RowHeights = (
        32
        32
        32
        32
        32
        32
        32
        32
        32
        32
        32)
    end
    object SaveMemGrid: TButton
      Left = 644
      Top = 375
      Width = 204
      Height = 30
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      Caption = 'SaveMemGrid'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
      OnClick = SaveMemGridClick
    end
    object SaweSeans: TButton
      Left = 175
      Top = 368
      Width = 129
      Height = 34
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      Caption = 'SaweSeans'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Segoe UI Semibold'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 10
      OnClick = SaweSeansClick
    end
  end
  object pnlCalkParam: TPanel
    Left = 1143
    Top = 635
    Width = 239
    Height = 156
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BevelInner = bvLowered
    BevelWidth = 6
    TabOrder = 5
    object Time: TLabel
      Left = 12
      Top = 12
      Width = 215
      Height = 70
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Align = alTop
      Alignment = taCenter
      Caption = 'time'
      Color = clNavy
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clNavy
      Font.Height = -52
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      StyleElements = [seBorder]
      ExplicitWidth = 111
    end
  end
  object pnlComm: TPanel
    Left = 8
    Top = 8
    Width = 891
    Height = 115
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    BevelWidth = 6
    TabOrder = 6
    object gbConnection: TGroupBox
      Left = 613
      Top = 6
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
      TabOrder = 0
      object ToggleSwitch1: TToggleSwitch
        Left = 17
        Top = 38
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
        Font.Height = -24
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
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
    object gbDeviceSelect: TGroupBox
      Left = 9
      Top = 6
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
      TabOrder = 2
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
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -28
        Font.Name = 'Segoe UI'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 0
        Text = 'SPPS3001D'
        OnChange = ComboBox1Change
        Items.Strings = (
          'SPPS3001D')
      end
    end
    object gbCommPortSelect: TGroupBox
      Left = 263
      Top = 6
      Width = 340
      Height = 92
      Margins.Left = 5
      Margins.Top = 5
      Margins.Right = 5
      Margins.Bottom = 5
      BiDiMode = bdLeftToRight
      Caption = #1042#1099#1073#1086#1088' '#1087#1086#1088#1090#1072' '
      Color = clBtnFace
      Ctl3D = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -28
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentBackground = False
      ParentBiDiMode = False
      ParentColor = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 1
      DesignSize = (
        340
        92)
      object ComboBoxComPorts: TComboBox
        Left = 7
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
  end
  object CheckBoxCurrent: TCheckBox
    Left = 552
    Top = 560
    Width = 78
    Height = 26
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = #1058#1086#1082
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold, fsItalic]
    ParentFont = False
    State = cbChecked
    TabOrder = 7
    StyleElements = [seClient, seBorder]
    OnClick = CheckBoxCurrentClick
  end
  object CheckBoxVoltage: TCheckBox
    Left = 395
    Top = 560
    Width = 150
    Height = 26
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold, fsItalic]
    ParentFont = False
    State = cbChecked
    TabOrder = 8
    StyleElements = [seClient, seBorder]
    OnClick = CheckBoxVoltageClick
  end
  object ButtonResetZoom: TButton
    Left = 680
    Top = 560
    Width = 140
    Height = 26
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = #1042#1077#1089#1100' '#1075#1088#1072#1092#1080#1082
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold, fsItalic]
    ParentFont = False
    TabOrder = 9
    OnClick = ButtonResetZoomClick
  end
  object Panel6: TPanel
    Left = 80
    Top = 559
    Width = 256
    Height = 26
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Alignment = taRightJustify
    Anchors = [akLeft]
    BevelOuter = bvNone
    Caption = #1089#1077#1082' '#1044#1080#1089#1082#1088#1077#1090#1085#1086#1089#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold, fsItalic]
    ParentFont = False
    TabOrder = 10
    ExplicitTop = 548
    object discret: TEdit
      Left = 0
      Top = -4
      Width = 65
      Height = 26
      Margins.Left = 8
      Margins.Top = 8
      Margins.Right = 8
      Margins.Bottom = 8
      Alignment = taRightJustify
      AutoSize = False
      BorderStyle = bsNone
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold, fsItalic]
      ParentFont = False
      TabOrder = 0
      Text = '10'
      TextHint = '-'
      OnExit = discretExit
      OnKeyPress = discretKeyPress
    end
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 1160
    Top = 656
  end
  object sdSaveSeans: TSaveDialog
    Left = 256
    Top = 504
  end
end
