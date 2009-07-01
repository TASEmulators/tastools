VERSION 5.00
Begin VB.Form frmFileOpen 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Open File"
   ClientHeight    =   4680
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
   ScaleHeight     =   4680
   ScaleWidth      =   7380
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.OptionButton optFileType 
      Caption         =   "MMV"
      Height          =   465
      Index           =   6
      Left            =   4590
      TabIndex        =   13
      Top             =   3555
      Width           =   825
   End
   Begin VB.OptionButton optFileType 
      Caption         =   "M64"
      Height          =   465
      Index           =   5
      Left            =   3690
      TabIndex        =   12
      Top             =   3555
      Width           =   780
   End
   Begin VB.OptionButton optFileType 
      Caption         =   "FCM"
      Height          =   465
      Index           =   4
      Left            =   1800
      TabIndex        =   11
      Top             =   3555
      Width           =   780
   End
   Begin VB.OptionButton optFileType 
      Caption         =   "VBM"
      Height          =   465
      Index           =   3
      Left            =   6525
      TabIndex        =   10
      Top             =   3555
      Width           =   825
   End
   Begin VB.OptionButton optFileType 
      Caption         =   "GMV"
      Height          =   465
      Index           =   2
      Left            =   2745
      TabIndex        =   9
      Top             =   3555
      Width           =   780
   End
   Begin VB.OptionButton optFileType 
      Caption         =   "SMV"
      Height          =   465
      Index           =   1
      Left            =   5580
      TabIndex        =   8
      Top             =   3555
      Width           =   780
   End
   Begin VB.OptionButton optFileType 
      Caption         =   "All supported"
      Height          =   465
      Index           =   0
      Left            =   180
      TabIndex        =   7
      Top             =   3555
      Value           =   -1  'True
      Width           =   1500
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "&Cancel"
      Height          =   510
      Left            =   5715
      TabIndex        =   6
      Top             =   4140
      Width           =   1590
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "&Open"
      Default         =   -1  'True
      Height          =   510
      Left            =   4005
      TabIndex        =   5
      Top             =   4140
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
      Pattern         =   "*.smv;*.gmv;*.vbm;*.fcm;*.m64;*.mmv"
      TabIndex        =   3
      Top             =   720
      Width           =   3975
   End
   Begin VB.DriveListBox drvDialog 
      Height          =   360
      Left            =   135
      TabIndex        =   4
      Top             =   4215
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
'File open dialog box for QuickHex

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

'Different option buttons will change the pattern
'on the file open object.
Private Sub optFileType_Click(Index As Integer)
  Select Case Index
    Case 0
      filDialog.Pattern = "*.smv;*.gmv;*.vbm;*.fcm;*.m64;*.mmv"
    Case 1
      filDialog.Pattern = "*.smv"
    Case 2
      filDialog.Pattern = "*.gmv"
    Case 3
      filDialog.Pattern = "*.vbm"
    Case 4
      filDialog.Pattern = "*.fcm"
    Case 5
      filDialog.Pattern = "*.m64"
    Case 6
      filDialog.Pattern = "*.mmv"
  End Select
End Sub
