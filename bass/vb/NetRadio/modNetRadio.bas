Attribute VB_Name = "modNetRadio"
'/////////////////////////////////////////////////////////////////////////////////
' modNetRadio.bas - Copyright (c) 2002-2006 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                         [http://www.jobnik.org]
'                                                         [  jobnik@jobnik.org  ]
'
' * Save local copy is added by: Peter Hebels @ http://www.phsoft.nl
'                                             e-mail: info@phsoft.nl
'
' Other sources: frmNetRadio.frm & clsFileIo.cls
'
' BASS Internet radio example
' Originally translated from - netradio.c - Example of Ian Luck
'/////////////////////////////////////////////////////////////////////////////////

Option Explicit

Public chan As Long
Public url As Variant
Public TmpNameHold As String
Public TmpNameHold2 As String

Public proxy(100) As Byte ' proxy server

' SAVE LOCAL COPY
Public WriteFile As clsFileIo
Public FileIsOpen As Boolean, GotHeader As Boolean
Public DownloadStarted As Boolean, DoDownload As Boolean
Public DlOutput As String, SongNameUpdate As Boolean

' THREADING
Public cthread As Long
Public Declare Function CreateThread Lib "kernel32" (lpThreadAttributes As Any, ByVal dwStackSize As Long, ByVal lpStartAddress As Long, ByVal lpParameter As Long, ByVal dwCreationFlags As Long, lpThreadID As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

' MESSAGE BOX
Public Declare Function MessageBox Lib "user32" Alias "MessageBoxA" (ByVal hwnd As Long, ByVal lpText As String, ByVal lpCaption As String, ByVal wType As Long) As Long

Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

' display error message
Public Sub Error_(ByVal es As String)
    Call MessageBox(frmNetRadio.hwnd, es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, "Error", vbExclamation)
End Sub

' update stream title from metadata
Sub DoMeta(ByVal meta As Long)
    Dim p As String, tmpMeta As String
    If meta = 0 Then Exit Sub
    tmpMeta = VBStrFromAnsiPtr(meta)
    If ((Mid(tmpMeta, 1, 13) = "StreamTitle='")) Then
        p = Mid(tmpMeta, 14)
        TmpNameHold = Mid(p, 1, InStr(p, ";") - 2)
        frmNetRadio.lblSong.Caption = TmpNameHold
        
        If TmpNameHold = TmpNameHold2 Then
            ' do noting
        Else
            TmpNameHold2 = TmpNameHold
            GotHeader = False
            DownloadStarted = False
        End If
        
        DlOutput = App.Path & "\" & RemoveSpecialChar(Mid(p, 1, InStr(p, ";") - 2)) & ".mp3"
    End If
End Sub

Sub MetaSync(ByVal handle As Long, ByVal channel As Long, ByVal data As Long, ByVal user As Long)
    Call DoMeta(data)
End Sub

Public Sub OpenURL(ByVal clkURL As Long)
    With frmNetRadio
        Call BASS_StreamFree(chan) ' close old stream
        .lblName.Caption = "connecting..."
        .lblBPS.Caption = ""
        .lblSong.Caption = ""

        chan = BASS_StreamCreateURL(CStr(url((IIf(clkURL < 5, clkURL * 2, (clkURL * 2) - 9)))), 0, BASS_STREAM_STATUS, AddressOf SUBDOWNLOADPROC, 0)

        If chan = 0 Then
            .lblName.Caption = "not playing"
            Call Error_("Can't play the stream")
        Else
            Do
                Dim progress As Long, len_ As Long
                len_ = BASS_StreamGetFilePosition(chan, BASS_FILEPOS_END)
                If (len_ = -1) Then GoTo done ' something's gone wrong! (eg. BASS_Free called)
                progress = (BASS_StreamGetFilePosition(chan, BASS_FILEPOS_DOWNLOAD) _
                    - BASS_StreamGetFilePosition(chan, BASS_FILEPOS_CURRENT)) * 100 / len_ ' percentage of buffer filled
                If (progress > 75) Then Exit Do ' over 75% full, enough
                .lblName.Caption = "buffering... " & progress & "%"
                Call Sleep(50)
            Loop While 1

            Dim icyPTR As Long  ' a pointer where ICY info is stored
            Dim tmpICY As String

            ' get the broadcast name and bitrate
            icyPTR = BASS_ChannelGetTags(chan, BASS_TAG_ICY)
            If icyPTR = 0 Then
                icyPTR = BASS_ChannelGetTags(chan, BASS_TAG_HTTP) ' no ICY tags, try HTTP
            End If
            If (icyPTR) Then
                Do
                    tmpICY = VBStrFromAnsiPtr(icyPTR)
                    icyPTR = icyPTR + Len(tmpICY) + 1
                    .lblName.Caption = IIf(Mid(tmpICY, 1, 9) = "icy-name:", Mid(tmpICY, 10), .lblName.Caption)
                    .lblBPS.Caption = IIf(Mid(tmpICY, 1, 7) = "icy-br:", "bitrate: " & Mid(tmpICY, 8), .lblBPS.Caption)

                    ' NOTE: you can get more ICY info like: icy-genre:, icy-url:... :)
                Loop While (tmpICY <> "")
            End If

            ' get the stream title and set sync for subsequent titles
            Call DoMeta(BASS_ChannelGetTags(chan, BASS_TAG_META))
            Call BASS_ChannelSetSync(chan, BASS_SYNC_META, 0, AddressOf MetaSync, 0)

            ' play it!
            Call BASS_ChannelPlay(chan, BASSFALSE)
        End If
    End With

done:
    Call CloseHandle(cthread)   ' close the thread
    cthread = 0
End Sub

' The following functions where added by Peter Hebels
Public Sub SUBDOWNLOADPROC(ByVal buffer As Long, ByVal length As Long, ByVal user As Long)
    If (buffer And length = 0) Then
        frmNetRadio.lblBPS.Caption = VBStrFromAnsiPtr(buffer) ' display connection status
        Exit Sub
    End If

    If (Not DoDownload) Then
        DownloadStarted = False
        Call WriteFile.CloseFile
        Exit Sub
    End If

    If (Trim(DlOutput) = "") Then Exit Sub

    If (Not DownloadStarted) Then
        DownloadStarted = True
        Call WriteFile.CloseFile
        If (WriteFile.OpenFile(DlOutput)) Then
            SongNameUpdate = False
        Else
            
            SongNameUpdate = True
            
            GotHeader = False
        End If
    End If

    If (Not SongNameUpdate) Then
        If (length) Then
            Call WriteFile.WriteBytes(buffer, length)
        Else
            Call WriteFile.CloseFile
            GotHeader = False
        End If
    Else
        DownloadStarted = False
        Call WriteFile.CloseFile
        GotHeader = False
    End If
End Sub

Public Function RemoveSpecialChar(strFileName As String)
    Dim i As Byte
    Dim SpecialChar As Boolean
    Dim SelChar As String, OutFileName As String

    For i = 1 To Len(strFileName)
        SelChar = Mid(strFileName, i, 1)
        SpecialChar = InStr(":/\?*|<>" & Chr$(34), SelChar) > 0

        If (Not SpecialChar) Then
            OutFileName = OutFileName & SelChar
            SpecialChar = False
        Else
            OutFileName = OutFileName
            SpecialChar = False
        End If
    Next i

    RemoveSpecialChar = OutFileName
End Function
