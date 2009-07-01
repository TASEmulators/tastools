VERSION 5.00
Begin VB.Form frmControllers 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Controllers"
   ClientHeight    =   3075
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7140
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3075
   ScaleWidth      =   7140
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtCurrent 
      BackColor       =   &H80000013&
      Height          =   360
      Left            =   4860
      Locked          =   -1  'True
      TabIndex        =   9
      TabStop         =   0   'False
      Top             =   1170
      Width           =   2175
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   510
      Left            =   5490
      TabIndex        =   11
      Top             =   2520
      Width           =   1545
   End
   Begin VB.CommandButton cmdConvert 
      Caption         =   "&Convert"
      Height          =   510
      Left            =   3870
      TabIndex        =   10
      Top             =   2520
      Width           =   1545
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 1 Enabled"
      Height          =   465
      Index           =   1
      Left            =   225
      TabIndex        =   4
      Top             =   2520
      Value           =   1  'Checked
      Width           =   2040
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 2 Enabled"
      Height          =   465
      Index           =   2
      Left            =   225
      TabIndex        =   3
      Top             =   2070
      Value           =   1  'Checked
      Width           =   2040
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 3 Enabled"
      Height          =   465
      Index           =   3
      Left            =   225
      TabIndex        =   2
      Top             =   1620
      Value           =   1  'Checked
      Width           =   2040
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 4 Enabled"
      Height          =   465
      Index           =   4
      Left            =   225
      TabIndex        =   1
      Top             =   1170
      Value           =   1  'Checked
      Width           =   2040
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 5 Enabled"
      Height          =   465
      Index           =   5
      Left            =   225
      TabIndex        =   6
      Top             =   720
      Value           =   1  'Checked
      Width           =   2040
   End
   Begin VB.TextBox txtFile 
      BackColor       =   &H80000013&
      Height          =   360
      Left            =   4140
      Locked          =   -1  'True
      TabIndex        =   7
      TabStop         =   0   'False
      Top             =   720
      Width           =   2895
   End
   Begin VB.Label lblCurrent 
      Caption         =   "Current Controllers:"
      Height          =   330
      Left            =   2835
      TabIndex        =   8
      Top             =   1215
      Width           =   1950
   End
   Begin VB.Label lblFile 
      Caption         =   "Current File:"
      Height          =   330
      Left            =   2835
      TabIndex        =   5
      Top             =   765
      Width           =   1230
   End
   Begin VB.Label lblExplain 
      Alignment       =   2  'Center
      Caption         =   $"frmControllers.frx":0000
      Height          =   510
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   7080
   End
End
Attribute VB_Name = "frmControllers"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'This variable will hold value of which controllers were
'initially enabled

Public Success As Boolean

Public FrameCount As Long
Public ControllerOffset As Long
Public NewFileName As String

Dim OldControllers(1 To 5) As Boolean

'When a controller is changed, check the current
'configuration against the initial one, and if
'necessary, enable/disable the 'convert' button

Private Sub chkPlayer_Click(Index As Integer)

  Dim Selected As String

  For Index = 1 To 5
    
    If chkPlayer(Index).Value = 1 Then
      
      'Build a temporary string to compare against
      'the inital enabled controllers
      
      Selected = Selected & Trim$(Str$(Index)) & ", "
    
    End If
  
  Next Index
  
  Selected = Mid$(Selected, 1, Len(Selected) - 2)
  
  'If no controllers are enabled, disable
  'the 'convert' button
  
  If Selected = "" Then Selected = txtCurrent.Text
    
  'If the strings are not equal, enable
  'the 'convert' button

  cmdConvert.Enabled = (Selected <> txtCurrent.Text)
  
End Sub

'Controllers have been selected, the new file is
'ready to be created.

Private Sub cmdConvert_Click()

  Dim Index As Long
  Dim Index2 As Integer
  
  Dim OldOffset As Long
  Dim NewOffset As Long
  Dim CurOldOffset As Long
  Dim CurNewOffset As Long
  Dim Offset As Long
  
  Dim FileLength As Long
  
  Dim Selected As String
  
  Dim OldNumControllers As Integer
  Dim NewNumControllers As Integer
  
  Dim ControllerData As Byte
  Dim CurrentByte As Byte
  Dim CurrentTwoByte As Integer
  
  'Get the number of old and new controllers
  
  For Index = 1 To 5
    If chkPlayer(Index).Value = 1 Then NewNumControllers = NewNumControllers + 1
    
    If InStr(1, txtCurrent.Text, Trim$(Str$(Index))) > 0 Then
      OldControllers(Index) = True
      OldNumControllers = OldNumControllers + 1
    End If
  Next Index
  
  'Append "(#, #, #)" to the current filename to save in a new place
  
  For Index = 1 To 5
    
    If chkPlayer(Index).Value = 1 Then
      
      'Build a temporary string
      
      Selected = Selected & Trim$(Str$(Index)) & ", "
    
    End If
  
  Next Index
  
  Selected = Mid$(Selected, 1, Len(Selected) - 2)
    
  NewFileName = Left$(txtFile.Text, Len(txtFile.Text) - 4) & "(" & Selected & ").smv"

  'Open the new file
  
  On Error GoTo FileOpenError
    Open NewFileName For Binary Lock Read Write As #2
  
    'Get the frame count and controller offset, and calculate
    'the length of the file
  
    'Get #1, 17, FrameCount
    'Get #1, 29, ControllerOffset
  
    'ControllerOffset = ControllerOffset + 1
    
    'Write the metadata and savestate, which is the data
    'located between 1 and ControllerOffset - 1.
    
    For Index = 1 To ControllerOffset - 1
    
      Get #1, Index, CurrentByte
    
      If Index = 21 Then
    
        'For position 21, this is the new controller flag byte,
        'so it will be updated.
        
        For Index2 = 1 To 5
        
          If chkPlayer(Index2).Value = 1 Then
            ControllerData = ControllerData + (2 ^ (Index2 - 1))
          End If
      
        Next Index2
      
        Put #2, Index, ControllerData
      
      Else
      
        Put #2, Index, CurrentByte
    
      End If
  
    Next Index
  
    'Now write all the appropriate controller data to the new file.
    'This will be handled frame-by-frame.
    
    For Index = 0 To FrameCount
    
      'Calculate the offset of the first controller data for frame
      '(Index) for old and new files.
    
      OldOffset = ControllerOffset + Index * 2 * OldNumControllers
      NewOffset = ControllerOffset + Index * 2 * NewNumControllers
  
      CurOldOffset = 0
      CurNewOffset = 0
      
      For Index2 = 1 To 5
      
        'If the current controller should be enabled
        
        If chkPlayer(Index2).Value = 1 Then
        
          'Check to see if there is a value to get from the old movie.
          'If there is, get it, and increment the offset by 2 for
          'the next controller byte to be possibly read.
        
          If OldControllers(Index2) Then
          
            Get #1, OldOffset + CurOldOffset, CurrentTwoByte
      
            CurOldOffset = CurOldOffset + 2
          
          Else
          
            CurrentTwoByte = 0
            
          End If
                  
          'Write the controller data, and increment the offset by 2
          'to write the next controller byte.
                  
          Put #2, NewOffset + CurNewOffset, CurrentTwoByte
        
          CurNewOffset = CurNewOffset + 2
        
        ElseIf OldControllers(Index2) Then
          
          'If the old controller was enabled and the new one is not,
          'the old offset must still be incremented
          
          CurOldOffset = CurOldOffset + 2
        
        End If
        
      Next Index2
        
    Next Index
  
  On Error GoTo 0
  
  Close #2
  
  'File has been written successfully!
  
  Success = True
  
  Call Me.Hide
  
  Exit Sub
  
FileOpenError:
  
  Call MsgBox("Error opening file " & NewFileName, vbOKOnly)

  Close #2
 
End Sub


'Cancel button

Private Sub cmdCancel_Click()
  
  Call Me.Hide

End Sub
