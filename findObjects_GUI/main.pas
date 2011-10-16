unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, FileCtrl, ShellAPI, ExtCtrls, jpeg, ComCtrls;

type
  TForm1 = class(TForm)
    backgroundImage: TImage;
    lbl1: TLabel;
    lbl2: TLabel;
    lbl3: TLabel;
    lbl4: TLabel;
    lbl5: TLabel;
    lbl6: TLabel;
    lbl7: TLabel;
    lbl8: TLabel;
    lbl9: TLabel;
    lbl10: TLabel;
    lbl11: TLabel;
    lbl13: TLabel;
    lbl12: TLabel;
    pathEdit: TEdit;
    btnBtn: TButton;
    startTimeEdit: TEdit;
    endTimeEdit: TEdit;
    magEdit: TEdit;
    umEdit: TEdit;
    raRateMinEdit: TEdit;
    raRateMaxEdit: TEdit;
    declRateMinEdit: TEdit;
    declRateMaxEdit: TEdit;
    blackListEdit: TEdit;
    raPositionMinEdit: TEdit;
    raPositionMaxEdit: TEdit;
    btnButton: TButton;
    dayEdit: TEdit;
    versionLabel: TLabel;
    copyrightLabel: TLabel;
    procedure FormShow(Sender: TObject);
    procedure btnBtnClick(Sender: TObject);
    procedure btnButtonClick(Sender: TObject);
  private
    { Private declarations }

    settingsFileName : string;
    selfExePath  : string;

    procedure loadSettings();
    procedure saveSettings();
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.loadSettings();
var
  settingsFile : TextFile;
  line : string;
  p: integer;
  key, val : string;
begin
    Form1.pathEdit.Text := 'asdfs';
    AssignFile(settingsFile, settingsFileName);
    Reset(settingsFile);
    while not EOF(settingsFile) do
    begin
      Readln(settingsFile, line);
      if (Length(line) > 2) and (line[1] <> '#') then
      begin
 //         ShowMessage(line);
          p := pos('=', line);
          if p <> 0 then
          begin
              key := Copy(line, 0, p-1);
              val := Copy(line, p+1, Length(line)-p);
              key := Trim(key);
              val := Trim(val);
              if AnsiCompareText(key, 'EFEM_DIR')=0 then
              begin
                 pathEdit.Text := val;
              end else if AnsiCompareText(key, 'DAY')=0 then
              begin
                  dayEdit.text := val;
              end else if AnsiCompareText(key, 'START_OBSERVATION')=0 then
              begin
                  startTimeEdit.Text := val;
              end else if AnsiCompareText(key, 'END_OBSERVATION')=0 then
              begin
                  endTimeEdit.Text := val;
              end else if AnsiCompareText(key, 'MAG')=0 then
              begin
                  magEdit.Text := val;
              end else if AnsiCompareText(key, 'UM')=0 then
              begin
                  umEdit.Text := val;
              end else if AnsiCompareText(key, 'RA_RATE_MIN')=0 then
              begin
                  raRateMinEdit.Text := val;
              end else if AnsiCompareText(key, 'RA_RATE_MAX')=0 then
              begin
                  raRateMaxEdit.Text := val;
              end else if AnsiCompareText(key, 'DECL_RATE_MIN')=0 then
              begin
                  declRateMinEdit.Text := val;
              end else if AnsiCompareText(key, 'DECL_RATE_MAX')=0 then
              begin
                  declRateMaxEdit.Text := val;
              end else if AnsiCompareText(key, 'BLACK_LIST')=0 then
              begin
                  blackListEdit.Text := val;
              end else if AnsiCompareText(key, 'RA_POSITION_MIN')=0 then
              begin
                  raPositionMinEdit.Text := val;
              end else if AnsiCompareText(key, 'RA_POSITION_MAX')=0 then
              begin
                  raPositionMaxEdit.Text := val;
              end else if AnsiCompareText(key, 'REPORT_TYPE')=0 then
              begin
              end else if AnsiCompareText(key, 'REPORT_HTML_FONT_SIZE')=0 then
              begin
              end;

 //             ShowMessage(key+'-+-'+val);
          end;
      end;
 //     ShowMessage(line);
    end;
    CloseFile(settingsFile);
end;


procedure tform1.saveSettings();
var
  settingsFile : TextFile;
begin
    AssignFile(settingsFile, settingsFileName);
    Rewrite(settingsFile);
    if Length(Trim(pathEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'EFEM_DIR	= '+pathEdit.text);
    end;
    if Length(Trim(dayEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'DAY = '+dayEdit.text);
    end;
    if Length(Trim(startTimeEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'START_OBSERVATION	= '+startTimeEdit.text);
    end;
    if Length(Trim(endTimeEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'END_OBSERVATION	= '+endTimeEdit.text);
    end;
    if Length(Trim(magEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'MAG	= '+magEdit.text);
    end;
    if Length(Trim(umEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'UM	= '+umEdit.text);
    end;
    if Length(Trim(rarateMinEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'RA_RATE_MIN	= '+rarateMinEdit.text);
    end;
    if Length(Trim(raRateMaxEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'RA_RATE_MAX	= '+raRateMaxEdit.text);
    end;
    if Length(Trim(declRateMinEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'DECL_RATE_MIN	= '+declRateMinEdit.text);
    end;
    if Length(Trim(declRateMaxEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'DECL_RATE_MAX	= '+declRateMaxEdit.text);
    end;
    if Length(Trim(blackListEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'BLACK_LIST	= '+blackListEdit.text);
    end;
    if Length(Trim(raPositionMinEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'RA_POSITION_MIN	= '+raPositionMinEdit.text);
    end;
    if Length(Trim(raPositionMaxEdit.Text)) > 0 then
    begin
      Writeln(settingsfile, 'RA_POSITION_MAX	= '+raPositionMaxEdit.text);
    end;
    Writeln(settingsfile);
    Writeln(settingsfile, '#not changed');
    Writeln(settingsfile, 'REPORT_TYPE = TXT');
    Writeln(settingsfile, 'REPORT_HTML_FONT_SIZE = 10pt');
    CloseFile(settingsFile);
end;

procedure TForm1.FormShow(Sender: TObject);
begin
    settingsFileName := 'config_GUI.txt';
    selfExePath := GetCurrentDir;
    loadSettings();
end;

procedure TForm1.btnBtnClick(Sender: TObject);
const
  SELDIRHELP  = 1000;
var
  dir: String;
begin
  dir := '';
  if SelectDirectory(dir, [sdAllowCreate, sdPerformCreate, sdPrompt], SELDIRHELP) then
  begin
      pathEdit.Text := dir+'\';
      SetCurrentDir(selfExePath);
  end;
end;

procedure TForm1.btnButtonClick(Sender: TObject);
var
  cmd: string;
begin
    saveSettings();
    ShellExecute(Self.Handle, 'open', PChar('findObjects.exe'), PChar('"'+settingsFileName+'"'), nil, SW_SHOWNORMAL)
end;

end.
