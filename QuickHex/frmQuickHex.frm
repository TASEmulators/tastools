VERSION 5.00
Begin VB.Form frmQuickHex 
   BackColor       =   &H8000000B&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "QuickHex v6"
   ClientHeight    =   4245
   ClientLeft      =   150
   ClientTop       =   435
   ClientWidth     =   7755
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
   ScaleHeight     =   4245
   ScaleWidth      =   7755
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer InsertTimer 
      Interval        =   1000
      Left            =   3195
      Top             =   2925
   End
   Begin VB.TextBox txtMessage 
      BackColor       =   &H80000004&
      Height          =   825
      Left            =   45
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   24
      TabStop         =   0   'False
      Top             =   3375
      Width           =   4200
   End
   Begin VB.CheckBox chkBackup 
      Caption         =   "Create Backup Before Inserting"
      Enabled         =   0   'False
      Height          =   330
      Left            =   4365
      TabIndex        =   20
      Top             =   3060
      Value           =   1  'Checked
      Width           =   3390
   End
   Begin VB.Timer SecretTimer 
      Enabled         =   0   'False
      Interval        =   205
      Left            =   2745
      Tag             =   "ChangeSecretPic"
      Top             =   2925
   End
   Begin VB.TextBox txtOffset 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   5670
      TabIndex        =   15
      Top             =   2070
      Width           =   2040
   End
   Begin VB.TextBox txtStart 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Index           =   2
      Left            =   5670
      TabIndex        =   11
      Top             =   1620
      Width           =   2040
   End
   Begin VB.TextBox txtEnd 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Index           =   2
      Left            =   5670
      Locked          =   -1  'True
      TabIndex        =   19
      TabStop         =   0   'False
      Top             =   2520
      Width           =   2040
   End
   Begin VB.TextBox txtFile 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Index           =   2
      Left            =   4185
      Locked          =   -1  'True
      TabIndex        =   5
      TabStop         =   0   'False
      Top             =   630
      Width           =   3525
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "Open File &2"
      Height          =   510
      Index           =   2
      Left            =   6165
      TabIndex        =   3
      Top             =   45
      Width           =   1545
   End
   Begin VB.TextBox txtDuration 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   1620
      TabIndex        =   13
      Top             =   2070
      Width           =   2040
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "Open File &1"
      Height          =   510
      Index           =   1
      Left            =   2070
      TabIndex        =   1
      Top             =   45
      Width           =   1545
   End
   Begin VB.TextBox txtFile 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Index           =   1
      Left            =   90
      Locked          =   -1  'True
      TabIndex        =   4
      TabStop         =   0   'False
      Top             =   630
      Width           =   3525
   End
   Begin VB.CommandButton cmdExit 
      Cancel          =   -1  'True
      Caption         =   "E&xit"
      Height          =   510
      Left            =   6165
      TabIndex        =   22
      Top             =   3690
      Width           =   1545
   End
   Begin VB.CommandButton cmdInsert 
      Caption         =   "&Insert"
      Default         =   -1  'True
      Enabled         =   0   'False
      Height          =   510
      Left            =   4500
      TabIndex        =   21
      Top             =   3690
      Width           =   1545
   End
   Begin VB.TextBox txtEnd 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Index           =   1
      Left            =   1620
      TabIndex        =   17
      Top             =   2520
      Width           =   2040
   End
   Begin VB.TextBox txtStart 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Index           =   1
      Left            =   1620
      TabIndex        =   9
      Top             =   1620
      Width           =   2040
   End
   Begin VB.Image imgSkate 
      Height          =   555
      Index           =   3
      Left            =   1575
      Picture         =   "frmQuickHex.frx":0000
      Top             =   2835
      Visible         =   0   'False
      Width           =   345
   End
   Begin VB.Image imgSkate 
      Height          =   555
      Index           =   2
      Left            =   1575
      Picture         =   "frmQuickHex.frx":0AAA
      Top             =   2835
      Visible         =   0   'False
      Width           =   345
   End
   Begin VB.Image imgSkate 
      Height          =   555
      Index           =   1
      Left            =   1575
      Picture         =   "frmQuickHex.frx":1554
      Top             =   2835
      Visible         =   0   'False
      Width           =   345
   End
   Begin VB.Image imgSkate 
      Height          =   555
      Index           =   0
      Left            =   1575
      Picture         =   "frmQuickHex.frx":1FFE
      Top             =   2835
      Visible         =   0   'False
      Width           =   345
   End
   Begin VB.Label lblSecret 
      Alignment       =   2  'Center
      BackColor       =   &H80000004&
      ForeColor       =   &H00FFFFFF&
      Height          =   285
      Left            =   135
      TabIndex        =   23
      Top             =   2970
      Width           =   1320
   End
   Begin VB.Line Line1 
      BorderColor     =   &H80000005&
      Index           =   1
      X1              =   3945
      X2              =   3945
      Y1              =   100
      Y2              =   3100
   End
   Begin VB.Line Line1 
      BorderColor     =   &H80000003&
      Index           =   0
      X1              =   3930
      X2              =   3930
      Y1              =   100
      Y2              =   3100
   End
   Begin VB.Label lblFrame 
      Alignment       =   1  'Right Justify
      Caption         =   "Offset"
      Height          =   285
      Index           =   8
      Left            =   4725
      TabIndex        =   14
      Top             =   2115
      Width           =   870
   End
   Begin VB.Label lblFrame 
      Alignment       =   1  'Right Justify
      Caption         =   "Starting Frame"
      Height          =   285
      Index           =   7
      Left            =   4095
      TabIndex        =   10
      Top             =   1665
      Width           =   1500
   End
   Begin VB.Label lblFrame 
      Alignment       =   1  'Right Justify
      Caption         =   "Ending Frame"
      Height          =   285
      Index           =   6
      Left            =   4185
      TabIndex        =   18
      Top             =   2565
      Width           =   1410
   End
   Begin VB.Label lblFrame 
      Caption         =   "Movie Length:"
      Height          =   285
      Index           =   4
      Left            =   4275
      TabIndex        =   7
      Top             =   1170
      Width           =   3390
   End
   Begin VB.Label lblOutput 
      Alignment       =   1  'Right Justify
      Caption         =   "Output Movie File"
      Height          =   285
      Left            =   4230
      TabIndex        =   2
      Top             =   225
      Width           =   1725
   End
   Begin VB.Label lblFrame 
      Caption         =   "Movie Length:"
      Height          =   285
      Index           =   3
      Left            =   225
      TabIndex        =   6
      Top             =   1170
      Width           =   3390
   End
   Begin VB.Label lblFrame 
      Alignment       =   1  'Right Justify
      Caption         =   "Duration"
      Height          =   285
      Index           =   2
      Left            =   675
      TabIndex        =   12
      Top             =   2115
      Width           =   870
   End
   Begin VB.Label lblFile 
      Alignment       =   1  'Right Justify
      Caption         =   "Input Movie File"
      Height          =   285
      Left            =   135
      TabIndex        =   0
      Top             =   225
      Width           =   1635
   End
   Begin VB.Label lblFrame 
      Alignment       =   1  'Right Justify
      Caption         =   "Ending Frame"
      Height          =   285
      Index           =   1
      Left            =   135
      TabIndex        =   16
      Top             =   2565
      Width           =   1410
   End
   Begin VB.Label lblFrame 
      Alignment       =   1  'Right Justify
      Caption         =   "Starting Frame"
      Height          =   285
      Index           =   0
      Left            =   45
      TabIndex        =   8
      Top             =   1665
      Width           =   1500
   End
End
Attribute VB_Name = "frmQuickHex"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' QuickHex
' Author: JXQ
' Started 1-3-2008
' Finished v0.5 1-3-2008 (SMV)
' Finished v1.0 1-8-2008 (GMV, VBM)
' Finished v2.0 1-12-2008 (FCM)
' Finished v3.0 1-15-2008 (M64, UI improvements)
' Finished v4.0 1-27-2008 (A few small bugfixes)
' Finished v5.0 1-28-2008 (MMV, file mask
'                          radio button, auto-backup)
' Bugfix   v5.1 2-1-2008 (SMV 1.51 bugfix)
' Finished v6.0 #-#-#### (FCM rewrite, red error text)

'This program allows for quick pasting of a portion of one movie
'file into a position on another movie file of the same format.

'*** OVERVIEW FOR ADDING A NEW FORMAT ***

'When adding a new format, the following places must be updated:
'1) cmdOpen_Click - check file for signature.
'2) GetMetaData - get the appropriate metadata based on file format.
'3) FileSetup - check for additional equivalencies besides frame count.
'4) cmdInsert_Click - the actual pasting of the data.
'5) frmFileOpen: optFileType_Click - add a new file pattern type

Option Explicit

Private Type FCMMovie
  Controller As Long '32 bit string representing all four NES controllers
  System As Byte
End Type

'File type of each loaded file. (SMV, GMV, VBM, FCM, M64)
Dim FileType(1 To 2) As String

'For SMV, this is the number of bytes per frame / 2.
'For FCM, GMV and VBM, this is not used.
Dim NumControllers(1 To 2) As Integer

'This is the file offset of the controller data.
Dim ControllerOffset(1 To 2) As Long

'Frame count from the movie file.  Used to ensure
'that writing past EOF is not a problem.
Dim FrameCount(1 To 2) As Long

'Controller data length for FCM format.
Dim ControllerDataLength(1 To 2) As Long

'Controller type data for M64 format.
Dim ControllerType(1 To 2) As Long

'Frame-to-input ratio for M64 format.
Dim FrameToInputRatio(1 To 2) As Single

'Plugin data for M64 format.
Dim M64Plugin(1 To 2, 1 To 4) As String * 64

'Keep track of which movies are loaded.
Dim IsLoaded(1 To 2) As Boolean

'Keep track of the pattern index on the Open dialog box.
Dim PatternIndex As Integer

'Boolean so both duration and end value can be updated.
Dim CursorOnDuration As Boolean
Dim CursorOnEnd As Boolean

'Values for Easter Egg
Dim IsSecretActive As Boolean
Dim PictureIndex As Integer
Dim PictureData(0 To 63) As Integer

'Size the form correctly and center it on the screen.
Private Sub Form_Load()
  Me.Height = 4620
  Me.Width = 7845
  Me.Left = (Screen.Width - Me.Width) / 2
  Me.Top = (Screen.Height - Me.Height) / 2
  
  'Set one of these values to true, for correct behavior
  'before clicking on either field.
  CursorOnEnd = True
  
End Sub

'This is the routine that writes the appropriate data to the movie file.
Private Sub cmdInsert_Click()
  
  Dim FrameStart(1 To 2) As Long    'Starting frame for both movies.
  Dim FrameEnd(1 To 2) As Long      'Ending frame for both movies.
  Dim CurrentFrame(1 To 2) As Long  'Current frame, to use as an index.
  Dim FrameOffset As Long           'Offset chosen for output file.
  Dim CurController As Long         'Current controller for SMV index.
  Dim OffsetDifference As Long      'Difference between the two file pointers.
  
  Dim CurByte As Byte               'Byte to read. (FCM, VBM, GMV x3)
  Dim CurInt As Integer             'Integer to read. (SMV)
  Dim CurLong As Long               'Long to read. (M64)
  Dim CurLong2 As Long
  Dim InputBoxAnswer As String
  Dim BaseBackupFileName As String
  Dim RealBackupFileName As String
  
  Dim Index As Long
  
  'Clear the message box.
  txtMessage.Text = ""
  
  'Due to previous error checking, checking validity of
  'user inputs can be matched against the End Frame text field.
  If txtEnd(1).Text = "" Or txtEnd(2).Text = "" Then
    
    'Not a valid frame range.
    txtMessage.Text = "Insert failed.  Please enter valid numbers for Starting frame and/or Duration."
    
    'Error message in red.
    txtMessage.ForeColor = &HFF&
    
    'Set focus to the appropriate Start Frame field.
    If txtEnd(1).Text = "" Then
      txtStart(1).SetFocus
    ElseIf txtDuration.Text = "" Then
      txtDuration.SetFocus
    Else
      txtStart(2).SetFocus
    End If
    
    Exit Sub
  
  End If
    
  'Store values given in variables that are easier to work with.
  FrameStart(1) = Val(txtStart(1).Text)
  FrameEnd(1) = Val(txtEnd(1).Text)
  FrameOffset = Val(txtOffset.Text)
  FrameStart(2) = Val(txtStart(2).Text)
  FrameEnd(2) = Val(txtEnd(2).Text)
   
  'Check to see if the frame range is valid for the movie.
  'If the end value is too large, warn the user, correct
  'the value to the last possible frame.
  If FrameEnd(1) > FrameCount(1) Then
    
    'Display error when the given frames to place the data extend past
    'the frame count given in the movie file.
    txtMessage.Text = "Insert failed.  Input movie is only " & CStr(Trim$(FrameCount(1))) & " frames - " _
      & "input movie ending point cannot be beyond that frame."
    
    'Error message in red.
    txtMessage.ForeColor = &HFF&
    
    'Adjust the duration and/or starting frame to be valid values
    If FrameStart(1) >= FrameCount(1) Then
      txtStart(1).Text = Trim$(CStr(FrameCount(1)))
      txtDuration.Text = "1"
      txtEnd(1).Text = Trim$(CStr(FrameCount(1)))
    Else
      txtDuration.Text = Trim$(CStr(FrameCount(1) - FrameStart(1) + 1))
      txtEnd(1).Text = Trim$(CStr(FrameCount(1) + 1))
      txtDuration.SetFocus
    End If
      
    Exit Sub
  End If
  
  'The starting point of the Output movie cannot be past its
  'current ending frame (though it can extend beyond it once
  'the merge has started).
  If FrameStart(2) > FrameCount(2) Then
    
    'Display error when the given frames to place the data extend past
    'the frame count given in the movie file.
    txtMessage.Text = "Insert failed.  Output movie is only " & CStr(Trim$(FrameCount(2))) & " frames - " _
      & "output movie starting point must be before or on that frame."
    
    'Error message in red.
    txtMessage.ForeColor = &HFF&
    
    'Adjust the duration and/or starting frame to be valid values.
    txtStart(2).Text = Trim$(CStr(FrameCount(2)))
    txtStart(2).SetFocus
      
    Exit Sub
  End If
  
  'Alright, everything's good, so now we'll loop through the Input
  'Movie, copying each byte into the Output Movie's appropriate spot.
  On Error GoTo FileWriteError
  
  'Check automatic backup option, and make
  'a backup of the output movie if necessary.
  If chkBackup.Value = vbChecked Then
    
    'Create new base filename string to copy backup to.
    BaseBackupFileName = Left$(txtFile(2).Text, Len(txtFile(2).Text) - 4) & "_QuickHexBackup" & Right$(txtFile(2).Text, 4)
      
    'Now we'll check for the first available filename.
    On Error GoTo FileNotExists
    
    'Loop through each number appended.
    Do
    
      'Increment the number being looked for.
      Index = Index + 1
    
      'Append the number to the new filename.
      RealBackupFileName = Left$(BaseBackupFileName, Len(BaseBackupFileName) - 4) & CStr(Index) & Right$(BaseBackupFileName, 4)
    
      'Look for the new file.  If it doesn't exist,
      'the error will throw us out of this loop.
      Open RealBackupFileName For Input As #3
      
      'Still here, so we've got to try again.
      Close #3
    
    Loop
        
FileNotExists:

    'Reset our error checker.
    On Error GoTo FileWriteError
    
    'Copy the original output file to a backup file specified.
    Close #2
    Call FileCopy(txtFile(2).Text, RealBackupFileName)
    Open txtFile(2).Text For Binary Lock Read Write As #2
    
    'Update the message label.
    txtMessage.Text = "Created backup: " & RealBackupFileName
  
  End If
  
  Select Case FileType(1)
    Case "SMV"
      'SMV:
      'A piece of data goes in the following spot:
      'Start with the ControllerOffset of the entire movie.
      'Advance from there to the first frame desired.  Each
      'frame is 2 * (NumControllers) bytes.  The first
      'controller's input goes in this position, and
      'each subsequent controller will go 2 bytes later.
        
      'First, insert any offset 00 bytes.
      If FrameOffset > 0 Then
        CurInt = 0
        
        'For each frame,
        For CurrentFrame(2) = FrameStart(2) To FrameStart(2) + FrameOffset
          
          'For each controller,
          For CurController = 1 To NumControllers(2)
            
            'Write 2 blank bytes.
            Put #2, ControllerOffset(2) + 2 * ((CurrentFrame(2) * NumControllers(2))) + (CurController - 1) * 2, CurInt
          
          Next CurController
        
        Next CurrentFrame
      
      Else
        
        'No offset.
        FrameOffset = 0
      
      End If
    
      'Find the constant difference between the two files positions.
      OffsetDifference = -(ControllerOffset(1) - ControllerOffset(2) + ((FrameStart(1) - FrameStart(2) - FrameOffset) * 2 * (NumControllers(1))))
    
      'Then, insert all bytes from input file.
    
      'For each frame,
      For CurrentFrame(1) = FrameStart(1) To FrameEnd(1)
      
        'For each controller,
        For CurController = 1 To NumControllers(2)
        
          'Get the input data and write it to the output file.
          Get #1, ControllerOffset(1) + 2 * ((CurrentFrame(1) * NumControllers(1))) + (CurController - 1) * 2, CurInt
          Put #2, ControllerOffset(1) + 2 * ((CurrentFrame(1) * NumControllers(1))) + (CurController - 1) * 2 + OffsetDifference, CurInt
        Next CurController
    
      Next CurrentFrame(1)
   
      'If the new written file has become longer,
      If FrameEnd(2) + FrameOffset > FrameCount(2) Then
      
        'Then update the frame count.
        FrameCount(2) = FrameEnd(2) + FrameOffset
        Put #2, 17, FrameCount(2)
        lblFrame(4).Caption = "Movie Length: " & Str$(FrameCount(2)) & " frames"
    
      End If
      
    Case "GMV"
      'GMV:
      'A piece of data goes in the following spot:
      'Start with the ControllerOffset of the entire movie = 64.
      'Advance from there to the first frame desired.  Each
      'frame is 3 bytes.  The first controller's input goes
      'in this position, and each subsequent controller will go
      '1 byte later.  In a 2-player format movie, X, Y, Z, Mode
      'for Player 1 and 2 are combined into this third byte.
    
      'First, insert any offset FF bytes.
      If FrameOffset > 0 Then
        CurByte = 255 'Gens uses FF as a blank byte.
      
        'For each frame, insert three blank bytes.
        For CurrentFrame(2) = FrameStart(2) - 1 To FrameStart(2) + FrameOffset - 1
          Put #2, ControllerOffset(2) + 3 * (CurrentFrame(2)), CurByte
          Put #2, ControllerOffset(2) + 3 * (CurrentFrame(2)) + 1, CurByte
          Put #2, ControllerOffset(2) + 3 * (CurrentFrame(2)) + 2, CurByte
        Next CurrentFrame
    
      Else
        
        'No offset.
        FrameOffset = 0
      
      End If
    
      'Find constant difference between the two files positions.
      OffsetDifference = -3 * (FrameStart(1) - FrameStart(2) - FrameOffset)
    
      'Then, insert all 3-byte frames from the input file.
      For CurrentFrame(1) = FrameStart(1) - 1 To FrameEnd(1) - 1
        'First byte.
        Get #1, ControllerOffset(1) + (3 * CurrentFrame(1)), CurByte
        Put #2, ControllerOffset(1) + (3 * CurrentFrame(1)) + OffsetDifference, CurByte
      
        'Second byte.
        Get #1, ControllerOffset(1) + (3 * CurrentFrame(1)) + 1, CurByte
        Put #2, ControllerOffset(1) + (3 * CurrentFrame(1)) + OffsetDifference + 1, CurByte
      
        'Third byte.
        Get #1, ControllerOffset(1) + (3 * CurrentFrame(1)) + 2, CurByte
        Put #2, ControllerOffset(1) + (3 * CurrentFrame(1)) + OffsetDifference + 2, CurByte
      Next CurrentFrame(1)
    
      'If the new written file has become longer,
      If FrameEnd(2) + FrameOffset > FrameCount(2) Then
        
        'Then update the frame count. (No need to do so in the file for GMV)
        FrameCount(2) = FrameEnd(2) + FrameOffset
        lblFrame(4).Caption = "Movie Length: " & Str$(FrameCount(2)) & " frames"
      
      End If
    
    Case "VBM"
      'VBM:
      'A piece of data goes in the following spot:
      'Start with the ControllerOffset of the entire movie.
      'Advance from there to the first frame desired.  Each
      'frame is 2 bytes.
        
      'First, insert any offset 00 bytes.
      If FrameOffset > 0 Then
        CurInt = 0
        
        'For each frame,
        For CurrentFrame(2) = FrameStart(2) To FrameStart(2) + FrameOffset
          
          'Insert 2 blank bytes.
          Put #2, ControllerOffset(2) + 2 * (CurrentFrame(2)), CurInt
        
        Next CurrentFrame
      
      Else
        
        'No offset.
        FrameOffset = 0
      
      End If
    
      'Find constant difference between the two files positions.
      OffsetDifference = -(ControllerOffset(1) - ControllerOffset(2) + ((FrameStart(1) - FrameStart(2) - FrameOffset) * 2))
    
      'Then, insert all bytes from input file.
      
      'For each frame,
      For CurrentFrame(1) = FrameStart(1) To FrameEnd(1)
          
          'Get the input data and write it to the output file.
          Get #1, ControllerOffset(1) + 2 * CurrentFrame(1), CurInt
          Put #2, ControllerOffset(1) + 2 * CurrentFrame(1) + OffsetDifference, CurInt
      
      Next CurrentFrame(1)
   
      'If the new written file has become longer,
      If FrameEnd(2) + FrameOffset > FrameCount(2) Then
        
        'Then update the frame count.
        FrameCount(2) = FrameEnd(2) + FrameOffset
        Put #2, 13, FrameCount(2)
        lblFrame(4).Caption = "Movie Length: " & Str$(FrameCount(2)) & " frames"
      
      End If
  
    Case "FCM"
      'FCM:
      'I hate this format.
      
      'The controller data starts at a given offset.
      
      'There are two types of bytes: Update bytes and delta bytes.
      'The first byte is an update byte.  All update bytes contain:
      
      '1) "Updates" of which buttons are pressed for all controllers and system
      '   for the current frame (starting at frame 0).
      '2) The number of delta bytes which follow the current byte.
      
      'If no delta bytes follow the update byte, the update happens immediately.
      'This is used to have more than 1 update on a frame - for example, release
      'B and press A.  Similarly for multiple controllers.
      
      'Anywhere from 0-3 delta bytes can follow.  These bytes give the
      'number of frames to WAIT before performing the UPDATE of the
      'PREVIOUS byte.  If there is no delta byte, there is no wait
      'before updating.
      
      'So, we read in:
      'Byte #1: "Toggle Controller 1 B button, but look for 1 delta frame
      'on the next byte to advance before doing this toggle."
      'Byte #2: "A value of 30.  So before actually doing Byte #1's toggle,
      'wait 30 frames."
      
      'This means, to tie together two random strings of FCM,
      'the entire movie must be taken into account.
      
      ReDim InputMovie(0 To FrameCount(1)) As FCMMovie
      ReDim OutputMovie(0 To FrameCount(2)) As FCMMovie
      
      Dim IsNewMovieLonger As Boolean
      Dim ControllerState As Long
      Dim ControllerStateDifference As Long
      Dim NextFrameToUpdate As Long
      
      'Temporary variables.
      Dim CurUpdateLength As Long
      Dim CurFrame As Long
      Dim CurUpdateByte As Byte
      Dim CurUpdateController As Byte
      Dim CurDeltaBytes As Long
      Dim NumUpdateFrames As Long
      
      Dim FrameDifference As Long
      Dim CurrentFilePosition As Long
      Dim FileEndPosition(1 To 2) As Long
      Dim DeltaByteWritten As Boolean
      
      Dim Index2 As Long
      Dim Index3 As Long
      Dim FileIndex As Integer
      Dim CurByte2 As Byte
      
      'Change both movies into a better format.
      For FileIndex = 1 To 2
          
        'Initialize a couple variables.
        ControllerState = 0
        NextFrameToUpdate = 0
        
        'Initialize file ending positions.  They will be
        'overwritten if the movie does become longer (frames).
        FileEndPosition(FileIndex) = ControllerOffset(FileIndex) + ControllerDataLength(FileIndex)
            
        'Initialize beginning frame and file offset.
        CurFrame = 0
        CurrentFilePosition = ControllerOffset(FileIndex)
            
        'Do until all frames are accounted for.
        Do Until CurFrame >= FrameCount(FileIndex)
                      
          'Get the next update byte.
          Get #FileIndex, CurrentFilePosition, CurByte
          
          'Get number of delta bytes following this update byte.
          CurDeltaBytes = NumDeltaBytes((CurByte \ 32) Mod 4)
        
          'If this byte is followed by a delta byte, then
          'we've hit a new update frame sector.  Update all
          'previous frames with the current controller state.
          If CurDeltaBytes > 0 Then
            
            CurUpdateLength = 0
            
            'Get the number of delta bytes.
            For Index = 1 To CurDeltaBytes
            
              Get #FileIndex, CurrentFilePosition + Index, CurByte2
              CurUpdateLength = CurUpdateLength + (CurByte2 * (256 ^ (Index - 1)))

            Next Index
            
            'The next frame to update to is the current frame
            'added to the next delta byte amount.
            NextFrameToUpdate = CurFrame + CurUpdateLength
          
            'Fill the controller state for all bytes from the previous
            'update, until this one, with the current state.
            For Index = CurFrame To NextFrameToUpdate
              
              Select Case FileIndex
                Case 1
                  InputMovie(Index).Controller = ControllerState
                Case 2
                  OutputMovie(Index).Controller = ControllerState
              End Select
            
            Next Index
          
            'Update the current frame and file position.
            CurFrame = NextFrameToUpdate
            CurrentFilePosition = CurrentFilePosition + CurDeltaBytes + 1
            
          Else
            
            'Update the file position.
            CurrentFilePosition = CurrentFilePosition + 1
          
          End If
          
          'If this update byte is a controller byte (not a control byte),
          If CurByte \ 128 = 0 Then
            
            'Get the current controller.
            CurUpdateController = (CurByte \ 8) Mod 4
            
            'Get the current button to toggle.
            CurUpdateByte = CurByte Mod 8
            
            'Update the controller state with this button toggle.
            ControllerState = ControllerState Xor (2 ^ (CurUpdateByte + 8 * (CurUpdateController)))

          Else
            
            'Get the current control code
            CurUpdateByte = CurByte Mod 32
            
            If CurUpdateByte > 0 Then
            
              Select Case FileIndex
                Case 1
                  InputMovie(CurFrame).System = CurUpdateByte
                Case 2
                  OutputMovie(CurFrame).System = CurUpdateByte
              End Select
            
            'Once a no-op is hit, the movie will need
            'the end filled with the current controller state.
            Else
              
              'Set 255 as special "no-op" indicator
              CurUpdateByte = 255
              
              Select Case FileIndex
                Case 1
                  InputMovie(CurFrame).System = 255
                Case 2
                  OutputMovie(CurFrame).System = 255
              End Select
            
              For Index = CurFrame To FrameCount(FileIndex)
              
                Select Case FileIndex
                  Case 1
                    InputMovie(Index).Controller = ControllerState
                  Case 2
                    OutputMovie(Index).Controller = ControllerState
                End Select
            
              Next Index
            
              'All done here.
              Exit Do
            
            End If
          
          End If
        
        Loop
      
      Next FileIndex
      
      Open "test.txt" For Output As #3
      For Index = 0 To FrameCount(2)
        Print #3, "Frame" & Index & "- " & OutputMovie(Index).Controller
      Next Index
      
      'Get the constant frame difference.
      FrameDifference = FrameStart(1) - (FrameStart(2) + FrameOffset)
      
      'Redim the output movie if it is being lengthened
      If FrameEnd(2) + FrameOffset > FrameCount(2) Then
        IsNewMovieLonger = True
        FrameCount(2) = FrameEnd(2) + FrameOffset
        ReDim OutputMovie(0 To FrameCount(2))
      End If
      
      'Insert any offset frames.
      For Index = FrameStart(2) To FrameStart(2) + FrameOffset - 1
        OutputMovie(Index).Controller = 0
      Next Index
            
      'Copy the input movie's data onto the output movie's data.
      For Index = FrameStart(2) + FrameOffset To FrameEnd(2) + FrameOffset
        OutputMovie(Index).Controller = InputMovie(Index + FrameDifference).Controller
      Next Index
      
      For Index = 0 To FrameCount(2)
        Print #3, "Frame" & Index & "- " & OutputMovie(Index).Controller
      Next Index
      Close #3
      
      'Rewrite the new output movie.
      CurrentFilePosition = ControllerOffset(2)
      NextFrameToUpdate = 0 'Used as previous frame to update
      ControllerState = 0   'Used as previous frame's controller state
      
      'Translate the Output Movie back to FCM format.
      For Index = 0 To FrameCount(2)
        
        CurByte = 0
        DeltaByteWritten = False
        
        'First, see if there are any update commands to insert.
        If OutputMovie(Index).System > 0 Or OutputMovie(Index).Controller <> ControllerState Then
          
          'Get the amount to put in for the delta frames.
          NumUpdateFrames = Index - NextFrameToUpdate
          
          'Get the number of delta bytes needed.
          CurDeltaBytes = NumDeltaBytes(NumUpdateFrames)
          
          'Add the delta byte indicator to the first update byte.
          CurByte = CurByte + 32 * CurDeltaBytes
          
          'If update byte is a control update,
          If OutputMovie(Index).System > 0 Then
          
            If OutputMovie(Index).System < 255 Then
            
              'Add control update to the current byte
              CurByte = CurByte + OutputMovie(Index).System + 128
            
            Else
              
              CurByte = CurByte + 128
              
            End If
            
            'Write the update byte.
            Put #2, CurrentFilePosition, CurByte
            Debug.Print "[" & CurrentFilePosition & "-" & CurByte & "]";
                
            'Write delta bytes.
            For Index2 = 1 To CurDeltaBytes
            
              Put #2, CurrentFilePosition + Index2, (NumUpdateFrames Mod (256 ^ (Index2))) \ (256 ^ (Index2 - 1))
              Debug.Print "[" & CurrentFilePosition + Index2 & "-" & (NumUpdateFrames Mod (256 ^ (Index2))) \ (256 ^ (Index2 - 1)) & "]";
              DeltaByteWritten = True
                
            Next Index2
            
            'Advance the file position.
            CurrentFilePosition = CurrentFilePosition + 1 + CurDeltaBytes
          
            CurByte = 0
          
          End If
          
          'If there is a controller update,
          If ControllerState <> OutputMovie(Index).Controller Then
          
            'Loop, and write remaining update bytes.
            ControllerStateDifference = ControllerState Xor OutputMovie(Index).Controller
          
            'Check each controller bit to see what changed.
            For Index2 = 1 To 31
              
              If ControllerStateDifference \ (2 ^ (Index2 - 1)) Mod 2 = 1 Then
                
                'Write update byte.
                CurByte = CurByte + (Index2 - 1)
                Put #2, CurrentFilePosition, CurByte
                Debug.Print "[" & CurrentFilePosition & "-" & CurByte & "]";
                CurByte = 0
                
                'Update controller comparison.
                ControllerStateDifference = ControllerStateDifference - (2 ^ (Index2 - 1))
                
                'Write delta byte if necessary.
                If Not DeltaByteWritten Then
                  
                  DeltaByteWritten = True
            
                  'Write delta bytes.
                  For Index3 = 1 To CurDeltaBytes
            
                    Put #2, CurrentFilePosition + Index3, (NumUpdateFrames Mod (256 ^ (Index3))) \ (256 ^ (Index3 - 1))
                    Debug.Print "[" & CurrentFilePosition + Index3 & "-" & (NumUpdateFrames Mod (256 ^ (Index3))) \ (256 ^ (Index3 - 1)) & "]";
                    DeltaByteWritten = True
                
                  Next Index3
            
                  'Advance the file position.
                  CurrentFilePosition = CurrentFilePosition + CurDeltaBytes
                  
                End If
              
                'Update file position.
                CurrentFilePosition = CurrentFilePosition + 1
                
              End If
            
            Next Index2
            
            'Check bit 32 differently
            If ControllerStateDifference < 0 Then
              
              'Write update byte.
              CurByte = 31
              Put #2, CurrentFilePosition, CurByte
              Debug.Print "[" & CurrentFilePosition & "-" & CurByte & "]";
                
              'Update file position.
              CurrentFilePosition = CurrentFilePosition + 1
            
            End If
            
          End If
          
          'Update the previous controller state.
          ControllerState = OutputMovie(Index).Controller
          
          'Update the previous updated frame.
          NextFrameToUpdate = Index
        End If
      
      Next Index
      
  
      'Update controller input data length.
      ControllerDataLength(2) = CurrentFilePosition - ControllerOffset(2)
      Put #2, 21, ControllerDataLength(2)
  
      'If new movie length is longer,
      If IsNewMovieLonger Then
        Put #2, 13, FrameCount(2)
        lblFrame(4).Caption = "Movie Length: " & Str$(FrameCount(2)) & " frames"
      End If
     
    Case "M64"
      'M64:
      'A piece of data goes in the following spot:
      'Start with the ControllerOffset of the entire movie = 64.
      'Advance from there to the first frame desired.  Each
      'frame is 4 bytes.  The first polled input goes in this position,
      'and each subsequent controller will go 4 bytes later.
    
      'First, insert any offset bytes.
      If FrameOffset > 0 Then
        
        'Set offset bytes in groups of 4.
        CurLong = 0
      
        'For each frame,
        For CurrentFrame(2) = FrameStart(2) - 1 To FrameStart(2) + FrameOffset - 1
          
          'For each controller,
          For CurController = 1 To NumControllers(1)
            
            'Insert four blank bytes.
            Put #2, ControllerOffset(2) + 4 * (NumControllers(1) * (CurrentFrame(2) + CurController - 1)), CurLong
            
          Next CurController
        
        Next CurrentFrame
    
      Else
        
        'No offset.
        FrameOffset = 0
      
      End If
    
      'Find constant difference between the two files positions.
      OffsetDifference = -4 * NumControllers(1) * (FrameStart(1) - FrameStart(2) - FrameOffset)
    
      'Then, insert all 4-byte frames from the input file.
      For CurrentFrame(1) = FrameStart(1) - 1 To FrameEnd(1) - 1
        
        'For each controller,
        For CurController = 1 To NumControllers(1)
        
          'Read the input byte and paste it onto the output movie.
          Get #1, ControllerOffset(1) + 4 * (NumControllers(1) * (CurrentFrame(1) + CurController - 1)), CurLong
          Put #2, ControllerOffset(1) + 4 * (NumControllers(1) * (CurrentFrame(1) + CurController - 1)) + OffsetDifference, CurLong
      
        Next CurController
        
      Next CurrentFrame(1)
    
      'If the new written file has become longer,
      If FrameEnd(2) + FrameOffset > FrameCount(2) Then
        
        'Then update the frame counts.
        FrameCount(2) = FrameEnd(2) + FrameOffset
        lblFrame(4).Caption = "Input-frame Length: " & Str$(FrameCount(2)) & " frames"
        Put #2, 25, FrameCount(2)
        
        'Calculate an estimation of the new real frame count.
        CurLong2 = ((FrameEnd(2) + FrameOffset) - (FrameStart(2)) / (FrameEnd(2) + FrameOffset)) * FrameToInputRatio(1) _
               + (FrameStart(2) / (FrameEnd(2) + FrameOffset)) * FrameToInputRatio(2) + 1
    
        'The real frame count cannot be measured with certainty,
        'so we'll ask the user for it, and offer an estimation
        'based on the ratios of the two movies.
        InputBoxAnswer = InputBox("QuickHex has estimated the new frame count of this movie." _
          & "You may enter a different value if desired.", "New Frame Count", CStr(CurLong2), _
          (Screen.Width - Me.Width) / 2, (Screen.Height - Me.Height) / 2)
        
        'If the answer given is numeric,
        If VerifyNumeric(InputBoxAnswer) Then
        
          'Update the frame count.
          FrameCount(2) = CLng(InputBoxAnswer)
          Put #2, 13, CLng(InputBoxAnswer)
          
        'If not numeric,
        Else
        
          'Update the frame count with the calculated estimation.
          FrameCount(2) = CLng(CurLong2)
          Put #2, 13, CLng(CurLong2)
        
        End If
      
      End If
      
    Case "MMV"
    'MMV:
      'A piece of data goes in the following spot:
      'Start with the ControllerOffset of the entire movie.
      'Advance from there to the first frame desired.  Each
      'frame is 2 bytes.
        
      'First, insert any offset 00 bytes.
      If FrameOffset > 0 Then
        CurInt = 0
        
        'For each frame,
        For CurrentFrame(2) = FrameStart(2) To FrameStart(2) + FrameOffset
          
          'Insert 2 blank bytes.
          Put #2, ControllerOffset(2) + 2 * (CurrentFrame(2)), CurInt
        
        Next CurrentFrame
      
      Else
        
        'No offset.
        FrameOffset = 0
      
      End If
    
      'Find constant difference between the two files positions.
      OffsetDifference = -(ControllerOffset(1) - ControllerOffset(2) + ((FrameStart(1) - FrameStart(2) - FrameOffset) * 2))
    
      'Then, insert all bytes from input file.
      
      'For each frame,
      For CurrentFrame(1) = FrameStart(1) To FrameEnd(1)
          
          'Get the input data and write it to the output file.
          Get #1, ControllerOffset(1) + 2 * CurrentFrame(1), CurInt
          Put #2, ControllerOffset(1) + 2 * CurrentFrame(1) + OffsetDifference, CurInt
      
      Next CurrentFrame(1)
   
      'If the new written file has become longer,
      If FrameEnd(2) + FrameOffset > FrameCount(2) Then
        
        'Then update the frame count.
        FrameCount(2) = FrameEnd(2) + FrameOffset
        Put #2, 13, FrameCount(2)
        lblFrame(4).Caption = "Movie Length: " & Str$(FrameCount(2)) & " frames"
      
      End If
      
    
    'More file types can be added here
    Case Else
        
  End Select
      
  'Temporary disable the insert button.
  cmdInsert.Enabled = False
  
  'Start the timer to re-enable the button.
  InsertTimer.Enabled = True
  
  'Send successful message to the user.
  txtMessage.Text = "Insert successful.  " & txtMessage.Text
  
  'Success message in black.
  txtMessage.ForeColor = &H80000008
    
  
  On Error GoTo 0
  
  Exit Sub
  
FileWriteError:
  Call MsgBox("Error " & Err.Number & " when writing to the file", vbOKOnly, "File I/O Error")
  
End Sub

'Pop up open dialog box and update filename
'if a valid one is given.
Private Sub cmdOpen_Click(FileNum As Integer)
  
  'Index for file pattern storing.
  Dim Index As Integer
  
  'File signature
  Dim Signature As Long
  
  'File name retrieved from open dialog for display
  Dim NewFileName As String
  
  'Disable the insert-button timer.
  InsertTimer.Enabled = False
  
  'Keep track of the button's function by its caption
  If Left$(cmdOpen(FileNum).Caption, 4) = "Open" Then
    
    'Pop up Open dialog
    Dim frmNew As frmFileOpen
    Set frmNew = New frmFileOpen
    
    'Set the pattern index.
    frmNew.optFileType(PatternIndex).Value = True
    
    'If one file is already open, mask the other to be the same type.
    If FileType(1) <> "" Or FileType(2) <> "" Then
    
    End If
    
    Call Load(frmNew)
    
    'Center open form on top of current form
    frmNew.Left = Me.Left + (Me.Width - frmNew.Width) / 2
    frmNew.Top = Me.Top + (Me.Height - frmNew.Height) / 2
      
    'Set FileNumber
    frmNew.FileNumber = FileNum
  
    'Title and show the form
    frmNew.Caption = "Open file"
    Call frmNew.Show(vbModal)
    
    'A non-null text file indicates that
    'a file was loaded successfully
    If Len(frmNew.strFileName) > 0 Then
      
      'Check for signature of file to make sure that it is a valid supported file.
      On Error GoTo ErrorInFile
        Get #FileNum, 1, Signature
      On Error GoTo 0
      
      'SMV signature of 53 4D 56 1A (hex, little-endian)
      '1A564D53 (hex) = 441863507 (decimal)
      If Signature = 441863507 Then
        
        'Valid SMV file, so enable appropriate functions and
        'update the filename textbox
        txtFile(FileNum).Text = frmNew.strFileName
        IsLoaded(FileNum) = True
        FileType(FileNum) = "SMV"
        Call FileSetup(FileNum)
        
      'GMV signature of 47 65 6E 73 (hex, little-endian)
      '736E6547 (hex) = 1936614727 (decimal)
      ElseIf Signature = 1936614727 Then
        
        'Valid GMV file, so enable appropriate functions and
        'update the filename textbox
        txtFile(FileNum).Text = frmNew.strFileName
        IsLoaded(FileNum) = True
        FileType(FileNum) = "GMV"
        Call FileSetup(FileNum)
      
      'VBM signature of 56 42 4D 1A (hex, little-endian)
      '1A4D4256 (hex) = 441270870(decimal)
      ElseIf Signature = 441270870 Then
        
        'Valid VBM file, so enable appropriate functions and
        'update the filename textbox
        txtFile(FileNum).Text = frmNew.strFileName
        IsLoaded(FileNum) = True
        FileType(FileNum) = "VBM"
        Call FileSetup(FileNum)
      
      'FCM signature of 46 43 4D 1A (hex, little-endian)
      '1A4D4346 (hex) = 441271110 (decimal)
      ElseIf Signature = 441271110 Then
        
        'Valid FCM file, so enable appropriate functions and
        'update the filename textbox
        txtFile(FileNum).Text = frmNew.strFileName
        IsLoaded(FileNum) = True
        FileType(FileNum) = "FCM"
        Call FileSetup(FileNum)
      
      'M64 signature of 4D 36 34 1A (hex, little-endian)
      '1A34364D (hex) = 439629389
      ElseIf Signature = 439629389 Then
      
        'Valid M64 file, so enable appropriate functions and
        'update the filename textbox
        txtFile(FileNum).Text = frmNew.strFileName
        IsLoaded(FileNum) = True
        FileType(FileNum) = "M64"
        Call FileSetup(FileNum)

      'MMV signature of 4D 4D 56 00 (hex, little-endian)
      '00564D4D (hex) = 5655885
      ElseIf Signature = 5655885 Then
      
        'Valid MMV file, so enable appropriate functions and
        'update the filename textbox
        txtFile(FileNum).Text = frmNew.strFileName
        IsLoaded(FileNum) = True
        FileType(FileNum) = "MMV"
        Call FileSetup(FileNum)

      'Not a valid signature.
      Else
      
        Call MsgBox("File is not a valid format", vbOKOnly, "Invalid File")
        FileType(FileNum) = ""
        Close #FileNum
      
      End If
    
    End If
  
    'Save the file pattern index.
    For Index = 0 To (frmNew.optFileType.Count - 1)
      If frmNew.optFileType(Index).Value = True Then
        PatternIndex = Index
        Exit For
      End If
    Next Index
    
    'Unload open dialog
    Call Unload(frmNew)
    Set frmNew = Nothing
  
  Else
    
    'If the caption is "cl&ose", close the current file.  Disable
    'appropriate options and remove filename from text box.
    
    On Error GoTo 0
        
    Close #FileNum
    txtFile(FileNum).Text = ""
    IsLoaded(FileNum) = False
    Call FileSetup(FileNum)
  
  End If

  Exit Sub
  
ErrorInFile:
  Call MsgBox("Error " & Err.Number & " when trying to read file", vbOKOnly, "File I/O Error")
  Close #FileNum

End Sub

'If the Duration field gets focus, disable live updating of it.
Private Sub txtDuration_GotFocus()
  CursorOnDuration = True
  CursorOnEnd = False
  
  'If the value "all remaining" is in the duration,
  If txtDuration.Text = "All remaining" Then
  
    'Highlight the text for quick deleting.
    txtDuration.SelStart = 0
    txtDuration.SelLength = 13
    txtDuration.Refresh
  
  End If

End Sub

'If the Duration field loses focus, enable live updating of it.
Private Sub txtDuration_LostFocus()
  CursorOnDuration = False
  CursorOnEnd = True
End Sub

'If the End field gets focus, disable live updating of it.
Private Sub txtEnd_GotFocus(Index As Integer)
  If Index = 1 Then
    CursorOnEnd = True
    CursorOnDuration = False
  End If
End Sub

'If the End field loses focus, enable live updating of it.
Private Sub txtEnd_LostFocus(Index As Integer)
  If Index = 1 Then
    CursorOnEnd = False
    CursorOnDuration = True
  End If
End Sub

'If the duration is being updated, real-time
'update the ending frame if starting frame is given.
Private Sub txtDuration_Change()
    
  'Only update if the cursor is not on the Input End Frame field.
  If Not CursorOnEnd Then
  
    'If Input Starting Frame and Duration are valid,
    If (VerifyNumeric(txtStart(1).Text) Or txtStart(1).Text = "0") And VerifyNumeric(txtDuration.Text) Then
      
      'Calculate the Input Ending Frame.
      txtEnd(1).Text = CStr(Val(txtStart(1).Text) + Val(txtDuration.Text) - 1)
      
      'Do not leave a negative value for the Input Ending Frame.
      If Val(txtEnd(1).Text) < 0 Then
        txtEnd(1).Text = ""
      End If
    
    Else
      
      'Without those two values, Input Ending Frame is cleared.
      txtEnd(1).Text = ""
    
    End If
  
    'If Output Starting Frame and Duration are valid,
    If (VerifyNumeric(txtStart(2).Text) Or txtStart(2).Text = "0") And VerifyNumeric(txtDuration.Text) Then
      
      'Calculate the Output Ending Frame.
      txtEnd(2).Text = Trim$(Str$(Val(txtStart(2).Text) + Val(txtDuration.Text) - 1))
    
    Else
      txtEnd(2).Text = ""
    End If
  
  End If
  
End Sub

'If the ending frame is being updated, real-time
'update the duration if starting frame is given.
Private Sub txtEnd_Change(Index As Integer)

  'Only update if the cursor is not on the Duration field.
  If Not CursorOnDuration Then
  
    'Only calculate changes for Input End Frame, not Output.
    If Index = 1 Then
      
      'If Input Starting Frame is valid,
      If (VerifyNumeric(txtStart(1).Text) Or txtStart(1).Text = "0") Then
        
        'Calculate the Duration.
        txtDuration.Text = Trim$(Str$(Val(txtEnd(1).Text) - Val(txtStart(1).Text) + 1))
        
        'If the Duration is negative, clear the value.
        If Val(txtDuration.Text) < 0 Then
          txtDuration.Text = ""
        End If
      
      Else
        
        'Without a valid Input Starting Frame, clear the Duration.
        txtDuration.Text = ""
      
      End If
    
      'If Output Starting Frame and Duration are valid,
      If (VerifyNumeric(txtStart(2).Text) Or txtStart(2).Text = "0") And VerifyNumeric(txtDuration.Text) Then
        
        'Calculate the Output Ending Frame.
        txtEnd(2).Text = Trim$(Str$(Val(txtStart(2).Text) + Val(txtDuration.Text) - 1))
      Else
        
        'Without these values, clear the Output Ending Frame.
        txtEnd(2).Text = ""
      
      End If
  
    End If
  
 End If
End Sub

Private Sub txtStart_Change(Index As Integer)
  
  'If the duration and input ending frame aren't given,
  If Index = 1 And ((Not VerifyNumeric(txtDuration.Text) And Not VerifyNumeric(txtEnd(1).Text)) _
    Or (txtDuration.Text = "All remaining")) Then
    
    'If the Input movie's starting value is valid,
    If VerifyNumeric(txtStart(1).Text) Or txtStart(1).Text = "0" Then
    
      CursorOnDuration = True
      CursorOnEnd = True
      
      'Assume 0 for the duration (which is the remainder of the movie).
      txtDuration.Text = "All remaining"
      txtEnd(1).Text = CStr(FrameCount(1))
    
      CursorOnDuration = False
      CursorOnEnd = False
      
    End If
  
  End If
  
  'If the offset isn't given,
  If Not VerifyNumeric(txtOffset.Text) Then
    
    'If the Offset is enabled,
    If txtOffset.Enabled Then
      
      'Change the value to 0.
      txtOffset.Text = "0"
    
    End If
  
  End If

  'If the Input Starting Frame is valid,
  If VerifyNumeric(txtStart(Index).Text) Or txtStart(Index).Text = "0" Then
    
    CursorOnDuration = True
    CursorOnEnd = True
    
    'And the Duration is valid (and not copying the entire movie),
    If VerifyNumeric(txtDuration.Text) Then
    
      'Update the Ending Frame.
      txtEnd(Index).Text = CStr(Val(txtStart(Index).Text) + Val(txtDuration.Text) - 1)
    
    'If the duration is the remainder of the movie,
    ElseIf txtDuration.Text = "All remaining" Then
    
      'Depending on which Ending Frame is being updated,
      If Index = 2 Then
        
        'Update the new Output Ending Frame.
        txtEnd(Index).Text = CStr(Val(txtEnd(1).Text) - Val(txtStart(1).Text) + Val(txtStart(2).Text))
        
        'If the value is negative,
        If Val(txtEnd(Index).Text) < 0 Then
        
          'Clear it out.
          txtEnd(Index).Text = ""
        
        End If
        
      Else
        'Update the new Input Ending Frame.
        txtEnd(Index).Text = FrameCount(1)
        
        'If the Output Starting Frame is valid,
        If VerifyNumeric(txtStart(2).Text) Then
        
          'Also update the Output Ending Frame.
          txtEnd(2).Text = CStr(Val(txtStart(2).Text) + Val(txtEnd(1).Text) - Val(txtStart(1).Text))
                
        End If

        'If the ending frame is lower than the starting frame,
        If Val(txtEnd(Index).Text) < Val(txtStart(Index).Text) Then
        
          'Clear the negative ending frames.
          txtEnd(Index).Text = ""
          txtEnd(2).Text = ""
        
        End If
        
        
      End If
    
    End If
  
    CursorOnDuration = False
    CursorOnEnd = False
  
  Else
    
    'Clear the Ending Frame.
    txtEnd(Index).Text = ""
    
    'If Output movie,
    If Index = 2 Then
  
      'Clear the offset as well.
      txtOffset.Text = ""
    
    End If
  
  End If
  
End Sub

'Enable/disable various parts of the form based
'on whether or not a file is open currently.
'Accepts boolean parameter which determines
'whether or not a file has been loaded successfully.
Private Sub FileSetup(Index As Integer)
  Dim Controllers As Byte
  Dim PluginsMatch As Boolean
  
  'If the file is loaded,
  If IsLoaded(Index) Then
        
    'Get relevant metadata from movie file
    If Not GetMetaData(Index) Then
      GoTo FileReadError
    End If
      
    'Enable Input/Output Starting Frame.
    With txtStart(Index)
      .Enabled = True
      .BackColor = &H80000005
    End With
    
    'Based on Index given,
    If Index = 1 Then
      
      'Enable Duration.
      With txtDuration
        .Enabled = True
        .BackColor = &H80000005
      End With
     
      'Enable Input Ending Frame.
      With txtEnd(Index)
        .Enabled = True
        .BackColor = &H80000005
      End With
      
    Else
    
      'Enable Offset.
      With txtOffset
        .Enabled = True
        .BackColor = &H80000005
      End With
        
      'Enable Output Ending Frame.
      txtEnd(Index).Enabled = True
      
    End If
    
    'Enable Filename text field.
    txtFile(Index).Enabled = True
    
    'For M64 format, use different wording to be clear.
    If FileType(Index) = "M64" Then
    
      'Display the input-frame count
      lblFrame(Index + 2).Caption = "Input-frame Length: " & Str$(FrameCount(Index)) & " frames"
    
    'For other formats, just call it "Movie Length".
    Else
    
      'Display the frame count.
      lblFrame(Index + 2).Caption = "Movie Length: " & Str$(FrameCount(Index)) & " frames"
    
    End If

    'Switch button caption now that a file is open
    cmdOpen(Index).Caption = "Close File &" & CStr(Index)
        
  Else
  
    'Disable Input/Output Starting Frame.
    With txtStart(Index)
      .Text = ""
      .Enabled = False
      .BackColor = &H80000013
    End With
    
    'Based on Index given,
    If Index = 1 Then
      
      'Disable Duration.
      With txtDuration
        .Text = ""
        .Enabled = False
        .BackColor = &H80000013
      End With
     
      'Disable Input Ending Frame.
      With txtEnd(Index)
        .Text = ""
        .Enabled = False
        .BackColor = &H80000013
      End With
      
    Else
    
      'Disable Offset.
      With txtOffset
        .Text = ""
        .Enabled = False
        .BackColor = &H80000013
      End With
        
      'Disable Output Ending Frame.
      txtEnd(Index).Enabled = False
    
    End If
    
    'Disable Filename text field.
    txtFile(Index).Enabled = False
    
    'Remove label showing the frame count
    lblFrame(Index + 2).Caption = "Movie Length:"
        
    'Change button caption to "&Open" for the next file
    cmdOpen(Index).Caption = "Open File &" & CStr(Index)
    
  End If
  
  'If both movies are loaded,
  If IsLoaded(1) And IsLoaded(2) Then
    
    'Check that they are the same format.
    If FileType(1) = FileType(2) Then
    
      'If it's an SMV,
      If FileType(1) = "SMV" Then
        
        'Then check for # of controllers.
        If NumControllers(1) = NumControllers(2) Then
      
          'All good for SMV, enable the insert command
          cmdInsert.Enabled = True
          chkBackup.Enabled = True
        
        'Not the same amount of controllers.
        Else
          
          'Do not enable the insert button.
          Call MsgBox("These SMV file cannot be merged because they have a different number of " _
            & "active controllers.", vbOKOnly, "Error")
          cmdInsert.Enabled = False
          chkBackup.Enabled = False
        
        End If
      
      'If it's an M64,
      ElseIf FileType(1) = "M64" Then
      
        'Check that the controller setup data matches.
        If ControllerType(1) = ControllerType(2) Then
        
          'Assume the Plugins will match.
          PluginsMatch = True
          
          'For all four plugins,
          For Index = 1 To 4
          
            'If one does not match,
            If M64Plugin(1, Index) <> M64Plugin(2, Index) Then
            
              'Save as such.
              PluginsMatch = False
            
            End If
          
          Next Index
          
          'If a plugin does not match,
          If Not PluginsMatch Then
          
            'Warn the user, but still enable the insert command.
            Call MsgBox("Warning: One or more of the plugins do not match " _
              & "on these two movies.  Pasted input may not sync correctly." _
              , vbOKOnly, "Plugins do not match")
            
          End If
            
          'All good for M64, enable the insert command
          cmdInsert.Enabled = True
          chkBackup.Enabled = True
        
        'Not the same amount controller setup.
        Else
          
          'Do not enable the insert button.
          Call MsgBox("These M64 files cannot be merged because they have a different controller " _
            & "configuration.", vbOKOnly, "Error")
          cmdInsert.Enabled = False
          chkBackup.Enabled = False
        
        End If

      'Other file types do not need additional checks.
      Else
        
        'Enable the insert button.
        cmdInsert.Enabled = True
        chkBackup.Enabled = True
          
      End If
    
      'Set the cursor on the Input Starting Frame field.
      Call txtStart(1).SetFocus
    
    'Files are not the same format
    Else
      
      'Do not enable the insert button.
      Call MsgBox("These files cannot be merged because they are not the same format." _
        , vbOKOnly, "Error")
      cmdInsert.Enabled = False
      chkBackup.Enabled = False
    
    End If
  
  'One or both of the files is not open.
  Else
    
    'Disable the insert button.
    cmdInsert.Enabled = False
    chkBackup.Enabled = False
  
  End If


  Exit Sub

FileReadError:
  Call MsgBox("Error " & Err.Number & " when trying to read file", vbOKOnly, "File I/O Error")
  Close #1, #2

End Sub

'This subroutine loads a few pieces of metadata from
'a movie file for use later.
Private Function GetMetaData(FileNum As Integer) As Boolean

  Dim Controllers As Byte    'Number of controllers used in the movie.
  Dim RealFrameCount As Long 'Used for M64 format.
  Dim Index As Integer
  
  GetMetaData = False
  
  'The locations are dependent on the file format.
  Select Case FileType(FileNum)
  
    'SMV needs the number of controllers as well as the offsets.
    Case "SMV"
      
      'Get metadata.
      On Error GoTo FileMetaReadError
        Get #FileNum, 17, FrameCount(FileNum)
        Get #FileNum, 21, Controllers
        Get #FileNum, 29, ControllerOffset(FileNum)
      On Error GoTo 0
    
      'Add 1 to make consistent with documentation.
      ControllerOffset(FileNum) = ControllerOffset(FileNum) + 1
      
      'Determine how many *active* controllers are in this movie file.
      NumControllers(FileNum) = 0
      For Index = 1 To 5
      
        'If this controller is enabled,
        If (Controllers \ (2 ^ (Index - 1))) Mod 2 = 1 Then
        
          'add one to the number of active controllers
          NumControllers(FileNum) = NumControllers(FileNum) + 1
      
        End If
      Next Index
     
    'Everything is a constant for GMV.  <3
    Case "GMV"
      
      'The Frame Count is calculated relative to file length.
      'The offset is always 65.
      On Error GoTo FileMetaReadError
        FrameCount(FileNum) = (LOF(FileNum) - 64) / 3
        ControllerOffset(FileNum) = 65
      On Error GoTo 0
      
    'VBM is not constant, but number of controllers isn't necessary.
    Case "VBM"
      
      'Get metadata.
      On Error GoTo FileMetaReadError
        Get #FileNum, 13, FrameCount(FileNum)
        Get #FileNum, 61, ControllerOffset(FileNum)
      On Error GoTo 0
    
      'Increase by 1 to make sense with documentation provided.
      ControllerOffset(FileNum) = ControllerOffset(FileNum) + 1
    
    'FCM has several offsets that are needed due to its compressed format.
    Case "FCM"
      
      'Get metadata.  Although number of controllers can vary, it does
      'not affect the positioning of the data, so they are not checked.
      On Error GoTo FileMetaReadError
        Get #FileNum, 13, FrameCount(FileNum)
        Get #FileNum, 21, ControllerDataLength(FileNum)
        Get #FileNum, 29, ControllerOffset(FileNum)
      On Error GoTo 0
      
      'Increase by 1 to make sense with documentation provided.
      ControllerOffset(FileNum) = ControllerOffset(FileNum) + 1
    
    'M64 - "frame count" will be "input samples".
    'Also make sure the controller setup is the same.
    Case "M64"
      On Error GoTo FileMetaReadError
        Get #FileNum, 13, RealFrameCount
        Get #FileNum, 22, NumControllers(FileNum)
        Get #FileNum, 25, FrameCount(FileNum)
        Get #FileNum, 33, ControllerType(FileNum)
      
        Get #FileNum, 291, M64Plugin(FileNum, 1)
        Get #FileNum, 355, M64Plugin(FileNum, 2)
        Get #FileNum, 419, M64Plugin(FileNum, 3)
        Get #FileNum, 483, M64Plugin(FileNum, 4)
      
      On Error GoTo 0
      
      ControllerOffset(FileNum) = 65
      FrameToInputRatio(FileNum) = RealFrameCount / FrameCount(FileNum)
    
    'MMV
    Case "MMV"
      
      'Currently the controller offset is a constant F4,
      'but to be extendable, it will be read from location
      '0018.
      On Error GoTo FileMetaReadError
        Get #FileNum, 9, FrameCount(FileNum)
        Get #FileNum, 25, ControllerOffset(FileNum)
      On Error GoTo 0
      
      'Increase by 1 to make sense with documentation provided.
      ControllerOffset(FileNum) = ControllerOffset(FileNum) + 1
      
      
  End Select
  
  GetMetaData = True
  Exit Function

FileMetaReadError:
  'No need to display an error - the calling routine will do that.
  Close #FileNum
  GetMetaData = False
  
End Function

Private Sub InsertTimer_Timer()
  
  'After a small delay, re-enable the insert button
  'if both movies are still open.
  If IsLoaded(1) And IsLoaded(2) Then
    cmdInsert.Enabled = True
  End If
  
  InsertTimer.Enabled = False
  
End Sub

'Return true if StrToVerify is numeric, larger than zero, and an integer.
'Otherwise return false.
Private Function VerifyNumeric(StrToVerify As String)
  If (Len(StrToVerify) > 0 And Trim$(Str$(Val(StrToVerify))) = StrToVerify) Then
    If Val(StrToVerify) = Int(Val(StrToVerify)) And Val(StrToVerify) > 0 Then
      VerifyNumeric = True
    Else
      VerifyNumeric = False
    End If
  Else
    VerifyNumeric = False
  End If
End Function

Private Function NumDeltaBytes(Value As Long) As Long
  If Value = 0 Then
    NumDeltaBytes = 0
  ElseIf Value < 256 Then
    NumDeltaBytes = 1
  ElseIf Value < 65536 Then
    NumDeltaBytes = 2
  Else
    NumDeltaBytes = 3
  End If
End Function

'Exit the program via the exit button
Private Sub cmdExit_Click()
  Call Unload(Me)
End Sub

'A little Easter Egg. :)
Private Sub lblSecret_Click()
  
  'Toggle the Easter Egg.
  IsSecretActive = Not IsSecretActive
  
  'If Active, show the Easter Egg.
  If IsSecretActive Then
    
    'Label for creds to help my pride.
    lblSecret.Caption = "Dev by JXQ"
    lblSecret.BackColor = &HC00000
    
    'Set animation data.
    Call SetAnimation
    
    'Set the image to visible.
    imgSkate(0).Visible = True
    
    'Start the timer for animation purposes.
    SecretTimer.Enabled = True
  
  'Disabling the Easter Egg.
  Else
    lblSecret.Caption = ""
    lblSecret.BackColor = &H8000000B
    imgSkate(0).Visible = False
    imgSkate(1).Visible = False
    imgSkate(2).Visible = False
    imgSkate(3).Visible = False
    SecretTimer.Enabled = False
    PictureIndex = 0
  End If

End Sub

'When the timer's interval hits, go to the next
'animation frame.
Private Sub SecretTimer_Timer()
  Call ChangeSecretPic
End Sub

'Rotates the Easter Egg picture, since VB doesn't support
'Animated GIF files.
Private Sub ChangeSecretPic()

  If PictureData(PictureIndex) <> 4 Then
    imgSkate(PictureData(PictureIndex)).Visible = False
  End If
  PictureIndex = (PictureIndex + 1) Mod 64
  If PictureData(PictureIndex) <> 4 Then
    imgSkate(PictureData(PictureIndex)).Visible = True
  End If
  
End Sub

'Sets the animation data for the Easter Egg.
Private Sub SetAnimation()
  Dim Index As Integer
  
  For Index = 0 To 63
    If Index Mod 2 = 1 Then
      PictureData(Index) = 1
    ElseIf Index Mod 8 = 2 Then
      PictureData(Index) = 2
    ElseIf Index Mod 8 = 4 Then
      PictureData(Index) = 3
    Else
      PictureData(Index) = 0
    End If
  Next Index
  
  PictureData(27) = 4
  PictureData(29) = 4
  PictureData(31) = 4
  
  PictureData(48) = 4
  PictureData(49) = 4
  PictureData(52) = 4
  PictureData(53) = 4
  PictureData(54) = 3
  
  PictureData(56) = 4
  PictureData(57) = 4
  PictureData(58) = 2
  PictureData(59) = 3
  PictureData(60) = 2
  PictureData(61) = 3
  PictureData(62) = 2
  PictureData(63) = 3
  
End Sub
