object Form1: TForm1
  Left = 120
  Top = 119
  Width = 409
  Height = 389
  Caption = 'findObjects_GUI'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lbl1: TLabel
    Left = 8
    Top = 12
    Width = 75
    Height = 13
    Caption = #1055#1091#1090#1100' '#1082' '#1092#1072#1081#1083#1072#1084
  end
  object lbl2: TLabel
    Left = 8
    Top = 36
    Width = 93
    Height = 13
    Caption = #1044#1077#1085#1100' '#1085#1072#1073#1083#1102#1076#1077#1085#1080#1081
  end
  object lbl3: TLabel
    Left = 8
    Top = 60
    Width = 104
    Height = 13
    Caption = #1053#1072#1095#1072#1083#1086' '#1085#1072#1073#1083#1102#1076#1077#1085#1080#1081
  end
  object lbl4: TLabel
    Left = 8
    Top = 84
    Width = 129
    Height = 13
    Caption = #1047#1072#1074#1077#1088#1096#1077#1085#1080#1077' '#1085#1072#1073#1083#1102#1076#1077#1085#1080#1081
  end
  object lbl5: TLabel
    Left = 8
    Top = 108
    Width = 20
    Height = 13
    Caption = 'Mag'
  end
  object lbl6: TLabel
    Left = 8
    Top = 132
    Width = 15
    Height = 13
    Caption = 'UM'
  end
  object lbl7: TLabel
    Left = 8
    Top = 156
    Width = 71
    Height = 13
    Caption = 'RA_RATE_MIN'
  end
  object lbl8: TLabel
    Left = 8
    Top = 180
    Width = 70
    Height = 13
    Caption = 'RA_Rate_MAX'
  end
  object lbl9: TLabel
    Left = 8
    Top = 204
    Width = 82
    Height = 13
    Caption = 'DECL_RATE_MIN'
  end
  object lbl10: TLabel
    Left = 8
    Top = 228
    Width = 84
    Height = 13
    Caption = 'DECL_RATE_MAX'
  end
  object lbl11: TLabel
    Left = 8
    Top = 252
    Width = 58
    Height = 13
    Caption = 'BLACK_LIST'
  end
  object Label1: TLabel
    Left = 8
    Top = 276
    Width = 94
    Height = 13
    Caption = 'RA_POSITION_MIN'
  end
  object lbl12: TLabel
    Left = 8
    Top = 300
    Width = 96
    Height = 13
    Caption = 'RA_POSITION_MAX'
  end
  object pathEdit: TEdit
    Left = 88
    Top = 8
    Width = 257
    Height = 21
    TabOrder = 0
    Text = 'pathEdit'
  end
  object pathBtn: TButton
    Left = 352
    Top = 8
    Width = 25
    Height = 25
    Caption = '...'
    TabOrder = 1
    OnClick = pathBtnClick
  end
  object dayEdit: TEdit
    Left = 160
    Top = 32
    Width = 185
    Height = 21
    TabOrder = 2
    Text = 'pathEdit'
  end
  object startTimeEdit: TEdit
    Left = 160
    Top = 56
    Width = 185
    Height = 21
    TabOrder = 3
    Text = 'pathEdit'
  end
  object endTimeEdit: TEdit
    Left = 160
    Top = 80
    Width = 185
    Height = 21
    TabOrder = 4
    Text = 'pathEdit'
  end
  object magEdit: TEdit
    Left = 120
    Top = 104
    Width = 225
    Height = 21
    TabOrder = 5
    Text = 'pathEdit'
  end
  object umEdit: TEdit
    Left = 120
    Top = 128
    Width = 225
    Height = 21
    TabOrder = 6
    Text = 'pathEdit'
  end
  object raRateMinEdit: TEdit
    Left = 120
    Top = 152
    Width = 225
    Height = 21
    TabOrder = 7
    Text = 'pathEdit'
  end
  object raRateMaxEdit: TEdit
    Left = 120
    Top = 176
    Width = 225
    Height = 21
    TabOrder = 8
    Text = 'pathEdit'
  end
  object declRateMinEdit: TEdit
    Left = 120
    Top = 200
    Width = 225
    Height = 21
    TabOrder = 9
    Text = 'pathEdit'
  end
  object declRateMaxEdit: TEdit
    Left = 120
    Top = 224
    Width = 225
    Height = 21
    TabOrder = 10
    Text = 'pathEdit'
  end
  object blackListEdit: TEdit
    Left = 120
    Top = 248
    Width = 225
    Height = 21
    TabOrder = 11
    Text = 'pathEdit'
  end
  object raPositionMinEdit: TEdit
    Left = 120
    Top = 272
    Width = 225
    Height = 21
    TabOrder = 12
    Text = 'pathEdit'
  end
  object raPositionMaxEdit: TEdit
    Left = 120
    Top = 296
    Width = 225
    Height = 21
    TabOrder = 13
    Text = 'pathEdit'
  end
  object runButton: TButton
    Left = 272
    Top = 328
    Width = 75
    Height = 25
    Caption = #1055#1086#1089#1095#1080#1090#1072#1090#1100
    TabOrder = 14
    OnClick = runButtonClick
  end
end
