<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class FCMFix
#Region "Windows Form Designer generated code "
	<System.Diagnostics.DebuggerNonUserCode()> Public Sub New()
		MyBase.New()
		'This call is required by the Windows Form Designer.
		InitializeComponent()
	End Sub
	'Form overrides dispose to clean up the component list.
	<System.Diagnostics.DebuggerNonUserCode()> Protected Overloads Overrides Sub Dispose(ByVal Disposing As Boolean)
		If Disposing Then
			If Not components Is Nothing Then
				components.Dispose()
			End If
		End If
		MyBase.Dispose(Disposing)
	End Sub
	'Required by the Windows Form Designer
	Private components As System.ComponentModel.IContainer
	Public ToolTip1 As System.Windows.Forms.ToolTip
	Public WithEvents cmdResetCheck As System.Windows.Forms.Button
	Public WithEvents txtFileName As System.Windows.Forms.TextBox
	Public WithEvents cmdLengthCheck As System.Windows.Forms.Button
	Public WithEvents DriveList As Microsoft.VisualBasic.Compatibility.VB6.DriveListBox
	Public WithEvents cmdSaveState As System.Windows.Forms.Button
	Public WithEvents FileList As Microsoft.VisualBasic.Compatibility.VB6.FileListBox
	Public WithEvents DirList As Microsoft.VisualBasic.Compatibility.VB6.DirListBox
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(FCMFix))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.cmdResetCheck = New System.Windows.Forms.Button
		Me.txtFileName = New System.Windows.Forms.TextBox
		Me.cmdLengthCheck = New System.Windows.Forms.Button
		Me.DriveList = New Microsoft.VisualBasic.Compatibility.VB6.DriveListBox
		Me.cmdSaveState = New System.Windows.Forms.Button
		Me.FileList = New Microsoft.VisualBasic.Compatibility.VB6.FileListBox
		Me.DirList = New Microsoft.VisualBasic.Compatibility.VB6.DirListBox
		Me.SuspendLayout()
		Me.ToolTip1.Active = True
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
		Me.Text = "FCM Fix v4"
		Me.ClientSize = New System.Drawing.Size(446, 207)
		Me.Location = New System.Drawing.Point(3, 22)
		Me.MaximizeBox = False
		Me.MinimizeBox = False
		Me.StartPosition = System.Windows.Forms.FormStartPosition.WindowsDefaultLocation
		Me.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.ShowInTaskbar = True
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "FCMFix"
		Me.cmdResetCheck.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdResetCheck.Text = "Check for &Reset or Power Cycle"
		Me.cmdResetCheck.Enabled = False
		Me.cmdResetCheck.Size = New System.Drawing.Size(106, 34)
		Me.cmdResetCheck.Location = New System.Drawing.Point(111, 132)
		Me.cmdResetCheck.TabIndex = 6
		Me.cmdResetCheck.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.cmdResetCheck.BackColor = System.Drawing.SystemColors.Control
		Me.cmdResetCheck.CausesValidation = True
		Me.cmdResetCheck.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdResetCheck.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdResetCheck.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdResetCheck.TabStop = True
		Me.cmdResetCheck.Name = "cmdResetCheck"
		Me.txtFileName.AutoSize = False
		Me.txtFileName.BackColor = System.Drawing.SystemColors.Control
		Me.txtFileName.Size = New System.Drawing.Size(439, 22)
		Me.txtFileName.Location = New System.Drawing.Point(3, 3)
		Me.txtFileName.ReadOnly = True
		Me.txtFileName.TabIndex = 5
		Me.txtFileName.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.txtFileName.AcceptsReturn = True
		Me.txtFileName.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtFileName.CausesValidation = True
		Me.txtFileName.Enabled = True
		Me.txtFileName.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtFileName.HideSelection = True
		Me.txtFileName.Maxlength = 0
		Me.txtFileName.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtFileName.MultiLine = False
		Me.txtFileName.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtFileName.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtFileName.TabStop = True
		Me.txtFileName.Visible = True
		Me.txtFileName.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtFileName.Name = "txtFileName"
		Me.cmdLengthCheck.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdLengthCheck.Text = "&Validate Length"
		Me.cmdLengthCheck.Enabled = False
		Me.cmdLengthCheck.Size = New System.Drawing.Size(103, 34)
		Me.cmdLengthCheck.Location = New System.Drawing.Point(3, 171)
		Me.cmdLengthCheck.TabIndex = 4
		Me.cmdLengthCheck.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.cmdLengthCheck.BackColor = System.Drawing.SystemColors.Control
		Me.cmdLengthCheck.CausesValidation = True
		Me.cmdLengthCheck.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdLengthCheck.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdLengthCheck.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdLengthCheck.TabStop = True
		Me.cmdLengthCheck.Name = "cmdLengthCheck"
		Me.DriveList.Size = New System.Drawing.Size(103, 21)
		Me.DriveList.Location = New System.Drawing.Point(3, 135)
		Me.DriveList.TabIndex = 3
		Me.DriveList.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.DriveList.BackColor = System.Drawing.SystemColors.Window
		Me.DriveList.CausesValidation = True
		Me.DriveList.Enabled = True
		Me.DriveList.ForeColor = System.Drawing.SystemColors.WindowText
		Me.DriveList.Cursor = System.Windows.Forms.Cursors.Default
		Me.DriveList.TabStop = True
		Me.DriveList.Visible = True
		Me.DriveList.Name = "DriveList"
		Me.cmdSaveState.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdSaveState.Text = "Remove &Savestate"
		Me.cmdSaveState.Enabled = False
		Me.cmdSaveState.Size = New System.Drawing.Size(106, 34)
		Me.cmdSaveState.Location = New System.Drawing.Point(111, 171)
		Me.cmdSaveState.TabIndex = 2
		Me.cmdSaveState.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.cmdSaveState.BackColor = System.Drawing.SystemColors.Control
		Me.cmdSaveState.CausesValidation = True
		Me.cmdSaveState.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdSaveState.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdSaveState.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdSaveState.TabStop = True
		Me.cmdSaveState.Name = "cmdSaveState"
		Me.FileList.Size = New System.Drawing.Size(220, 175)
		Me.FileList.Location = New System.Drawing.Point(222, 27)
		Me.FileList.Pattern = "*.fcm"
		Me.FileList.TabIndex = 1
		Me.FileList.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.FileList.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.FileList.Archive = True
		Me.FileList.BackColor = System.Drawing.SystemColors.Window
		Me.FileList.CausesValidation = True
		Me.FileList.Enabled = True
		Me.FileList.ForeColor = System.Drawing.SystemColors.WindowText
		Me.FileList.Hidden = False
		Me.FileList.Cursor = System.Windows.Forms.Cursors.Default
		Me.FileList.SelectionMode = System.Windows.Forms.SelectionMode.One
		Me.FileList.Normal = True
		Me.FileList.ReadOnly = True
		Me.FileList.System = False
		Me.FileList.TabStop = True
		Me.FileList.TopIndex = 0
		Me.FileList.Visible = True
		Me.FileList.Name = "FileList"
		Me.DirList.Size = New System.Drawing.Size(214, 96)
		Me.DirList.Location = New System.Drawing.Point(3, 27)
		Me.DirList.TabIndex = 0
		Me.DirList.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.DirList.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.DirList.BackColor = System.Drawing.SystemColors.Window
		Me.DirList.CausesValidation = True
		Me.DirList.Enabled = True
		Me.DirList.ForeColor = System.Drawing.SystemColors.WindowText
		Me.DirList.Cursor = System.Windows.Forms.Cursors.Default
		Me.DirList.TabStop = True
		Me.DirList.Visible = True
		Me.DirList.Name = "DirList"
		Me.Controls.Add(cmdResetCheck)
		Me.Controls.Add(txtFileName)
		Me.Controls.Add(cmdLengthCheck)
		Me.Controls.Add(DriveList)
		Me.Controls.Add(cmdSaveState)
		Me.Controls.Add(FileList)
		Me.Controls.Add(DirList)
		Me.ResumeLayout(False)
		Me.PerformLayout()
	End Sub
#End Region 
End Class