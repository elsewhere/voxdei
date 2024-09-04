object Form1: TForm1
  Left = 192
  Top = 114
  Width = 387
  Height = 92
  Caption = 'BASS full-duplex recording test with effects'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 160
    Top = 32
    Width = 57
    Height = 17
  end
  object Label1: TLabel
    Left = 168
    Top = 8
    Width = 44
    Height = 13
    Caption = 'Latency :'
  end
  object Label2: TLabel
    Left = 184
    Top = 33
    Width = 6
    Height = 13
    Alignment = taCenter
    Caption = '0'
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 16
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
    OnChange = ComboBox1Change
  end
  object CheckBox1: TCheckBox
    Left = 312
    Top = 32
    Width = 65
    Height = 17
    Caption = 'Reverb'
    TabOrder = 1
    OnClick = CheckBox1Click
  end
  object CheckBox2: TCheckBox
    Left = 232
    Top = 8
    Width = 65
    Height = 17
    Caption = 'Chorus'
    TabOrder = 2
    OnClick = CheckBox2Click
  end
  object CheckBox3: TCheckBox
    Left = 312
    Top = 8
    Width = 65
    Height = 17
    Caption = 'Flanger'
    TabOrder = 3
    OnClick = CheckBox3Click
  end
  object CheckBox4: TCheckBox
    Left = 232
    Top = 32
    Width = 65
    Height = 17
    Caption = 'Gargle'
    TabOrder = 4
    OnClick = CheckBox4Click
  end
  object TrackBar1: TTrackBar
    Left = 8
    Top = 40
    Width = 145
    Height = 17
    Max = 100
    TabOrder = 5
    ThumbLength = 10
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = TrackBar1Change
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 250
    OnTimer = Timer1Timer
    Left = 264
    Top = 8
  end
end
