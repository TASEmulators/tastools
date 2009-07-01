VERSION 5.00
Begin VB.Form frmFileOpen 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Open File"
   ClientHeight    =   4110
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7380
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
   ScaleHeight     =   4110
   ScaleWidth      =   7380
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "&Cancel"
      Height          =   510
      Left            =   5715
      TabIndex        =   6
      Top             =   3510
      Width           =   1590
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "&Open"
      Default         =   -1  'True
      Height          =   510
      Left            =   4005
      TabIndex        =   5
      Top             =   3510
      Width           =   1590
   End
   Begin VB.DirListBox dirDialog 
      Height          =   2520
      Left            =   135
      TabIndex        =   2
      Top             =   720
      Width           =   3075
   End
   Begin VB.TextBox txtFileOpen 
      Height          =   375
      Left            =   1395
      TabIndex        =   1
      Top             =   90
      Width           =   5865
   End
   Begin VB.FileListBox filDialog 
      Height          =   2730
      Left            =   3330
      Pattern         =   "*.smv"
      TabIndex        =   3
      Top             =   720
      Width           =   3975
   End
   Begin VB.DriveListBox drvDialog 
      Height          =   360
      Left            =   135
      TabIndex        =   4
      Top             =   3585
      Width           =   3120
   End
   Begin VB.Label lblOpen 
      Caption         =   "Open File"
      Height          =   285
      Left            =   270
      TabIndex        =   0
      Top             =   180
      Width           =   1050
   End
End
Attribute VB_Name = "frmFileOpen"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'File open dialog box for SNES9x Input Editor

Option Explicit

'Public filename so it can be accessed easily by other forms

Public strFileName As String

'Variable to indicate which file number to use for opening

Public FileNumber As Byte

'If FileNumber is given as 3, change the open
'button caption to "&Save", and enable typing
'in the filename box


'Hide the form and return control to frmInputEdit

Private Sub cmdCancel_Click()
  
  Call Me.Hide

End Sub

'Update the directory list when the drive is changed

Private Sub drvDialog_Change()
  
  On Error GoTo DriveNotValid
    dirDialog.Path = drvDialog.Drive
  On Error GoTo 0
  
  Exit Sub

DriveNotValid:
  Call MsgBox("Error accessing drive", vbOKOnly, "Error")

End Sub

'Update the file list when the directory is changed

Private Sub DirDialog_Change()
  
  On Error GoTo PathNotValid
    filDialog.Path = dirDialog.Path
  On Error GoTo 0
  
  Exit Sub

PathNotValid:
  Call MsgBox("Error accessing path", vbOKOnly, "Error")

End Sub

'Update the text box with the filename when a file is clicked

Private Sub filDialog_Click()
  
  txtFileOpen.Text = filDialog.FileName

End Sub

Private Sub cmdOpen_Click()
  
  'String for possible error message
  
  Dim strError
  
  If Right(filDialog.Path, 1) = "\" Then
    strFileName = filDialog.Path & txtFileOpen.Text
  Else
    strFileName = filDialog.Path & "\" & txtFileOpen.Text
  End If
  
  If cmdOpen.Caption = "&Open" Then
  
    'Attempt to open file.  It is not necessary to prompt for
    'saving because all changes done with this program are
    'instantaneous.  Additionally, this form is not accessible
    'when a file is open.
    
    On Error GoTo FileOpenError
     
      'Open as Input to check for file's existence
     
      Open strFileName For Input As FileNumber
    
      Close FileNumber
        
      'Now open file for actual reading/writing
        
      Open strFileName For Binary Lock Read Write As FileNumber
  
    On Error GoTo 0
  
  Else
  
    'We might need to ask about overwriting
    
    Dim YNResult As Integer
    
    'If the file does not contain the .pat extension,
    'append ".pat" to the end of the file
    
    If UCase$(Right$(strFileName, 4)) <> ".PAT" Then
    
      strFileName = strFileName & ".pat"
      
    End If
    
    'Check to see if the file exists
    
    On Error GoTo NoOverWrite
    
      Open strFileName For Input As FileNumber
      Close FileNumber
    
    On Error GoTo 0
    
    YNResult = MsgBox("Warning - the file " & strFileName & " already exists." & vbCrLf & _
        "Do you want to overwrite?", vbYesNo, "Overwrite file?")
    
    If YNResult = vbNo Then Exit Sub
    
    On Error GoTo FileOpenError
    
      Kill strFileName
    
NoOverWrite:
    
    On Error GoTo FileOpenError
    
      Open strFileName For Binary Lock Read Write As FileNumber
    
    On Error GoTo 0
    
  End If
    
  'Return control to frmInputEdit
  
  Call Me.Hide
  Exit Sub

FileOpenError:
  
  'Some standard error codes for file I/O:
  
  '52  Bad file name or number
  '53  File Not found
  '54  Bad file mode
  '55  File already open
  '57  Device I/O error
  '58  File already exists
  '59  Bad record length
  '61  Disk full
  '62  Input past end of file
  '63  Bad record number
  '67  Too many files
  '68  Device unavailable
  '70  Permission denied
  '71  Disk Not ready
  '74  Can 't rename with different drive
  '75  Path/File access error
  '76  Path Not found
  
  Select Case Err.Number
  
    Case 53
      strError = "File not found"
  
    Case 55
       strError = "File already open"
  
    Case 61
      strError = "Disk full"
    
    Case 75
      strError = "Path/File access error"
    
    Case 76
      strError = "Path not found"
  
    Case Else
      strError = "Error #" & Str$(Err.Number) & " when opening the file"
    
  End Select
  
  Call MsgBox(strError, vbOKOnly, "File I/O Error")
  
  strFileName = ""
    
  Exit Sub

End Sub
