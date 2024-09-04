// BASS Full-duplex test
unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, BASS, StdCtrls, ExtCtrls, ComCtrls;

type
  TForm1 = class(TForm)
    Timer1: TTimer;
    ComboBox1: TComboBox;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    CheckBox3: TCheckBox;
    CheckBox4: TCheckBox;
    Bevel1: TBevel;
    Label1: TLabel;
    Label2: TLabel;
    TrackBar1: TTrackBar;
    procedure ComboBox1Change(Sender: TObject);
    procedure CheckBox1Click(Sender: TObject);
    procedure CheckBox2Click(Sender: TObject);
    procedure CheckBox3Click(Sender: TObject);
    procedure CheckBox4Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure TrackBar1Change(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  fx : array [1..4] of HFX;
  chan : HRECORD;  // recording channel
  pchan : HSTREAM;  // playback stream
  length : integer;  // FX handles
  latency : extended; // Current latency
  chunk : integer;


implementation

{$R *.dfm}

function stream(handle: HSTREAM; buffer: Pointer; length: DWORD; user: DWORD): Cardinal; stdcall;
var
  c : integer;
begin
c := BASS_ChannelGetData(chan, 0, BASS_DATA_AVAILABLE);
if (c > (2 * (chunk + 1764))) then
  BASS_ChannelGetData(chan, 0, c);
c := BASS_ChannelGetData(chan, buffer, length);

Result := c;
end;

procedure TForm1.ComboBox1Change(Sender: TObject);
var
	i: Integer;
    r: Boolean;
begin
	// enable the selected input
    r := True;
    i := 0;
    // first disable all inputs, then...
	while r do
    begin
		r := BASS_RecordSetInput(i, BASS_INPUT_OFF);
        Inc(i);
	end;
    // ...enable the selected.
	BASS_RecordSetInput(ComboBox1.ItemIndex, BASS_INPUT_ON);
end;

procedure TForm1.CheckBox1Click(Sender: TObject);
begin
if CheckBox1.Checked = true then
  fx[1] := BASS_ChannelSetFX(pchan,BASS_FX_REVERB,0)
else
  BASS_ChannelRemoveFX(pchan,fx[1]);
end;

procedure TForm1.CheckBox2Click(Sender: TObject);
begin
if CheckBox2.Checked = true then
  fx[2] := BASS_ChannelSetFX(pchan,BASS_FX_CHORUS,0)
else
  BASS_ChannelRemoveFX(pchan,fx[2]);
end;

procedure TForm1.CheckBox3Click(Sender: TObject);
begin
if CheckBox3.Checked = true then
  fx[3] := BASS_ChannelSetFX(pchan,BASS_FX_FLANGER,0)
else
  BASS_ChannelRemoveFX(pchan,fx[3]);
end;

procedure TForm1.CheckBox4Click(Sender: TObject);
begin
if CheckBox4.Checked = true then
  fx[4] := BASS_ChannelSetFX(pchan,BASS_FX_GARGLE,0)
else
  BASS_ChannelRemoveFX(pchan,fx[4]);
end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin
	latency :=(latency*3+BASS_ChannelGetData(pchan,0,BASS_DATA_AVAILABLE)
		+BASS_ChannelGetData(chan,0,BASS_DATA_AVAILABLE))/4;
	Label2.Caption := inttostr(round(latency*1000/176400));
end;

procedure TForm1.TrackBar1Change(Sender: TObject);
begin
	BASS_RecordSetInput(ComboBox1.ItemIndex, BASS_INPUT_LEVEL or TrackBar1.Position);
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  i : bass_info;
  a,b: Integer;
  dName: PChar;
begin
Application.MessageBox('Do not set the input to "WAVE" or "What you hear" (etc...) with the level set high, as that is likely to result in nasty feedback.',
				'Feedback warning',MB_ICONWARNING);


	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		MessageBox(0,'An incorrect version of BASS.DLL was loaded',0,MB_ICONERROR);
		Halt;
	end;
  Latency := 0;
	BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10);
	// setup output - get device latency
	if not BASS_Init(-1,44100,BASS_DEVICE_LATENCY,Application.Handle,nil) then
    begin
		  ShowMessage('Can''t initialize device');
		  Exit;
	  end;

	BASS_GetInfo(i);
	// buffer size = update period x2 + 'minbuf'
	BASS_SetConfig(BASS_CONFIG_BUFFER,20+i.minbuf);

  BASS_RecordInit(-1);
  chan := BASS_RecordStart(44100,2,0,nil,0); // Recording Channel
  pchan := BASS_StreamCreate(44100,2,0,@stream,0);  //Playing Channel

  BASS_ChannelPlay(pchan,False); // Start Playing
  Timer1.Enabled := True;  // Start the latency timer

  // Populate the list
	a := 0;
  dName := BASS_RecordGetInputName(a);
	while dName <> nil do
	begin
		ComboBox1.Items.Add(StrPas(dName));
		// is this one currently "on"?
		if (BASS_RecordGetInput(a) and BASS_INPUT_OFF) = 0 then
        	ComboBox1.ItemIndex := a;
		Inc(a);
		dName := BASS_RecordGetInputName(a);
	end;
    ComboBox1Change(Self);	// display info
	b := BASS_RecordGetInput(ComboBox1.ItemIndex);
	TrackBar1.Position := LoWord(b);	// set the level slider
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
BASS_Free();
end;

end.
