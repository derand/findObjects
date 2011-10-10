unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, FileCtrl, ShellAPI;

type
  TForm1 = class(TForm)
    pathEdit: TEdit;
    pathBtn: TButton;
    lbl1: TLabel;
    lbl2: TLabel;
    dayEdit: TEdit;
    lbl3: TLabel;
    startTimeEdit: TEdit;
    lbl4: TLabel;
    endTimeEdit: TEdit;
    lbl5: TLabel;
    magEdit: TEdit;
    lbl6: TLabel;
    umEdit: TEdit;
    lbl7: TLabel;
    raRateMinEdit: TEdit;
    lbl8: TLabel;
    raRateMaxEdit: TEdit;
    lbl9: TLabel;
    declRateMinEdit: TEdit;
    lbl10: TLabel;
    declRateMaxEdit: TEdit;
    lbl11: TLabel;
    blackListEdit: TEdit;
    Label1: TLabel;
    raPositionMinEdit: TEdit;
    lbl12: TLabel;
    raPositionMaxEdit: TEdit;
    runButton: TButton;
    procedure FormShow(Sender: TObject);
    procedure pathBtnClick(Sender: TObject);
    procedure runButtonClick(Sender: TObject);
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
                  dayEdit.Text := val;
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
    Writeln(settingsfile, 'EFEM_DIR	= '+pathEdit.text);
    Writeln(settingsfile, 'DAY = '+dayEdit.text);
    Writeln(settingsfile, 'START_OBSERVATION	= '+startTimeEdit.text);
    Writeln(settingsfile, 'END_OBSERVATION	= '+endTimeEdit.text);
    Writeln(settingsfile, 'MAG	= '+magEdit.text);
    Writeln(settingsfile, 'UM	= '+umEdit.text);
    Writeln(settingsfile, 'RA_RATE_MIN	= '+rarateMinEdit.text);
    Writeln(settingsfile, 'RA_RATE_MAX	= '+raRateMaxEdit.text);
    Writeln(settingsfile, 'DECL_RATE_MIN	= '+declRateMinEdit.text);
    Writeln(settingsfile, 'DECL_RATE_MAX	= '+declRateMaxEdit.text);
    Writeln(settingsfile, 'BLACK_LIST	= '+blackListEdit.text);
    Writeln(settingsfile, 'RA_POSITION_MIN	= '+raPositionMinEdit.text);
    Writeln(settingsfile, 'RA_POSITION_MAX	= '+raPositionMaxEdit.text);
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

procedure TForm1.pathBtnClick(Sender: TObject);
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

procedure TForm1.runButtonClick(Sender: TObject);
var
  cmd: string;
begin
    saveSettings();
    ShellExecute(Self.Handle, 'open', PChar('findObjects.exe'), PChar('"'+settingsFileName+'"'), nil, SW_SHOWNORMAL)
end;

end.
