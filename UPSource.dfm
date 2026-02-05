object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'PowerSource interface'
  ClientHeight = 545
  ClientWidth = 1721
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
    Left = 872
    Top = 25
    Width = 267
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = #1057#1086#1089#1090#1086#1103#1085#1080#1077' '#1089#1074#1103#1079#1080' '#1089' '#1041#1055
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    OnClick = Label8Click
  end
  object Label9: TLabel
    Left = 8
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
    Left = 420
    Top = 25
    Width = 62
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = #1055#1086#1088#1090
  end
  object Label11: TLabel
    Left = 1188
    Top = 996
    Width = 94
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Label11'
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
  object LabelSelectedPort: TLabel
    Left = 1688
    Top = 639
    Width = 212
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'LabelSelectedPort'
    Visible = False
  end
  object Label13: TLabel
    Left = 143
    Top = 90
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
    Left = 143
    Top = 137
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
  object powerValue: TLabel
    Left = 91
    Top = 349
    Width = 256
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    AutoSize = False
    BiDiMode = bdRightToLeft
    Caption = 'powerValue'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentBiDiMode = False
    ParentFont = False
  end
  object Label15: TLabel
    Left = 369
    Top = 349
    Width = 27
    Height = 37
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'W'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object RadioGroup1: TRadioGroup
    Left = 1312
    Top = 592
    Width = 188
    Height = 116
    BiDiMode = bdLeftToRight
    Columns = 2
    Ctl3D = True
    Items.Strings = (
      'C.C.'
      'C.V.')
    ParentBiDiMode = False
    ParentCtl3D = False
    TabOrder = 8
    StyleElements = [seFont, seClient]
  end
  object Panel1: TPanel
    Left = 219
    Top = 197
    Width = 351
    Height = 147
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    TabOrder = 4
    object Label2: TLabel
      Left = 280
      Top = -19
      Width = 56
      Height = 106
      Caption = 'A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -80
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Edit8: TEdit
      Left = 8
      Top = -24
      Width = 266
      Height = 114
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -80
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = '0,00'
      OnExit = Edit8Exit
      OnKeyDown = Edit8KeyDown
    end
    object ScrollBar1: TScrollBar
      Left = 8
      Top = 90
      Width = 329
      Height = 40
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Ctl3D = True
      LargeChange = 50
      PageSize = 0
      ParentCtl3D = False
      TabOrder = 1
      StyleElements = [seFont, seBorder]
      OnChange = ScrollBar1Change
    end
  end
  object Edit1: TEdit
    Left = 1032
    Top = 760
    Width = 337
    Height = 45
    TabOrder = 0
    Text = 'Edit1'
    Visible = False
  end
  object Button1: TButton
    Left = 1375
    Top = 760
    Width = 346
    Height = 45
    Caption = #1054#1090#1087#1088#1072#1074#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Visible = False
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 1032
    Top = 592
    Width = 337
    Height = 162
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 2
    Visible = False
  end
  object Memo2: TMemo
    Left = 1375
    Top = 592
    Width = 346
    Height = 162
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 3
    Visible = False
  end
  object Panel2: TPanel
    Left = 583
    Top = 197
    Width = 353
    Height = 147
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    TabOrder = 5
    object Label1: TLabel
      Left = 284
      Top = -21
      Width = 53
      Height = 106
      Caption = 'V'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -80
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Edit9: TEdit
      Left = 10
      Top = -24
      Width = 266
      Height = 114
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -80
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = '0,00'
      OnExit = Edit9Exit
      OnKeyDown = Edit9KeyDown
    end
    object ScrollBar2: TScrollBar
      Left = 6
      Top = 90
      Width = 323
      Height = 40
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
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
  object Chart1: TChart
    Left = 420
    Top = 695
    Width = 404
    Height = 250
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
    TabOrder = 6
    Visible = False
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
    Left = 12
    Top = 695
    Width = 400
    Height = 250
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
    TabOrder = 7
    Visible = False
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
    Left = 1147
    Top = 27
    Width = 40
    Height = 39
    AutoSize = False
    StateCaptions.CaptionOn = ' '
    StateCaptions.CaptionOff = ' '
    SwitchWidth = 40
    TabOrder = 9
    OnClick = ToggleSwitch1Click
  end
  object Panel3: TPanel
    Left = 219
    Top = 88
    Width = 350
    Height = 104
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 10
    object currentValue: TLabel
      Left = 8
      Top = -9
      Width = 266
      Height = 121
      AutoSize = False
      Caption = '0.000'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -80
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
      Transparent = True
    end
    object Label4: TLabel
      Left = 280
      Top = -9
      Width = 56
      Height = 106
      Caption = 'A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -80
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object Panel4: TPanel
    Left = 583
    Top = 88
    Width = 350
    Height = 104
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 11
    object Label3: TLabel
      Left = 281
      Top = -11
      Width = 53
      Height = 106
      Caption = 'V'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -80
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object voltageValue: TLabel
      Left = 9
      Top = -11
      Width = 266
      Height = 111
      AutoSize = False
      Caption = '0.000'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -80
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
      Transparent = True
    end
  end
  object ToggleSwitch2: TToggleSwitch
    Left = 8
    Top = 73
    Width = 104
    Height = 132
    AutoSize = False
    Color = clRed
    DisabledColor = clBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -28
    Font.Name = 'Segoe UI'
    Font.Style = []
    FrameColor = clMaroon
    ParentFont = False
    StateCaptions.CaptionOn = 'ON'
    StateCaptions.CaptionOff = 'OFF'
    StyleElements = [seClient, seBorder]
    SwitchHeight = 100
    SwitchWidth = 100
    TabOrder = 12
    ThumbColor = clBlack
    ThumbWidth = 33
    OnClick = ToggleSwitch2Click
  end
  object ComboBoxComPorts: TComboBox
    Left = 492
    Top = 26
    Width = 188
    Height = 45
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    AutoComplete = False
    TabOrder = 13
    Text = 'ComboBoxComPorts'
    OnChange = ComboBoxComPortsChange
  end
  object ButtonRefreshPorts: TButton
    Left = 688
    Top = 28
    Width = 132
    Height = 41
    Caption = #1054#1073#1085#1086#1074#1080#1090#1100
    TabOrder = 14
    OnClick = ButtonRefreshPortsClick
  end
  object ComboBox1: TComboBox
    Left = 115
    Top = 26
    Width = 218
    Height = 45
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 15
    Text = 'SPPS3001D'
    Items.Strings = (
      'SPPS3001D')
  end
  object Button2: TButton
    Left = 39
    Top = 197
    Width = 152
    Height = 52
    Caption = 'DropErrors'
    TabOrder = 16
    OnClick = Button2Click
  end
  object memoryGrid: TStringGrid
    Left = 976
    Top = 88
    Width = 220
    Height = 415
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    ColCount = 3
    DefaultColWidth = 70
    DefaultRowHeight = 36
    RowCount = 11
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goRowSelect, goFixedRowDefAlign]
    ScrollBars = ssNone
    TabOrder = 17
    OnKeyDown = memoryGridKeyDown
    OnMouseDown = memoryGridMouseDown
  end
  object StringGrid2: TStringGrid
    Left = 1292
    Top = 88
    Width = 517
    Height = 415
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    ColCount = 6
    DefaultColWidth = 80
    DefaultRowHeight = 36
    RowCount = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goFixedRowDefAlign]
    ScrollBars = ssVertical
    TabOrder = 18
    Visible = False
    OnKeyDown = memoryGridKeyDown
  end
  object Button3: TButton
    Left = 1932
    Top = 12
    Width = 113
    Height = 38
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Button3'
    TabOrder = 19
  end
  object Button4: TButton
    Left = 39
    Top = 264
    Width = 152
    Height = 63
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'DropState'
    TabOrder = 20
    OnClick = Button4Click
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 1216
  end
end
