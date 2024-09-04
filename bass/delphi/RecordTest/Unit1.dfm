object Form1: TForm1
  Left = 193
  Top = 105
  Width = 336
  Height = 90
  Caption = 'BASS recording example'
  Color = clBtnFace
  Constraints.MaxHeight = 90
  Constraints.MinHeight = 90
  Constraints.MinWidth = 336
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  DesignSize = (
    328
    61)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 3
    Top = 30
    Width = 118
    Height = 13
    Alignment = taCenter
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
  end
  object Bevel1: TBevel
    Left = 127
    Top = 35
    Width = 211
    Height = 20
    Anchors = [akTop, akRight]
  end
  object lPos: TLabel
    Left = 135
    Top = 39
    Width = 196
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 8
    Width = 112
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 0
    OnChange = ComboBox1Change
  end
  object bRecord: TButton
    Left = 127
    Top = 8
    Width = 90
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Record'
    TabOrder = 1
    OnClick = bRecordClick
  end
  object bPlay: TButton
    Left = 224
    Top = 8
    Width = 53
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Play'
    TabOrder = 2
    OnClick = bPlayClick
  end
  object TrackBar1: TTrackBar
    Left = 6
    Top = 45
    Width = 115
    Height = 15
    Anchors = [akLeft, akTop, akRight]
    Max = 100
    TabOrder = 3
    ThumbLength = 9
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = TrackBar1Change
  end
  object bSave: TButton
    Left = 284
    Top = 8
    Width = 53
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Save'
    TabOrder = 4
    OnClick = bSaveClick
  end
  object PosTimer: TTimer
    Interval = 200
    OnTimer = PosTimerTimer
    Left = 120
    Top = 32
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'wav'
    Filter = 'WAV files|*.wav|All files|*.*'
    Title = 'Save WAV...'
    Left = 152
    Top = 32
  end
end
