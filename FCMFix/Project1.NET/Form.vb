Option Strict Off
Option Explicit On
Imports VB = Microsoft.VisualBasic
Friend Class FCMFix
	Inherits System.Windows.Forms.Form
	
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
	
	Private Function LengthCheck() As Object
		Dim lTrueFrameCount As Integer 'The frame count as it is counted through delta bytes.
		Dim lFrameCount As Integer 'The frame count given in the movie header.
		Dim lControlOffset As Integer 'The controller data offset given in the movie header.
		Dim lControlLength As Integer 'The controller data length given in the movie header.
		Dim lNumDelta As Integer 'The number of delta frames for the next update.
		Dim lNumUpdateFrames As Integer 'The number of update frames (stored in delta frames).
		
		Dim lIndex As Integer
		Dim lIndex2 As Integer
		
		Dim bCurByte As Byte
		Dim bCurByte2 As Byte
		
		Dim FileName As String
		Dim NewFileName As String
		
		On Error GoTo LengthCheckError
		
		'Get data from movie file header.
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, lFrameCount, 13)
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, lControlLength, 21)
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, lControlOffset, 29)
		
		'Add 1 to offsets so they make sense with documentation provided.
		lControlOffset = lControlOffset + 1
		
		'For all controller data.
		For lIndex = lControlOffset To lControlOffset + lControlLength
			
			'Get the next update byte.
			'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FileGet(1, bCurByte, lIndex)
			
			'Check how many delta bytes are to follow this update byte.
			lNumDelta = (bCurByte \ 32) Mod 4
			
			'Look at delta bytes to see how many frames pass.
			lNumUpdateFrames = 0
			
			'If there are delta frames, then check them and update frame counts.
			If lNumDelta > 0 Then
				
				'There can be 0 to 3 delta frames, so the value can be 0 to 2^24-1.
				For lIndex2 = 1 To lNumDelta
					
					'Calculate the number of update frames from the delta frame values.
					'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
					FileGet(1, bCurByte2, lIndex + lIndex2)
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
			FileClose(1)
			
			'Get new filename to correct the mistake to.
			'UPGRADE_WARNING: Couldn't resolve default property of object GetFileName(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			FileName = GetFileName()
			NewFileName = VB.Left(FileName, Len(FileName) - 4) & "_FixedLength.fcm"
			
			'Copy movie to a new file.
			Call FileCopy(FileName, NewFileName)
			
			'Open file and write correct frame count.
			FileOpen(2, NewFileName, OpenMode.Binary, , OpenShare.LockReadWrite)
			'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FilePut(2, lTrueFrameCount, 13)
			FileClose(2)
			
			'Length was changed.
			'UPGRADE_WARNING: Couldn't resolve default property of object LengthCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			LengthCheck = 1
			
		Else
			
			'Length was not changed.
			'UPGRADE_WARNING: Couldn't resolve default property of object LengthCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			LengthCheck = 0
			
		End If
		
		Exit Function
		
LengthCheckError: 
		'Some file error.
		'UPGRADE_WARNING: Couldn't resolve default property of object LengthCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		LengthCheck = 5
		
	End Function
	
	'This function checks if the movie file is marked to start from reset
	'or not, and compares it to the first byte of the controller data.
	'If a reset (81h) or power-cycle (82h) is not found, a power-cycle is
	'inserted at the beginning of the controller data.  This function
	'leaves any savestate intact.
	
	Private Function ResetCheck() As Object
		Dim lControlOffset As Integer 'Offset of the controller data given by the movie header.
		Dim lControlLength As Integer 'Length of the controller data given by the movie header.
		Dim ResetOffset As Integer 'Used as a boolean, adds 1 to offset for an added reset.
		Dim bStartFromReset As Byte 'Byte from movie file that contains start-from data.
		Dim StartFromReset As Boolean 'Bit value from bStartFromReset byte.
		
		Dim lIndex As Integer
		Dim bCurByte As Byte
		
		Dim FileName As String
		Dim NewFileName As String
		Dim sMessage As String
		
		On Error GoTo ResetCheckError
		
		'Position 08h contains movie start-from data.
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, bStartFromReset, 9)
		
		'Second bit determines if movie starts from reset.
		StartFromReset = (((bStartFromReset \ 2) Mod 2) = 1)
		
		'If the movie does not start from reset, exit here.
		If Not StartFromReset Then
			
			'Movie is marked to start from savestate; do nothing and tell the user.
			'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			ResetCheck = 1
			Exit Function
			
		End If
		
		'Get controller data offset from file header data.
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, lControlOffset, 29)
		
		'Add 1 to the offset so it makes sense with documentation provided.
		lControlOffset = lControlOffset + 1
		
		'Check for a reset in the controller data.
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, bCurByte, lControlOffset)
		
		'Check for a power-cycle (82h).
		If bCurByte = 130 Then
			
			'A power-cycle is found, so this file is good.
			'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			ResetCheck = 0
			Exit Function
			
			'Check for a soft reset (81h).
		ElseIf bCurByte = 129 Then 
			
			'A soft-reset is found, so ask the user if they want to replace
			'it with a power-cycle.
			sMessage = "A soft-reset is present.  Do you want to change this to a power-cycle?"
			If MsgBox(sMessage, MsgBoxStyle.YesNo, "Change Reset to Power-Cycle?") = MsgBoxResult.Yes Then
				
				'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				ResetCheck = 2
				
			Else
				
				'The user is fine with a reset, so do nothing to the file.
				'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				ResetCheck = 4
				Exit Function
				
			End If
			
			'Still go through the file, since we may be shifting all the controller
			'data by 1 byte.
		Else
			
			'This movie has no reset or power-cycle, and a power-cycle
			'will be inserted at the beginning of the controller data.
			'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			ResetCheck = 3
			
		End If
		
		'Metadata and savestate is a straight copy
		'(except for new offset, which we'll overwrite later).
		
		'Get new filename to correct the mistake to.
		'UPGRADE_WARNING: Couldn't resolve default property of object GetFileName(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		FileName = GetFileName()
		NewFileName = VB.Left(FileName, Len(FileName) - 4) & "_FixedPowerCycle.fcm"
		
		'Open the new file to copy the fix to.
		On Error GoTo ResetCheckError
		FileOpen(2, NewFileName, OpenMode.Binary, , OpenShare.LockReadWrite)
		On Error GoTo 0
		
		'First, copy all the metadata and savestate.
		For lIndex = 1 To lControlOffset
			'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FileGet(1, bCurByte, lIndex)
			'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FilePut(2, bCurByte, lIndex)
		Next lIndex
		
		'Now, jump to the controller data.
		lIndex = lControlOffset
		
		'Prepare to insert a power-cycle command.
		bCurByte = 130
		
		'Insert the power-cycle.
		'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FilePut(2, bCurByte, lIndex)
		
		'If necessary, add 1 to the length of controller data
		'(to allow space for the power-cycle).
		'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		If ResetCheck = 3 Then
			'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FileGet(1, lControlLength, 21)
			'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FilePut(2, lControlLength + 1, 21)
		End If
		
		'Be sure to not re-write the old reset command if it was replaced
		'with a power-cycle.
		'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		If ResetCheck = 2 Then lControlOffset = lControlOffset + 1
		
		'Copy the controller data from the old file to the appropriate spot
		'in the new file.  Depending on the above circumstances, the
		'power-cycle may have moved everything after it forward 1 byte.
		For lIndex = lControlOffset To LOF(1)
			
			'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FileGet(1, bCurByte, lIndex)
			'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FilePut(2, bCurByte, lIndex + (ResetCheck - 2))
			
		Next lIndex
		
		Exit Function
		
ResetCheckError: 
		'Some file error.
		'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		ResetCheck = 5
		
	End Function
	
	'This function removes the savestate and replaces it with a generic
	'16-byte string that will allow the format to still play.  If the
	'movie file is not marked as "start-from-reset", the savestate will
	'not be removed.
	
	Private Function SaveState() As Object
		Dim lSaveOffset As Integer 'Offset of the savestate data given by the movie header.
		Dim lControlOffset As Integer 'Offset of the controller data given by the movie header.
		Dim lControlLength As Integer 'Length of the controller data given by the movie header.
		Dim bStartFromReset As Byte 'Byte from movie file that contains start-from data.
		Dim StartFromReset As Boolean 'Bit value from bStartFromReset byte.
		
		Dim lCurLong As Integer
		Dim lIndex As Integer
		Dim bCurByte As Byte
		
		Dim FileName As String
		Dim NewFileName As String
		
		On Error GoTo SaveStateError
		
		'Position 08h contains movie start-from data.
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, bStartFromReset, 9)
		
		'Second bit determines if movie starts from reset.
		StartFromReset = (((bStartFromReset \ 2) Mod 2) = 1)
		
		'If the movie does not start from reset, exit here.
		If Not StartFromReset Then
			
			'Movie is marked to start from savestate; do nothing and tell the user.
			'UPGRADE_WARNING: Couldn't resolve default property of object SaveState. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			SaveState = 2
			Exit Function
			
		End If
		
		'Get offsets from file header data.
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, lSaveOffset, 25)
		'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FileGet(1, lControlOffset, 29)
		
		'Add 1 to offsets so they make sense with documentation provided.
		lSaveOffset = lSaveOffset + 1
		lControlOffset = lControlOffset + 1
		
		'If there are less than 17 bytes between the savestate and controller
		'data, then it has no savestate to begin with.
		If lControlOffset - lSaveOffset < 17 Then
			
			'No savestate present, so this check is finished.
			'UPGRADE_WARNING: Couldn't resolve default property of object SaveState. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			SaveState = 1
			Exit Function
			
		End If
		
		'Metadata is a straight copy (except for new offset, which we'll overwrite later).
		
		'Get new filename to correct the mistake to.
		'UPGRADE_WARNING: Couldn't resolve default property of object GetFileName(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		FileName = GetFileName()
		NewFileName = VB.Left(FileName, Len(FileName) - 4) & "_FixedSavestate.fcm"
		
		'Open the new file to copy the fix to.
		On Error GoTo SaveStateError
		FileOpen(2, NewFileName, OpenMode.Binary, , OpenShare.LockReadWrite)
		On Error GoTo 0
		
		'First, copy all the metadata.
		For lIndex = 1 To lSaveOffset
			'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FileGet(1, bCurByte, lIndex)
			'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FilePut(2, bCurByte, lIndex)
		Next lIndex
		
		'Now write the new controller data offset, as the savestate offset + 16
		'(minus 1, due to incrementing for documentation consistency).
		'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FilePut(2, lSaveOffset + 15, 29)
		
		'Put sixteen bytes here which represent:
		'000054 46 43 53 00   savestate header (version: 0.0.00)
		lCurLong = 5456710
		'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FilePut(2, lCurLong, lIndex - 1)
		
		'000058 00 00 00 00   savestate length: 0 bytes
		lCurLong = 0
		'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FilePut(2, lCurLong, lIndex + 3)
		
		'00005C 00 00 00 00   savestate version: 0.0.00
		'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FilePut(2, lCurLong, lIndex + 7)
		
		'000060 00 00 00 00   end of savestate
		'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		FilePut(2, lCurLong, lIndex + 11)
		
		'Now, skip the savestate and jump to the controller data.
		lIndex = lControlOffset
		
		'Copy the controller data from the old file to the appropriate spot
		'in the new file, so there is no savestate before it.
		For lIndex = lControlOffset To LOF(1)
			
			'UPGRADE_WARNING: Get was upgraded to FileGet and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FileGet(1, bCurByte, lIndex)
			'UPGRADE_WARNING: Put was upgraded to FilePut and has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
			FilePut(2, bCurByte, (lIndex - (lControlOffset - lSaveOffset) + 16))
			
		Next lIndex
		
		Exit Function
		
SaveStateError: 
		'Some file error.
		'UPGRADE_WARNING: Couldn't resolve default property of object SaveState. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		SaveState = 5
		
	End Function
	
	'This routine runs when the "Check for Reset" button is clicked.
	'It's a wrapper routine that calls ResetCheck() after setting up the files.
	
	Private Sub cmdResetCheck_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdResetCheck.Click
		Dim lResult As Integer 'Result returned from ResetCheck function.
		Dim sResponse As String 'Response to show the user after checking the file.
		Dim lIndex As Integer 'File list index
		
		'If a file was not selected, do not try to check it.
		If FileList.Items.Count = 0 Then Exit Sub
		
		'Try to open the file.
		'UPGRADE_WARNING: Couldn't resolve default property of object FileOpen_Renamed(GetFileName). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		If FileOpen_Renamed(GetFileName) Then
			
			'The file is open, proceed with the SaveState check.
			'UPGRADE_WARNING: Couldn't resolve default property of object ResetCheck(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			lResult = ResetCheck()
			
			'Close the file.
			Call FileClose_Renamed()
			
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
					sResponse = "There was a File I/O error. #" & Str(Err.Number)
			End Select
		Else
			
			'There was an error trying to open the file.
			sResponse = "There was a File I/O error. #" & Str(Err.Number)
			
		End If
		
		'Send return message to the user.
		Call MsgBox(sResponse, MsgBoxStyle.OKOnly, "Checking for Reset")
		
		'Save which list entry is currently selected.
		lIndex = FileList.SelectedIndex
		
		'Refresh the file list.
		Call FileList.Refresh()
		
		'Set the selected list entry back to its previous value, unless
		'refreshing the file list caused that to be invalid.
		
		'If the File List is empty, do not highlight a value.
		If FileList.Items.Count = 0 Then
			FileList.SelectedIndex = -1
			
			'If the previous selection is too far down, highlight
			'the top entry.
		ElseIf lIndex > FileList.Items.Count Then 
			FileList.SelectedIndex = 0
			
			'Otherwise, restore the previous selection.
		Else
			FileList.SelectedIndex = lIndex
		End If
	End Sub
	
	'This routine runs when the "Validate Length" button is clicked.
	'It's a wrapper routine that calls LengthCheck() after setting up the files.
	
	Private Sub cmdLengthCheck_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdLengthCheck.Click
		Dim lResult As Integer 'Result returned from LengthCheck function.
		Dim sResponse As String 'Response to show the user after checking the file.
		Dim lIndex As Integer 'File list index
		
		'If a file was not selected, do not try to check it.
		If FileList.Items.Count = 0 Then Exit Sub
		
		'Try to open the file.
		'UPGRADE_WARNING: Couldn't resolve default property of object FileOpen_Renamed(GetFileName). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		If FileOpen_Renamed(GetFileName) Then
			
			'The file is open, proceed with the LengthCheck.
			'UPGRADE_WARNING: Couldn't resolve default property of object LengthCheck(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			lResult = LengthCheck()
			
			'Close the file.
			Call FileClose_Renamed()
			
			'Assign the user message based on the result of the LengthCheck.
			Select Case lResult
				Case 0
					sResponse = "The movie length is correct."
				Case 1
					sResponse = "The movie length was incorrect, and has been corrected."
				Case 5
					sResponse = "There was a File I/O error. #" & Str(Err.Number)
			End Select
		Else
			
			'There was an error trying to open the file.
			sResponse = "There was a File I/O error. #" & Str(Err.Number)
			
		End If
		
		'Send return message to the user.
		Call MsgBox(sResponse, MsgBoxStyle.OKOnly, "Validating Length")
		
		'Save which list entry is currently selected.
		lIndex = FileList.SelectedIndex
		
		'Refresh the file list.
		Call FileList.Refresh()
		
		'Set the selected list entry back to its previous value, unless
		'refreshing the file list caused that to be invalid.
		
		'If the File List is empty, do not highlight a value.
		If FileList.Items.Count = 0 Then
			FileList.SelectedIndex = -1
			
			'If the previous selection is too far down, highlight
			'the top entry.
		ElseIf lIndex > FileList.Items.Count Then 
			FileList.SelectedIndex = 0
			
			'Otherwise, restore the previous selection.
		Else
			FileList.SelectedIndex = lIndex
		End If
	End Sub
	
	'This routine runs when the "Remove Savestate" & vbCrLf & "Check for Reset" button is clicked.
	'It's a wrapper routine that calls SaveState() after setting up the files.
	
	Private Sub cmdSaveState_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdSaveState.Click
		Dim lResult As Integer 'Result returned from SaveState function.
		Dim sResponse As String 'Response to show the user after checking the file.
		Dim lIndex As Integer 'File list index
		
		'If a file was not selected, do not try to check it.
		If FileList.Items.Count = 0 Then Exit Sub
		
		'Try to open the file.
		'UPGRADE_WARNING: Couldn't resolve default property of object FileOpen_Renamed(GetFileName). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		If FileOpen_Renamed(GetFileName) Then
			
			'The file is open, proceed with the SaveState check.
			'UPGRADE_WARNING: Couldn't resolve default property of object SaveState(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			lResult = SaveState()
			
			'Close the file.
			Call FileClose_Renamed()
			
			'Assign the user message based on the result of the LengthCheck.
			Select Case lResult
				Case 0
					sResponse = "There was a savestate present, which has been removed."
				Case 1
					sResponse = "There was no savestate present."
				Case 2
					sResponse = "There is a savestate present, but the movie is marked to start from it." & vbCrLf & "No changes were made to the file."
				Case 5
					sResponse = "There was a File I/O error. #" & Str(Err.Number)
			End Select
		Else
			
			'There was an error trying to open the file.
			sResponse = "There was a File I/O error. #" & Str(Err.Number)
			
		End If
		
		'Send return message to the user.
		Call MsgBox(sResponse, MsgBoxStyle.OKOnly, "Removing Savestate")
		
		'Save which list entry is currently selected.
		lIndex = FileList.SelectedIndex
		
		'Refresh the file list.
		Call FileList.Refresh()
		
		'Set the selected list entry back to its previous value, unless
		'refreshing the file list caused that to be invalid.
		
		'If the File List is empty, do not highlight a value.
		If FileList.Items.Count = 0 Then
			FileList.SelectedIndex = -1
			
			'If the previous selection is too far down, highlight
			'the top entry.
		ElseIf lIndex > FileList.Items.Count Then 
			FileList.SelectedIndex = 0
			
			'Otherwise, restore the previous selection.
		Else
			FileList.SelectedIndex = lIndex
		End If
		
	End Sub
	
	'Enable or disable appropriate buttons based on
	'whether a file is available to check.
	
	Private Sub CheckEnabled()
		
		If FileList.Items.Count = 0 Then
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
	
	Private Function GetFileName() As Object
		If FileList.Items.Count = 0 Then
			'UPGRADE_WARNING: Couldn't resolve default property of object GetFileName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			GetFileName = ""
			Exit Function
		End If
		
		If VB.Right(FileList.Path, 1) = "\" Then
			'UPGRADE_WARNING: Couldn't resolve default property of object GetFileName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			GetFileName = DirList.Path & FileList.FileName
		Else
			'UPGRADE_WARNING: Couldn't resolve default property of object GetFileName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			GetFileName = DirList.Path & "\" & FileList.FileName
		End If
		
	End Function
	
	'UPGRADE_NOTE: FileOpen was upgraded to FileOpen_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Private Function FileOpen_Renamed(ByRef FileName As String) As Object
		On Error GoTo FileOpenError
		
		'Check for existence
		FileOpen(1, FileName, OpenMode.Input)
		FileClose(1)
		
		'Now open file for actual reading/writing
		FileOpen(1, FileName, OpenMode.Binary, , OpenShare.LockReadWrite)
		On Error GoTo 0
		'UPGRADE_WARNING: Couldn't resolve default property of object FileOpen_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		FileOpen_Renamed = True
		
		Exit Function
		
FileOpenError: 
		'UPGRADE_WARNING: Couldn't resolve default property of object FileOpen_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		FileOpen_Renamed = False
		On Error GoTo 0
		
	End Function
	
	'Close all files that might be open
	'UPGRADE_NOTE: FileClose was upgraded to FileClose_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Private Sub FileClose_Renamed()
		FileClose(1, 2)
	End Sub
	
	'Upon loading, place the filename in the textbox
	'and enable or disable the appropriate buttons.
	
	Private Sub FCMFix_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		txtFileName.Text = DirList.Path & FileList.FileName
		Call CheckEnabled()
	End Sub
	
	Private Sub cmdExit_Click()
		Call Me.Close()
	End Sub
	
	Private Sub DirList_Change(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles DirList.Change
		
		On Error GoTo PathNotValid
		FileList.Path = DirList.Path
		On Error GoTo 0
		
		If FileList.Items.Count > 0 Then FileList.SelectedIndex = 0
		Call CheckEnabled()
		txtFileName.Text = DirList.Path & FileList.FileName
		
		Exit Sub
		
PathNotValid: 
		Call MsgBox("Error accessing path", MsgBoxStyle.OKOnly, "Error")
		
	End Sub
	
	Private Sub DriveList_SelectedIndexChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles DriveList.SelectedIndexChanged
		
		On Error GoTo DriveNotValid
		DirList.Path = DriveList.Drive
		On Error GoTo 0
		
		If FileList.Items.Count > 0 Then FileList.SelectedIndex = 0
		
		Call CheckEnabled()
		txtFileName.Text = DirList.Path & FileList.FileName
		
		Exit Sub
		
DriveNotValid: 
		Call MsgBox("Error accessing drive", MsgBoxStyle.OKOnly, "Error")
		
	End Sub
	
	Private Sub DirList_SelectedIndexChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles DirList.SelectedIndexChanged
		Call CheckEnabled()
	End Sub
	
	Private Sub FileList_SelectedIndexChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles FileList.SelectedIndexChanged
		Call CheckEnabled()
		txtFileName.Text = DirList.Path & FileList.FileName
	End Sub
End Class