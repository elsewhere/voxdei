unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Bass, ExtCtrls;

const
  WM_INFO_UPDATE = WM_USER + 101;

type
  
  TForm1 = class(TForm)
    Panel1: TPanel;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button6: TButton;
    Button7: TButton;
    Button8: TButton;
    Button9: TButton;
    Button10: TButton;
    GroupBox2: TGroupBox;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    GroupBox3: TGroupBox;
    Label6: TLabel;
    ed_ProxyServer: TEdit;
    cbDirectConnection: TCheckBox;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure cbDirectConnectionClick(Sender: TObject);
  private
    { Private declarations }

  public
    { Public declarations }
    procedure WndProc(var Msg: TMessage); override;


  end;

var
  Form1: TForm1;
  Proxy: array [0..99] of char; //proxy server

  cthread: DWORD = 0;
  chan: HSTREAM = 0;
  win: hwnd;
implementation

const
  urls: array[0..9] of string = (
    'http://64.236.126.41/stream/1048', 'http://205.188.215.226:8000',
    'http://64.236.98.50/stream/1006', 'http://206.98.167.99:8406',
    'http://64.236.98.50/stream/1013', 'http://212.23.5.99:8002',
    'http://205.188.215.226:8012', 'http://205.188.215.226:8008',
    'http://207.200.96.225:8010', 'http://64.202.98.91:8082'
    );

{$R *.dfm}

  { display error messages }

procedure Error(es: string);
begin
  MessageBox(win, PChar(es + #13#10 + '(error code: ' + IntToStr(BASS_ErrorGetCode) +
    ')'), nil, 0);
end;

{ update stream title from metadata }

procedure DoMeta(meta: PChar);
var
  p: Integer;
begin
  if (meta <> nil) then
  begin
    p := Pos('StreamTitle=', meta);
    if (p = 0) then
      Exit;
    p := p + 13;
    SendMessage(win, WM_INFO_UPDATE, 7, DWORD(PChar(Copy(meta, p, Pos(';', meta) - p - 1))));
  end;
end;

procedure MetaSync(handle: HSYNC; channel, data, user: DWORD); stdcall;
begin
  DoMeta(PChar(data));
end;

procedure StatusProc(buffer: Pointer; len, user: DWORD); stdcall;
begin
  if (buffer <> nil) and (len = 0) then
    SendMessage(win, WM_INFO_UPDATE, 8, DWORD(PChar(buffer)));
end;

function OpenURL(url: PChar): Integer;
var
  icy: PChar;
  Len, Progress: DWORD;
begin
  Result := 0;
  BASS_StreamFree(chan); // close old stream
  progress := 0;
  SendMessage(win, WM_INFO_UPDATE, 0, 0); // reset the Labels and trying connecting

  chan := BASS_StreamCreateURL(url, 0, BASS_STREAM_STATUS, @StatusProc, 0);
  if (chan = 0) then
  begin

    SendMessage(win, WM_INFO_UPDATE, 1, 0); // Oops Error
  end
  else
  begin
    // Progress
    repeat
      len := BASS_StreamGetFilePosition(chan, BASS_FILEPOS_END);
      if (len = DW_Error) then
        break; // something's gone wrong! (eg. BASS_Free called)
      progress := (BASS_StreamGetFilePosition(chan, BASS_FILEPOS_DOWNLOAD) -
        BASS_StreamGetFilePosition(chan, BASS_FILEPOS_CURRENT)) * 100 div len;
      // percentage of buffer filled
      SendMessage(win, WM_INFO_UPDATE, 2, progress); // show the Progess value in the label

    until
      progress > 75;

    // get the broadcast name and bitrate
    icy := BASS_ChannelGetTags(chan, BASS_TAG_ICY);
    if (icy = nil) then
      icy := BASS_ChannelGetTags(chan, BASS_TAG_HTTP); // no ICY tags, try HTTP
    if (icy <> nil) then
      while (icy^ <> #0) do
      begin
        if (Copy(icy, 1, 9) = 'icy-name:') then
          SendMessage(win, WM_INFO_UPDATE, 3, DWORD(PChar(Copy(icy, 10, MaxInt))))

        else if (Copy(icy, 1, 7) = 'icy-br:') then

          SendMessage(win, WM_INFO_UPDATE, 4, DWORD(PChar('bitrate: ' + Copy(icy, 8, MaxInt))));
        icy := icy + Length(icy) + 1;
      end;
    // get the stream title and set sync for subsequent titles
    DoMeta(BASS_ChannelGetTags(chan, BASS_TAG_META));
    BASS_ChannelSetSync(chan, BASS_SYNC_META, 0, @MetaSync, 0);
    // play it!
    BASS_ChannelPlay(chan, FALSE);
  end;
  cthread := 0;
end;

                      
procedure TForm1.WndProc(var Msg: TMessage);
// to be threadsave we are passing all Canvas Stuff(e.g. Labels) to this messages
begin
  inherited;
  if Msg.Msg = WM_INFO_UPDATE then
    case msg.WParam of
      0:
        begin
          Label4.Caption := 'connecting...';
          Label3.Caption := '';
          Label5.Caption := '';
        end;
      1:
        begin
          Label4.Caption := 'not playing';
          Error('Can''t play the stream');
        end;
      2: Label4.Caption := Format('buffering... %d%%', [msg.LParam]);
      3: Label4.Caption := PChar(msg.LParam);
      4: Label5.Caption := PChar(msg.LParam);
      5: Label5.Caption := PChar(msg.LParam);
      6: Label3.Caption := PChar(msg.LParam);
      7: Label3.Caption := PChar(msg.LParam);
      8: Label5.Caption := PChar(msg.LParam);
    end;
end;                       

procedure TForm1.FormCreate(Sender: TObject);
begin
  // check the correct BASS was loaded
  win := handle;
  if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
  begin
    MessageBox(0, 'An incorrect version of BASS.DLL was loaded', nil, MB_ICONERROR);
    Halt;
  end;
  if (not BASS_Init(-1, 44100, 0, Handle, nil)) then
  begin
    Error('Can''t initialize device');
    Halt;
  end;
  BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 0);
  // minimize automatic pre-buffering, so we can do it (and display it) instead
  BASS_SetConfig(BASS_CONFIG_NET_PROXY,
   DWORD(StrPCopy(proxy,ed_ProxyServer.Text))); // setup proxy server location

end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  BASS_Free;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  ThreadId: Cardinal;
begin
  StrPCopy(proxy,ed_ProxyServer.Text); // copy the Servertext to the Proxy array
  if (cthread <> 0) then
    MessageBeep(0)
  else
    cthread := BeginThread(nil, 0, @OpenURL, PChar(urls[TButton(Sender).Tag]), 0, ThreadId);
end;

procedure TForm1.cbDirectConnectionClick(Sender: TObject);
begin
  if not TCheckbox(Sender).Checked then
    BASS_SetConfig(BASS_CONFIG_NET_PROXY, (DWORD(StrPCopy(proxy,ed_ProxyServer.Text)))) // enable proxy
  else
    BASS_SetConfig(BASS_CONFIG_NET_PROXY, 0); // disable proxy
end;

end.

