VERSION 5.00
Begin VB.Form FCMFix 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "FCM Fix v4"
   ClientHeight    =   3105
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6690
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3105
   ScaleWidth      =   6690
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdResetCheck 
      Caption         =   "Check for &Reset or Power Cycle"
      Enabled         =   0   'False
      Height          =   510
      Left            =   1665
      TabIndex        =   6
      Top             =   1980
      Width           =   1590
   End
   Begin VB.TextBox txtFileName 
      BackColor       =   &H8000000F&
      Height          =   330
      Left            =   45
      Locked          =   -1  'True
      TabIndex        =   5
      Top             =   45
      Width           =   6585
   End
   Begin VB.CommandButton cmdLengthCheck 
      Caption         =   "&Validate Length"
      Enabled         =   0   'False
      Height          =   510
      Left            =   45
      TabIndex        =   4
      Top             =   2565
      Width           =   1545
   End
   Begin VB.DriveListBox DriveList 
      Height          =   315
      Left            =   45
      TabIndex        =   3
      Top             =   2025
      Width           =   1545
   End
   Begin VB.CommandButton cmdSaveState 
      Caption         =   "Remove &Savestate"
      Enabled         =   0   'False
      Height          =   510
      Left            =   1665
      TabIndex        =   2
      Top             =   2565
      Width           =   1590
   End
   Begin VB.FileListBox FileList 
      Height          =   2625
      Left            =   3330
      Pattern         =   "*.fcm"
      TabIndex        =   1
      Top             =   405
      Width           =   3300
   End
   Begin VB.DirListBox DirList 
      Height          =   1440
      Left            =   45
      TabIndex        =   0
      Top             =   405
      Width           =   3210
   End
End
Attribute VB_Name = "FCMFix"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'Started 09-24-2007
'Finished v1 10-13-2007
'Finished v2 11-19-2007
'Finished v3 12-13-2007


'This function obtains the length of the movie in frames as given by
'memory position 00Ch (4-byte long), and controller data length
'given by memory position 014h (4-byte long).

'Then, the controller data is traversed, and all "delta frames" are
'added, from the beginning until either the first "no-op" frame, or the
'end of controller data.  This will tell how many game-frames are spent
'in the controller data.  If no hex-edit hacking was done, then the value
'in 00C will be the same.  If they are not the same, fix the value in 00C.

Private Function LengthCheck()
  Dim lTrueFrameCount As Long     'The frame count as it is counted through delta bytes.
  Dim lFrameCount As Long         'The frame count given in the movie header.
  Dim lControlOffset As Long      'The controller data offset given in the movie header.
  Dim lControlLength As Long      'The controller data length given in the movie header.
  Dim lNumDelta As Long           'The number of delta frames for the next update.
  Dim lNumUpdateFrames As Long    'The number of update frames (stored in delta frames).
  
  Dim lIndex As Long
  Dim lIndex2 As Long
  
  Dim bCurByte As Byte
  Dim bCurByte2 As Byte
  
  Dim FileName As String
  Dim NewFileName As String
  
  On Error GoTo LengthCheckError

  'Get data from movie file header.
  Get #1, 13, lFrameCount
  Get #1, 21, lControlLength
  Get #1, 29, lControlOffset
  
  'Add 1 to offsets so they make sense with documentation provided.
  lControlOffset = lControlOffset + 1

  'For all controller data.
  For lIndex = lControlOffset To lControlOffset + lControlLength

    'Get the next update byte.
    Get #1, lIndex, bCurByte

    'Check how many delta bytes are to follow this update byte.
    lNumDelta = (bCurByte \ 32) Mod 4

    'Look at delta bytes to see how many frames pass.
    lNumUpdateFrames = 0

    'If there are delta frames, then check them and update frame counts.
    If lNumDelta > 0 Then

      'There can be 0 to 3 delta frames, so the value can be 0 to 2^24-1.
      For lIndex2 = 1 To lNumDelta

        'Calculate the number of update frames from the delta frame values.
        Get #1, lIndex + lIndex2, bCurByte2
        lNumUpdateFrames = lNumUpdateFrames + (bCurByte2 * (256 ^ (lIndex2 - 1)))

      Next lIndex2

    End If

    'Keep track of the true frame count to compare later.
    lTrueFrameCount = lTrueFrameCount + lNumUpdateFrames

    'Move the pointer past the delta frames.
    lIndex = lIndex + lNumDelta

    'If command is a No-Op (80h or A0h), stop counting frames.
    If bCurByte = 128 Or bCurByte = 160 Then Exit For

  Next lIndex

  'Check the true frame count vs. the given one in the header.
  If lTrueFrameCount <> lFrameCount Then
    
    'If the header is incorrect,
    Close #1
    
    'Get new filename to correct the mistake to.
    FileName = GetFileName()
    NewFileName = Left$(FileName, Len(FileName) - 4) & "_FixedLength.fcm"
    
    'Copy movie to a new file.
    Call FileCopy(FileName, NewFileName)
    
    'Open file and write correct frame count.
    Open NewFileName For Binary Lock Read Write As #2
      Put #2, 13, lTrueFrameCount
    Close #2
    
    'Length was changed.
    LengthCheck = 1
  
  Else
    
    'Length was not changed.
    LengthCheck = 0
  
  End If

  Exit Function

LengthCheckError:
  'Some file error.
  LengthCheck = 5

End Function

'This function checks if the movie file is marked to start from reset
'or not, and compares it to the first byte of the controller data.
'If a reset (81h) or power-cycle (82h) is not found, a power-cycle is
'inserted at the beginning of the controller data.  This function
'leaves any savestate intact.

Private Function ResetCheck()
  Dim lControlOffset As Long      'Offset of the controller data given by the movie header.
  Dim lControlLength As Long      'Length of the controller data given by the movie header.
  Dim ResetOffset As Long         'Used as a boolean, adds 1 to offset for an added reset.
  Dim bStartFromReset As Byte     'Byte from movie file that contains start-from data.
  Dim StartFromReset As Boolean   'Bit value from bStartFromReset byte.
    
  Dim lIndex As Long
  Dim bCurByte As Byte
  
  Dim FileName As String
  Dim NewFileName As String
  Dim sMessage As String
  
  On Error GoTo ResetCheckError
  
  'Position 08h contains movie start-from data.
  Get #1, 9, bStartFromReset
      
  'Second bit determines if movie starts from reset.
  StartFromReset = (((bStartFromReset \ 2) Mod 2) = 1)
  
  'If the movie does not start from reset, exit here.
  If Not StartFromReset Then
    
    'Movie is marked to start from savestate; do nothing and tell the user.
    ResetCheck = 1
    Exit Function
  
  End If
  
  'Get controller data offset from file header data.
  Get #1, 29, lControlOffset
  
  'Add 1 to the offset so it makes sense with documentation provided.
  lControlOffset = lControlOffset + 1
    
  'Check for a reset in the controller data.
  Get #1, lControlOffset, bCurByte

  'Check for a power-cycle (82h).
  If bCurByte = 130 Then
  
    'A power-cycle is found, so this file is good.
    ResetCheck = 0
    Exit Function

  'Check for a soft reset (81h).
  ElseIf bCurByte = 129 Then
  
    'A soft-reset is found, so ask the user if they want to replace
    'it with a power-cycle.
    sMessage = "A soft-reset is present.  Do you want to change this to a power-cycle?"
    If MsgBox(sMessage, vbYesNo, "Change Reset to Power-Cycle?") = vbYes Then
      
      ResetCheck = 2
      
    Else
      
      'The user is fine with a reset, so do nothing to the file.
      ResetCheck = 4
      Exit Function

    End If

  'Still go through the file, since we may be shifting all the controller
  'data by 1 byte.
  Else
      
    'This movie has no reset or power-cycle, and a power-cycle
    'will be inserted at the beginning of the controller data.
    ResetCheck = 3
    
  End If
  
  'Metadata and savestate is a straight copy
  '(except for new offset, which we'll overwrite later).
  
  'Get new filename to correct the mistake to.
  FileName = GetFileName()
  NewFileName = Left$(FileName, Len(FileName) - 4) & "_FixedPowerCycle.fcm"
    
  'Open the new file to copy the fix to.
  On Error GoTo ResetCheckError
    Open NewFileName For Binary Lock Read Write As #2
  On Error GoTo 0
  
  'First, copy all the metadata and savestate.
  For lIndex = 1 To lControlOffset
    Get #1, lIndex, bCurByte
    Put #2, lIndex, bCurByte
  Next lIndex
  
  'Now, jump to the controller data.
  lIndex = lControlOffset
  
  'Prepare to insert a power-cycle command.
  bCurByte = 130
    
  'Insert the power-cycle.
  Put #2, lIndex, bCurByte
    
  'If necessary, add 1 to the length of controller data
  '(to allow space for the power-cycle).
  If ResetCheck = 3 Then
    Get #1, 21, lControlLength
    Put #2, 21, lControlLength + 1
  End If
    
  'Be sure to not re-write the old reset command if it was replaced
  'with a power-cycle.
  If ResetCheck = 2 Then lControlOffset = lControlOffset + 1
  
  'Copy the controller data from the old file to the appropriate spot
  'in the new file.  Depending on the above circumstances, the
  'power-cycle may have moved everything after it forward 1 byte.
  For lIndex = lControlOffset To LOF(1)
    
    Get #1, lIndex, bCurByte
    Put #2, lIndex + (ResetCheck - 2), bCurByte
  
  Next lIndex
  
  Exit Function

ResetCheckError:
  'Some file error.
  ResetCheck = 5
  
End Function

'This function removes the savestate and replaces it with a generic
'16-byte string that will allow the format to still play.  If the
'movie file is not marked as "start-from-reset", the savestate will
'not be removed.

Private Function SaveState()
  Dim lSaveOffset As Long         'Offset of the savestate data given by the movie header.
  Dim lControlOffset As Long      'Offset of the controller data given by the movie header.
  Dim lControlLength As Long      'Length of the controller data given by the movie header.
  Dim bStartFromReset As Byte     'Byte from movie file that contains start-from data.
  Dim StartFromReset As Boolean   'Bit value from bStartFromReset byte.
  
  Dim lCurLong As Long
  Dim lIndex As Long
  Dim bCurByte As Byte
  
  Dim FileName As String
  Dim NewFileName As String
  
  On Error GoTo SaveStateError
  
  'Position 08h contains movie start-from data.
  Get #1, 9, bStartFromReset
      
  'Second bit determines if movie starts from reset.
  StartFromReset = (((bStartFromReset \ 2) Mod 2) = 1)
  
  'If the movie does not start from reset, exit here.
  If Not StartFromReset Then
    
    'Movie is marked to start from savestate; do nothing and tell the user.
    SaveState = 2
    Exit Function
  
  End If
  
  'Get offsets from file header data.
  Get #1, 25, lSaveOffset
  Get #1, 29, lControlOffset
  
  'Add 1 to offsets so they make sense with documentation provided.
  lSaveOffset = lSaveOffset + 1
  lControlOffset = lControlOffset + 1
    
  'If there are less than 17 bytes between the savestate and controller
  'data, then it has no savestate to begin with.
  If lControlOffset - lSaveOffset < 17 Then
    
    'No savestate present, so this check is finished.
    SaveState = 1
    Exit Function
  
  End If
  
  'Metadata is a straight copy (except for new offset, which we'll overwrite later).
  
  'Get new filename to correct the mistake to.
  FileName = GetFileName()
  NewFileName = Left$(FileName, Len(FileName) - 4) & "_FixedSavestate.fcm"
    
  'Open the new file to copy the fix to.
  On Error GoTo SaveStateError
    Open NewFileName For Binary Lock Read Write As #2
  On Error GoTo 0
  
  'First, copy all the metadata.
  For lIndex = 1 To lSaveOffset
    Get #1, lIndex, bCurByte
    Put #2, lIndex, bCurByte
  Next lIndex
  
  'Now write the new controller data offset, as the savestate offset + 16
  '(minus 1, due to incrementing for documentation consistency).
  Put #2, 29, lSaveOffset + 15
  
  'Put sixteen bytes here which represent:
  '000054 46 43 53 00   savestate header (version: 0.0.00)
  lCurLong = 5456710
  Put #2, lIndex - 1, lCurLong
  
  '000058 00 00 00 00   savestate length: 0 bytes
  lCurLong = 0
  Put #2, lIndex + 3, lCurLong
  
  '00005C 00 00 00 00   savestate version: 0.0.00
  Put #2, lIndex + 7, lCurLong
  
  '000060 00 00 00 00   end of savestate
  Put #2, lIndex + 11, lCurLong
  
  'Now, skip the savestate and jump to the controller data.
  lIndex = lControlOffset
  
  'Copy the controller data from the old file to the appropriate spot
  'in the new file, so there is no savestate before it.
  For lIndex = lControlOffset To LOF(1)
    
    Get #1, lIndex, bCurByte
    Put #2, (lIndex - (lControlOffset - lSaveOffset) + 16), bCurByte
  
  Next lIndex
  
  Exit Function

SaveStateError:
  'Some file error.
  SaveState = 5
  
End Function

'This routine runs when the "Check for Reset" button is clicked.
'It's a wrapper routine that calls ResetCheck() after setting up the files.

Private Sub cmdResetCheck_Click()
  Dim lResult As Long         'Result returned from ResetCheck function.
  Dim sResponse As String     'Response to show the user after checking the file.
  Dim lIndex As Long          'File list index
  
  'If a file was not selected, do not try to check it.
  If FileList.ListCount = 0 Then Exit Sub
  
  'Try to open the file.
  If FileOpen(GetFileName) Then
    
    'The file is open, proceed with the SaveState check.
    lResult = ResetCheck()
    
    'Close the file.
    Call FileClose
    
    'Assign the user message based on the result of the LengthCheck.
    Select Case lResult
      Case 0
        sResponse = "There is a power-cycle present."
      Case 1
        sResponse = "There is no reset or power-cycle present, because the movie is marked to start from a savestate."
      Case 2
        sResponse = "There was a reset present, which has been changed to a power-cycle."
      Case 3
        sResponse = "There was no reset or power-cycle present, so a power-cycle has been added."
      Case 4
        sResponse = "There is a reset present, which has not been changed."
      Case 5
        sResponse = "There was a File I/O error. #" + Str$(Err.Number)
    End Select
  Else
    
    'There was an error trying to open the file.
    sResponse = "There was a File I/O error. #" + Str$(Err.Number)
    
  End If
  
  'Send return message to the user.
  Call MsgBox(sResponse, vbOKOnly, "Checking for Reset")

  'Save which list entry is currently selected.
  lIndex = FileList.ListIndex
  
  'Refresh the file list.
  Call FileList.Refresh
  
  'Set the selected list entry back to its previous value, unless
  'refreshing the file list caused that to be invalid.
  
  'If the File List is empty, do not highlight a value.
  If FileList.ListCount = 0 Then
    FileList.ListIndex = -1
  
  'If the previous selection is too far down, highlight
  'the top entry.
  ElseIf lIndex > FileList.ListCount Then
    FileList.ListIndex = 0
  
  'Otherwise, restore the previous selection.
  Else
    FileList.ListIndex = lIndex
  End If
End Sub

'This routine runs when the "Validate Length" button is clicked.
'It's a wrapper routine that calls LengthCheck() after setting up the files.

Private Sub cmdLengthCheck_Click()
  Dim lResult As Long         'Result returned from LengthCheck function.
  Dim sResponse As String     'Response to show the user after checking the file.
  Dim lIndex As Long          'File list index
  
  'If a file was not selected, do not try to check it.
  If FileList.ListCount = 0 Then Exit Sub
  
  'Try to open the file.
  If FileOpen(GetFileName) Then
    
    'The file is open, proceed with the LengthCheck.
    lResult = LengthCheck()
    
    'Close the file.
    Call FileClose
    
    'Assign the user message based on the result of the LengthCheck.
    Select Case lResult
      Case 0
        sResponse = "The movie length is correct."
      Case 1
        sResponse = "The movie length was incorrect, and has been corrected."
      Case 5
        sResponse = "There was a File I/O error. #" + Str$(Err.Number)
    End Select
  Else
    
    'There was an error trying to open the file.
    sResponse = "There was a File I/O error. #" + Str$(Err.Number)
  
  End If
  
  'Send return message to the user.
  Call MsgBox(sResponse, vbOKOnly, "Validating Length")
  
  'Save which list entry is currently selected.
  lIndex = FileList.ListIndex
  
  'Refresh the file list.
  Call FileList.Refresh
  
  'Set the selected list entry back to its previous value, unless
  'refreshing the file list caused that to be invalid.
  
  'If the File List is empty, do not highlight a value.
  If FileList.ListCount = 0 Then
    FileList.ListIndex = -1
  
  'If the previous selection is too far down, highlight
  'the top entry.
  ElseIf lIndex > FileList.ListCount Then
    FileList.ListIndex = 0
  
  'Otherwise, restore the previous selection.
  Else
    FileList.ListIndex = lIndex
  End If
End Sub

'This routine runs when the "Remove Savestate" & vbCrLf & "Check for Reset" button is clicked.
'It's a wrapper routine that calls SaveState() after setting up the files.

Private Sub cmdSaveState_Click()
  Dim lResult As Long         'Result returned from SaveState function.
  Dim sResponse As String     'Response to show the user after checking the file.
  Dim lIndex As Long          'File list index
  
  'If a file was not selected, do not try to check it.
  If FileList.ListCount = 0 Then Exit Sub
  
  'Try to open the file.
  If FileOpen(GetFileName) Then
    
    'The file is open, proceed with the SaveState check.
    lResult = SaveState()
    
    'Close the file.
    Call FileClose
    
    'Assign the user message based on the result of the LengthCheck.
    Select Case lResult
       Case 0
        sResponse = "There was a savestate present, which has been removed."
      Case 1
        sResponse = "There was no savestate present."
      Case 2
        sResponse = "There is a savestate present, but the movie is marked to start from it." _
        & vbCrLf & "No changes were made to the file."
      Case 5
        sResponse = "There was a File I/O error. #" + Str$(Err.Number)
    End Select
  Else
    
    'There was an error trying to open the file.
    sResponse = "There was a File I/O error. #" + Str$(Err.Number)
  
  End If
  
  'Send return message to the user.
  Call MsgBox(sResponse, vbOKOnly, "Removing Savestate")
  
  'Save which list entry is currently selected.
  lIndex = FileList.ListIndex
  
  'Refresh the file list.
  Call FileList.Refresh
  
  'Set the selected list entry back to its previous value, unless
  'refreshing the file list caused that to be invalid.
  
  'If the File List is empty, do not highlight a value.
  If FileList.ListCount = 0 Then
    FileList.ListIndex = -1
  
  'If the previous selection is too far down, highlight
  'the top entry.
  ElseIf lIndex > FileList.ListCount Then
    FileList.ListIndex = 0
  
  'Otherwise, restore the previous selection.
  Else
    FileList.ListIndex = lIndex
  End If

End Sub

'Enable or disable appropriate buttons based on
'whether a file is available to check.

Private Sub CheckEnabled()
  
  If FileList.ListCount = 0 Then
    cmdLengthCheck.Enabled = False
    cmdSaveState.Enabled = False
    cmdResetCheck.Enabled = False
  
  Else
    cmdLengthCheck.Enabled = True
    cmdSaveState.Enabled = True
    cmdResetCheck.Enabled = True
    
  End If
End Sub

'Return the filename string based on the values
'in the various file objects' properties.

Private Function GetFileName()
  If FileList.ListCount = 0 Then
    GetFileName = ""
    Exit Function
  End If
   
  If Right(FileList.Path, 1) = "\" Then
    GetFileName = DirList.Path & FileList.FileName
  Else
    GetFileName = DirList.Path & "\" & FileList.FileName
  End If
  
End Function

Private Function FileOpen(FileName As String)
  On Error GoTo FileOpenError
     
  'Check for existence
  Open FileName For Input As #1
  Close #1
        
  'Now open file for actual reading/writing
  Open FileName For Binary Lock Read Write As #1
  On Error GoTo 0
  FileOpen = True
  
  Exit Function
  
FileOpenError:
  FileOpen = False
  On Error GoTo 0

End Function

'Close all files that might be open
Private Sub FileClose()
  Close #1, #2
End Sub

'Upon loading, place the filename in the textbox
'and enable or disable the appropriate buttons.

Private Sub Form_Load()
  txtFileName.Text = DirList.Path + FileList.FileName
  Call CheckEnabled
End Sub

Private Sub cmdExit_Click()
  Call Unload(Me)
End Sub

Private Sub DirList_Change()
  
  On Error GoTo PathNotValid
    FileList.Path = DirList.Path
  On Error GoTo 0
  
  If FileList.ListCount > 0 Then FileList.ListIndex = 0
  Call CheckEnabled
  txtFileName.Text = DirList.Path + FileList.FileName
  
  Exit Sub

PathNotValid:
  Call MsgBox("Error accessing path", vbOKOnly, "Error")

End Sub

Private Sub DriveList_Change()
  
  On Error GoTo DriveNotValid
    DirList.Path = DriveList.Drive
  On Error GoTo 0
  
  If FileList.ListCount > 0 Then FileList.ListIndex = 0
  
  Call CheckEnabled
  txtFileName.Text = DirList.Path + FileList.FileName
    
  Exit Sub

DriveNotValid:
  Call MsgBox("Error accessing drive", vbOKOnly, "Error")

End Sub

Private Sub DirList_Click()
  Call CheckEnabled
End Sub

Private Sub FileList_Click()
  Call CheckEnabled
  txtFileName.Text = DirList.Path + FileList.FileName
End Sub

