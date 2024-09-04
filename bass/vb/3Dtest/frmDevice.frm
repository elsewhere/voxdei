VERSION 5.00
Begin VB.Form frmDevice 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS output device selector"
   ClientHeight    =   1500
   ClientLeft      =   5160
   ClientTop       =   4530
   ClientWidth     =   3855
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1500
   ScaleWidth      =   3855
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   300
      Left            =   2280
      TabIndex        =   1
      Top             =   1080
      Width           =   1335
   End
   Begin VB.ListBox lstDevices 
      Height          =   840
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   3615
   End
End
Attribute VB_Name = "frmDevice"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'///////////////////////////////////////////////////////////////////
' frmDevice.frm (c) 2003-2006 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                           [http://www.jobnik.org]
'                                           [  jobnik@jobnik.org  ]
'
' Other source: frm3Dtest.frm
'
' BASS output device selector
' Originally translated from - 3dtest.c - example of Ian Luck
'///////////////////////////////////////////////////////////////////

Public device As Long       'selected device

Private Sub btnOK_Click()
    Unload Me
End Sub

Private Sub Form_Load()
    Dim text As String
    Dim c As Integer
    
    c = 1
    While BASS_GetDeviceDescription(c)  'device 1 = 1st real device
        text = VBStrFromAnsiPtr(BASS_GetDeviceDescription(c))
        'Check if the device supports 3D
        If (BASS_Init(c, 44100, BASS_DEVICE_3D, Me.hWnd, 0)) Then
            If (BASS_GetEAXParameters(0, 0#, 0#, 0#)) Then
                text = text & " [EAX]"  'it has EAX
            End If
            Call BASS_Free
        
            lstDevices.AddItem text
            lstDevices.ItemData(lstDevices.NewIndex) = c    'store device #
        End If
        c = c + 1
    Wend
    
    If (lstDevices.ListCount) Then lstDevices.Selected(0) = True
    device = 0
End Sub

Private Sub Form_Unload(Cancel As Integer)
    device = lstDevices.ItemData(lstDevices.ListIndex)  'get device #
End Sub

Private Sub lstDevices_DblClick()
    Unload Me
End Sub
