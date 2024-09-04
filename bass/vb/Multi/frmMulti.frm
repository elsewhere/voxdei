VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMulti 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS multiple output example"
   ClientHeight    =   1515
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5175
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1515
   ScaleWidth      =   5175
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton cmdSwap 
      Caption         =   "swap"
      Height          =   375
      Left            =   4320
      TabIndex        =   4
      Top             =   600
      Width           =   615
   End
   Begin VB.Frame frameMulti 
      Caption         =   " device 1 "
      Height          =   735
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   4935
      Begin VB.CommandButton cmdOpen 
         Caption         =   "click here to open a file..."
         Height          =   375
         Index           =   0
         Left            =   120
         TabIndex        =   1
         Top             =   240
         Width           =   3975
      End
   End
   Begin MSComDlg.CommonDialog cmd 
      Left            =   4440
      Top             =   960
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Frame frameMulti 
      Caption         =   " device 2 "
      Height          =   735
      Index           =   1
      Left            =   120
      TabIndex        =   2
      Top             =   720
      Width           =   4935
      Begin VB.CommandButton cmdOpen 
         Caption         =   "click here to open a file..."
         Height          =   375
         Index           =   1
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   3975
      End
   End
End
Attribute VB_Name = "frmMulti"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'//////////////////////////////////////////////////////////////////////////////
' frmMulti.frm - Copyright (c) 2003-2006 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                      [http://www.jobnik.org]
'                                                      [  jobnik@jobnik.org  ]
' Other source: frmDevice.frm
'
' BASS Multiple output example
' Originally translated from - multi.c - Example of Ian Luck
'//////////////////////////////////////////////////////////////////////////////
 
Option Explicit

Dim outdev(2) As Long   ' output devices
Dim chan(2) As Long     ' the streams

' display error messages
Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "Error Code : " & BASS_ErrorGetCode, vbExclamation, "Error")
End Sub

Private Sub Form_Load()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    ChDrive App.Path
    ChDir App.Path

    ' check the correct BASS was loaded
    If (HiWord(BASS_GetVersion) <> BASSVERSION) Then
        Call MsgBox("An incorrect version of BASS.DLL was loaded", vbCritical)
        End
    End If

    ' Let the user choose the output devices
    With frmDevice
        .SelectDevice 1
        .Show vbModal, Me
        outdev(0) = .device
        .SelectDevice 2
        .Show vbModal, Me
        outdev(1) = .device
    End With

    ' setup output devices
    If (BASS_Init(outdev(0), 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device 1")
        Unload Me
    End If

    If (BASS_Init(outdev(1), 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device 2")
        Unload Me
    End If

    frameMulti(0).Caption = " " & VBStrFromAnsiPtr(BASS_GetDeviceDescription(outdev(0))) & " "
    frameMulti(1).Caption = " " & VBStrFromAnsiPtr(BASS_GetDeviceDescription(outdev(1))) & " "
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' release both devices
    Call BASS_SetDevice(outdev(0))
    Call BASS_Free
    Call BASS_SetDevice(outdev(1))
    Call BASS_Free
    End
End Sub

Private Sub cmdOpen_Click(index As Integer)
    On Local Error Resume Next    ' if Cancel pressed...

    ' open a file to play on selected device
    cmd.CancelError = True
    cmd.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
    cmd.DialogTitle = "Open"
    cmd.Filter = "streamable files|*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif|All files|*.*"
    cmd.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub

    Call BASS_StreamFree(chan(index))
    Call BASS_SetDevice(outdev(index)) ' set the device to create stream on

    chan(index) = BASS_StreamCreateFile(BASSFALSE, cmd.filename, 0, 0, BASS_SAMPLE_LOOP)

    If (chan(index) = 0) Then
        cmdOpen(index).Caption = "click here to open a file..."
        Call Error_("Can't play the file")
        Exit Sub
    End If

    cmdOpen(index).Caption = cmd.filename
    Call BASS_ChannelPlay(chan(index), BASSFALSE)
End Sub

' swap channel devices
Private Sub cmdSwap_Click()
    ' swap handles
    Dim temp As Long
    temp = chan(0)
    chan(0) = chan(1)
    chan(1) = temp

    ' swap text
    Dim temp2 As String
    temp2 = cmdOpen(0).Caption
    cmdOpen(0).Caption = cmdOpen(1).Caption
    cmdOpen(1).Caption = temp2

    ' update the channel devices
    Call BASS_ChannelSetDevice(chan(0), outdev(0))
    Call BASS_ChannelSetDevice(chan(1), outdev(1))
End Sub

