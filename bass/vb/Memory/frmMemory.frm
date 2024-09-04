VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMemory 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "(: JOBnik! :) - Playing from Memory"
   ClientHeight    =   3300
   ClientLeft      =   45
   ClientTop       =   360
   ClientWidth     =   4215
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3300
   ScaleWidth      =   4215
   StartUpPosition =   2  'CenterScreen
   Begin VB.CheckBox chkSYNC 
      Caption         =   "SYNC @ END {will show an API MessageBox}"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   3000
      Width           =   3975
   End
   Begin VB.Frame Frame1 
      Height          =   3015
      Left            =   0
      TabIndex        =   0
      Top             =   -80
      Width           =   4215
      Begin VB.CommandButton cmdOpenPlay 
         Caption         =   "Click here to open a file && play it"
         Height          =   495
         Left            =   120
         TabIndex        =   1
         Top             =   1440
         Width           =   3975
      End
      Begin VB.Timer tmrBASS 
         Enabled         =   0   'False
         Interval        =   100
         Left            =   2880
         Top             =   840
      End
      Begin MSComDlg.CommonDialog cmd 
         Left            =   3480
         Top             =   840
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin VB.Label lblBitsPS 
         AutoSize        =   -1  'True
         Caption         =   "Kbp/s:"
         Height          =   195
         Left            =   120
         TabIndex        =   10
         Top             =   2760
         Width           =   480
      End
      Begin VB.Label lblBPS 
         AutoSize        =   -1  'True
         Caption         =   "Bytes/s:"
         Height          =   195
         Left            =   120
         TabIndex        =   9
         Top             =   2520
         Width           =   585
      End
      Begin VB.Label lblFreq 
         AutoSize        =   -1  'True
         Caption         =   "Frequency:"
         Height          =   195
         Left            =   120
         TabIndex        =   8
         Top             =   2280
         Width           =   795
      End
      Begin VB.Label lblDXVer 
         AutoSize        =   -1  'True
         Caption         =   "DX Version:"
         Height          =   195
         Left            =   120
         TabIndex        =   7
         Top             =   2040
         Width           =   840
      End
      Begin VB.Label lblFilePath 
         AutoSize        =   -1  'True
         Caption         =   "File:"
         Height          =   195
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   285
      End
      Begin VB.Label lblDur 
         AutoSize        =   -1  'True
         Caption         =   "Total Duration: 0.0 seconds / 00:00:00"
         Height          =   195
         Left            =   120
         TabIndex        =   4
         Top             =   600
         Width           =   2760
      End
      Begin VB.Label lblPos 
         AutoSize        =   -1  'True
         Caption         =   "Playing Position: 0.0 seconds"
         Height          =   195
         Left            =   120
         TabIndex        =   3
         Top             =   840
         Width           =   2070
      End
      Begin VB.Label lblMins 
         AutoSize        =   -1  'True
         Caption         =   "Time: 00:00:00"
         Height          =   195
         Left            =   120
         TabIndex        =   2
         Top             =   1080
         Width           =   1065
      End
   End
End
Attribute VB_Name = "frmMemory"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'///////////////////////////////////////////////////////////////////////////////
' frmMemory.frm - Copyright (c) 2001-2006 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                       [http://www.jobnik.org]
'                                                       [  jobnik@jobnik.org  ]
'
' Other sources: CBASS_TIME.cls & SYNCtest.bas
'
' (: JOBnik! :) - Playing from Memory
' * Updates:
'    . Now uses only VB functions without any Memory APIs
'    . Threading
' * Based on 'C' example by Ian Luck
'///////////////////////////////////////////////////////////////////////////////

Option Explicit

Private Declare Function GetModuleFileName Lib "kernel32" Alias "GetModuleFileNameA" (ByVal hModule As Long, ByVal lpFileName As String, ByVal nSize As Long) As Long

Private Sub Form_Initialize()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    ChDrive App.Path
    ChDir App.Path

    ' check the correct BASS was loaded
    If (HiWord(BASS_GetVersion) <> BASSVERSION) Then
        Call MsgBox("An incorrect version of BASS.DLL was loaded", vbCritical)
        End
    End If

    ' Start digital output
    If (BASS_Init(-1, 44100, 0, Me.hwnd, 0) = 0) Then
        Call Error_("Couldn't Initialize Digital Output")
        End
    End If

    Set bassTime = New cbass_time
    lblDXVer.Caption = "DX Version: " & bassTime.GetDXver

    cthread = 0
End Sub

' this function will check if you're running in IDE or EXE modes
' VB will crash if you're closing the app while (cthread<>0) in IDE,
' but won't crash if in EXE mode
Public Function isIDEmode() As Boolean
    Dim sFileName As String, lCount As Long

    sFileName = String(255, 0)
    lCount = GetModuleFileName(App.hInstance, sFileName, 255)
    sFileName = UCase(GetFileName(Mid(sFileName, 1, lCount)))

    isIDEmode = (sFileName = "VB6.EXE")
End Function

Private Sub Form_Unload(Cancel As Integer)
    If (isIDEmode And cthread) Then
        ' IDE Version
        Cancel = True   ' disable closing app to avoid crash
    Else
        ' Compiled Version or (cthread = 0) close app is available
        ' free it all
        Call BASS_Free
        Erase DataStore()
        Set bassTime = Nothing
        End
    End If
End Sub

Private Sub cmdOpenPlay_Click()
    On Local Error Resume Next          ' if Cancel was pressed

    If (cthread) Then   ' already creating
        Call Beep
    Else
        cmd.filename = ""
        cmd.CancelError = True
        cmd.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
        cmd.Filter = "playable files|*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.mp1;*.mp2;*.mp3;*.wav;*.ogg;*.aif|All files|*.*"
        cmd.ShowOpen

        ' if cancel was pressed, exit sub
        If Err.Number = 32755 Then Exit Sub

        tmrBASS.Enabled = False

        lblFilePath.Caption = "File: " & GetFileName(cmd.filename)
        cmdOpenPlay.Caption = "Loading file..."

        ' make a new thread, copy file into memory and play it :)
        Dim threadid As Long

        ' open file for reading
        Open cmd.filename For Binary As #100

        cthread = CreateThread(ByVal 0&, 0, AddressOf MemoryFileThread, FileLen(cmd.filename), 0, threadid)   ' threadid param required on win9x
    End If
End Sub

Public Sub chkSYNC_Click()
    If chkSYNC.value = vbChecked Then
        SyncEnd = BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, AddressOf SYNCtest.SyncEndTest, 0)
    Else
        Call BASS_ChannelRemoveSync(chan, SyncEnd)
    End If
End Sub

Private Sub tmrBASS_Timer()
    With bassTime
        lblPos.Caption = "Playing Position: " & Format(.GetPlayingPos(chan), "0.0") & " seconds"
        lblMins.Caption = "Time: " & .GetTime(.GetDuration(chan) - .GetPlayingPos(chan))
    End With
End Sub

'--------------------
' useful function :)
'--------------------

' get file name from file path
Public Function GetFileName(ByVal fp As String) As String
    GetFileName = Mid(fp, InStrRev(fp, "\") + 1)
End Function
