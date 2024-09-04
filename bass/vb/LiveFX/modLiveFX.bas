Attribute VB_Name = "modLiveFX"
'///////////////////////////////////////////////////////////////////////////////
' modLiveFX.bas - Copyright (c) 2002-2006 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                       [http://www.jobnik.org]
'                                                       [  jobnik@jobnik.org  ]
'
' Other source: frmLiveFX.frm
'
' BASS full-duplex recording test with effects
' Originally translated from - livefx.c - Example of Ian Luck
'///////////////////////////////////////////////////////////////////////////////

Option Explicit

Declare Sub FillMemory Lib "kernel32.dll" Alias "RtlFillMemory" (Destination As Any, ByVal length As Long, ByVal Fill As Byte)

Public rchan As Long    ' recording channel
Public chan As Long     ' playback stream
Public fx(4) As Long    ' FX handles
Public chunk As Long    ' recording chunk size
Public input_ As Long   ' current input source
Public latency As Long  ' current latency

' Display error message
Public Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, vbExclamation, "Error")
End Sub

' stream function - play the recording data
Public Function stream(ByVal handle As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long) As Long
    Dim c As Long
    ' check how much recorded data is buffered
    c = BASS_ChannelGetData(rchan, ByVal 0&, BASS_DATA_AVAILABLE)
    c = c - length
    If (c > 2 * chunk + 1764) Then ' buffer has gotten pretty large so remove some
        c = c - chunk   ' leave a single 'chunk'
        Call BASS_ChannelGetData(rchan, ByVal 0&, ByVal c)  ' remove it
     End If
     ' fetch recorded data into stream
     c = BASS_ChannelGetData(rchan, ByVal buffer, length)
     If (c < length) Then Call FillMemory(ByVal buffer + c, length - c, 0)  ' short of data
     stream = length
End Function

Static Function Initialize() As Boolean
    Dim i As BASS_INFO

    ' 10ms update period
    Call BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 10)

    ' setup output - get device latency
    If (BASS_Init(-1, 44100, BASS_DEVICE_LATENCY, frmLiveFX.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device")
        Initialize = False
        Exit Function
    End If

    Call BASS_GetInfo(i)
    ' buffer size = update period + 'minbuf'
    Call BASS_SetConfig(BASS_CONFIG_BUFFER, 10 + i.minbuf)

    If (i.dsver < 8) Then ' no DX8, so disable effect buttons
        With frmLiveFX
            .chkChorus.Enabled = False
            .chkFlanger.Enabled = False
            .chkGargle.Enabled = False
            .chkReverb.Enabled = False
        End With
    End If

    ' start recording - default device, 44100hz, stereo, 16 bits, no callback function
    Dim rinit As Long
    rinit = BASS_RecordInit(-1)
    rchan = BASS_RecordStart(44100, 2, 0, 0, 0)
    If (rinit = 0 Or rchan = 0) Then
        Call BASS_Free
        Call Error_("Can't initialize recording device")
        Initialize = False
        Exit Function
    End If
    
    ' wait for recorded data to start arriving (calculate the latency)
    Do
        chunk = BASS_ChannelGetData(rchan, 0, BASS_DATA_AVAILABLE)
    Loop While chunk = 0

    ' create a stream to play the recording data, and start it
    chan = BASS_StreamCreate(44100, 2, 0, AddressOf stream, 0)
    Call BASS_ChannelPlay(chan, BASSFALSE)
 
    ' get list of inputs
    Dim c As Integer
    While BASS_RecordGetInputName(c) <> 0
        frmLiveFX.cmbSelChange.AddItem VBStrFromAnsiPtr(BASS_RecordGetInputName(c))
        If (BASS_RecordGetInput(c) And BASS_INPUT_OFF) = 0 Then
            frmLiveFX.cmbSelChange.ListIndex = c  ' this 1 is currently "on"
            input_ = c
            frmLiveFX.sLevel.SelStart = LoWord(BASS_RecordGetInput(input_))  ' get level
        End If
        c = c + 1
    Wend

    Initialize = True
End Function
