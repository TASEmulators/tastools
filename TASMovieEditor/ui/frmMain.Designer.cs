namespace MovieSplicer.UI
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.mnuMain = new System.Windows.Forms.MenuStrip();
            this.mnuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuOpenRecent = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuClose = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuQuit = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuUndoChange = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuPasteBefore = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuPasteAfter = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuCopy = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.findToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findNextToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.replaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.goToToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.selectAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuEditing = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuInputDescriptions = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewBuffer = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuTools = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSplice = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuCompareMovieInputRange = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuExportSRT = new System.Windows.Forms.ToolStripMenuItem();
            this.runMovieGeneratorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuOptions = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuMessageHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuAutoFireOption = new System.Windows.Forms.ToolStripMenuItem();
            this.pasteOverwritesFromCurrentPositionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuEditingPrompt = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuContent = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuAbout = new System.Windows.Forms.ToolStripMenuItem();
            this.statMain = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.sbarFrameCount = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.sbarCopyBufferSize = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel4 = new System.Windows.Forms.ToolStripStatusLabel();
            this.sbarCopyBufferType = new System.Windows.Forms.ToolStripStatusLabel();
            this.cmnu_lvInput = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.cmnuitemInsertFrame = new System.Windows.Forms.ToolStripMenuItem();
            this.cmnuitemAppendFrame = new System.Windows.Forms.ToolStripMenuItem();
            this.cmnuitemRemoveFrames = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
            this.cmnuitemCopyFrames = new System.Windows.Forms.ToolStripMenuItem();
            this.cmnuitemPasteFramesBefore = new System.Windows.Forms.ToolStripMenuItem();
            this.cmnuitemPasteFramesAfter = new System.Windows.Forms.ToolStripMenuItem();
            this.tvInfo = new System.Windows.Forms.TreeView();
            this.grpMovieInfo = new System.Windows.Forms.GroupBox();
            this.pbFormat = new System.Windows.Forms.PictureBox();
            this.txtMovieFilename = new System.Windows.Forms.TextBox();
            this.grpFrameData = new System.Windows.Forms.GroupBox();
            this.lvInput = new MovieSplicer.Components.TASListView();
            this.cmnuitemCutFrames = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuCut = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuMain.SuspendLayout();
            this.statMain.SuspendLayout();
            this.cmnu_lvInput.SuspendLayout();
            this.grpMovieInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbFormat)).BeginInit();
            this.grpFrameData.SuspendLayout();
            this.SuspendLayout();
            // 
            // mnuMain
            // 
            this.mnuMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuFile,
            this.mnuEdit,
            this.toolStripMenuItem1,
            this.mnuTools,
            this.mnuOptions,
            this.mnuHelp});
            this.mnuMain.Location = new System.Drawing.Point(0, 0);
            this.mnuMain.Name = "mnuMain";
            this.mnuMain.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.mnuMain.Size = new System.Drawing.Size(842, 24);
            this.mnuMain.TabIndex = 7;
            // 
            // mnuFile
            // 
            this.mnuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuOpen,
            this.mnuOpenRecent,
            this.toolStripSeparator7,
            this.mnuSave,
            this.mnuSaveAs,
            this.mnuClose,
            this.toolStripSeparator1,
            this.mnuQuit});
            this.mnuFile.Name = "mnuFile";
            this.mnuFile.Size = new System.Drawing.Size(41, 20);
            this.mnuFile.Text = "&File";
            // 
            // mnuOpen
            // 
            this.mnuOpen.Name = "mnuOpen";
            this.mnuOpen.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.mnuOpen.Size = new System.Drawing.Size(153, 22);
            this.mnuOpen.Text = "&Open...";
            this.mnuOpen.Click += new System.EventHandler(this.mnuOpen_Click);
            // 
            // mnuOpenRecent
            // 
            this.mnuOpenRecent.Name = "mnuOpenRecent";
            this.mnuOpenRecent.Size = new System.Drawing.Size(153, 22);
            this.mnuOpenRecent.Text = "Open &Recent";
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(150, 6);
            // 
            // mnuSave
            // 
            this.mnuSave.Enabled = false;
            this.mnuSave.Name = "mnuSave";
            this.mnuSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.mnuSave.Size = new System.Drawing.Size(153, 22);
            this.mnuSave.Text = "&Save";
            this.mnuSave.Click += new System.EventHandler(this.mnuSave_Click);
            // 
            // mnuSaveAs
            // 
            this.mnuSaveAs.Enabled = false;
            this.mnuSaveAs.Name = "mnuSaveAs";
            this.mnuSaveAs.Size = new System.Drawing.Size(153, 22);
            this.mnuSaveAs.Text = "Save &As...";
            this.mnuSaveAs.Click += new System.EventHandler(this.mnuSaveAs_Click);
            // 
            // mnuClose
            // 
            this.mnuClose.Enabled = false;
            this.mnuClose.Name = "mnuClose";
            this.mnuClose.Size = new System.Drawing.Size(153, 22);
            this.mnuClose.Text = "&Close";
            this.mnuClose.Click += new System.EventHandler(this.mnuClose_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(150, 6);
            // 
            // mnuQuit
            // 
            this.mnuQuit.Name = "mnuQuit";
            this.mnuQuit.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Q)));
            this.mnuQuit.Size = new System.Drawing.Size(153, 22);
            this.mnuQuit.Text = "&Quit";
            this.mnuQuit.Click += new System.EventHandler(this.mnuExit_Click);
            // 
            // mnuEdit
            // 
            this.mnuEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuUndoChange,
            this.toolStripSeparator2,
            this.mnuCopy,
            this.mnuCut,
            this.mnuPasteBefore,
            this.mnuPasteAfter,
            this.toolStripSeparator8,
            this.findToolStripMenuItem,
            this.findNextToolStripMenuItem,
            this.replaceToolStripMenuItem,
            this.goToToolStripMenuItem,
            this.toolStripSeparator4,
            this.selectAllToolStripMenuItem});
            this.mnuEdit.Name = "mnuEdit";
            this.mnuEdit.Size = new System.Drawing.Size(41, 20);
            this.mnuEdit.Text = "&Edit";
            // 
            // mnuUndoChange
            // 
            this.mnuUndoChange.Enabled = false;
            this.mnuUndoChange.Name = "mnuUndoChange";
            this.mnuUndoChange.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.mnuUndoChange.Size = new System.Drawing.Size(213, 22);
            this.mnuUndoChange.Text = "&Undo";
            this.mnuUndoChange.Click += new System.EventHandler(this.mnuUndoChange_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(210, 6);
            // 
            // mnuPasteBefore
            // 
            this.mnuPasteBefore.Enabled = false;
            this.mnuPasteBefore.Name = "mnuPasteBefore";
            this.mnuPasteBefore.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.mnuPasteBefore.Size = new System.Drawing.Size(213, 22);
            this.mnuPasteBefore.Text = "&Paste Before";
            this.mnuPasteBefore.Click += new System.EventHandler(this.mnuPasteBefore_Click);
            // 
            // mnuPasteAfter
            // 
            this.mnuPasteAfter.Enabled = false;
            this.mnuPasteAfter.Name = "mnuPasteAfter";
            this.mnuPasteAfter.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.V)));
            this.mnuPasteAfter.Size = new System.Drawing.Size(213, 22);
            this.mnuPasteAfter.Text = "&Paste After";
            this.mnuPasteAfter.Click += new System.EventHandler(this.mnuPasteAfter_Click);
            // 
            // mnuCopy
            // 
            this.mnuCopy.Name = "mnuCopy";
            this.mnuCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.mnuCopy.Size = new System.Drawing.Size(213, 22);
            this.mnuCopy.Text = "&Copy Selection";
            this.mnuCopy.Click += new System.EventHandler(this.mnuCopy_Click);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(210, 6);
            // 
            // findToolStripMenuItem
            // 
            this.findToolStripMenuItem.Name = "findToolStripMenuItem";
            this.findToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
            this.findToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findToolStripMenuItem.Text = "&Find...";
            this.findToolStripMenuItem.Click += new System.EventHandler(this.findToolStripMenuItem_Click);
            // 
            // findNextToolStripMenuItem
            // 
            this.findNextToolStripMenuItem.Name = "findNextToolStripMenuItem";
            this.findNextToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F3;
            this.findNextToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findNextToolStripMenuItem.Text = "Find &Next";
            this.findNextToolStripMenuItem.Click += new System.EventHandler(this.findNextToolStripMenuItem_Click);
            // 
            // replaceToolStripMenuItem
            // 
            this.replaceToolStripMenuItem.Name = "replaceToolStripMenuItem";
            this.replaceToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.H)));
            this.replaceToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.replaceToolStripMenuItem.Text = "&Replace...";
            this.replaceToolStripMenuItem.Click += new System.EventHandler(this.replaceToolStripMenuItem_Click);
            // 
            // goToToolStripMenuItem
            // 
            this.goToToolStripMenuItem.Name = "goToToolStripMenuItem";
            this.goToToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.G)));
            this.goToToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.goToToolStripMenuItem.Text = "&Go To...";
            this.goToToolStripMenuItem.Click += new System.EventHandler(this.goToToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(210, 6);
            // 
            // selectAllToolStripMenuItem
            // 
            this.selectAllToolStripMenuItem.Name = "selectAllToolStripMenuItem";
            this.selectAllToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.selectAllToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.selectAllToolStripMenuItem.Text = "Select &All";
            this.selectAllToolStripMenuItem.Click += new System.EventHandler(this.selectAllToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuEditing,
            this.mnuInputDescriptions,
            this.mnuViewBuffer});
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(41, 20);
            this.toolStripMenuItem1.Text = "&View";
            // 
            // mnuEditing
            // 
            this.mnuEditing.Enabled = false;
            this.mnuEditing.Name = "mnuEditing";
            this.mnuEditing.ShortcutKeys = System.Windows.Forms.Keys.F2;
            this.mnuEditing.Size = new System.Drawing.Size(195, 22);
            this.mnuEditing.Text = "&Editing Console";
            this.mnuEditing.Click += new System.EventHandler(this.mnuEditing_Click);
            // 
            // mnuInputDescriptions
            // 
            this.mnuInputDescriptions.Enabled = false;
            this.mnuInputDescriptions.Name = "mnuInputDescriptions";
            this.mnuInputDescriptions.ShortcutKeys = System.Windows.Forms.Keys.F9;
            this.mnuInputDescriptions.Size = new System.Drawing.Size(195, 22);
            this.mnuInputDescriptions.Text = "Input Descriptions";
            // 
            // mnuViewBuffer
            // 
            this.mnuViewBuffer.Name = "mnuViewBuffer";
            this.mnuViewBuffer.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.B)));
            this.mnuViewBuffer.Size = new System.Drawing.Size(195, 22);
            this.mnuViewBuffer.Text = "Copy &Buffer";
            this.mnuViewBuffer.Click += new System.EventHandler(this.mnuViewBuffer_Click);
            // 
            // mnuTools
            // 
            this.mnuTools.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuSplice,
            this.mnuCompareMovieInputRange,
            this.toolStripSeparator6,
            this.mnuExportSRT,
            this.runMovieGeneratorToolStripMenuItem});
            this.mnuTools.Name = "mnuTools";
            this.mnuTools.Size = new System.Drawing.Size(47, 20);
            this.mnuTools.Text = "&Tools";
            // 
            // mnuSplice
            // 
            this.mnuSplice.Name = "mnuSplice";
            this.mnuSplice.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.mnuSplice.Size = new System.Drawing.Size(250, 22);
            this.mnuSplice.Text = "Movie &Splicer";
            this.mnuSplice.Click += new System.EventHandler(this.mnuSplice_Click);
            // 
            // mnuCompareMovieInputRange
            // 
            this.mnuCompareMovieInputRange.Name = "mnuCompareMovieInputRange";
            this.mnuCompareMovieInputRange.ShortcutKeys = System.Windows.Forms.Keys.F6;
            this.mnuCompareMovieInputRange.Size = new System.Drawing.Size(250, 22);
            this.mnuCompareMovieInputRange.Text = "&Compare Movie Input Range";
            this.mnuCompareMovieInputRange.Click += new System.EventHandler(this.mnuCompareMovieInputRange_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(247, 6);
            // 
            // mnuExportSRT
            // 
            this.mnuExportSRT.Enabled = false;
            this.mnuExportSRT.Name = "mnuExportSRT";
            this.mnuExportSRT.Size = new System.Drawing.Size(250, 22);
            this.mnuExportSRT.Text = "E&xport Frame Input as Subtitle";
            this.mnuExportSRT.Click += new System.EventHandler(this.mnuExportSRT_Click);
            // 
            // runMovieGeneratorToolStripMenuItem
            // 
            this.runMovieGeneratorToolStripMenuItem.Enabled = false;
            this.runMovieGeneratorToolStripMenuItem.Name = "runMovieGeneratorToolStripMenuItem";
            this.runMovieGeneratorToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F11;
            this.runMovieGeneratorToolStripMenuItem.Size = new System.Drawing.Size(250, 22);
            this.runMovieGeneratorToolStripMenuItem.Text = "Run Movie &Generator";
            this.runMovieGeneratorToolStripMenuItem.Click += new System.EventHandler(this.runMovieGeneratorToolStripMenuItem_Click);
            // 
            // mnuOptions
            // 
            this.mnuOptions.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuMessageHistory,
            this.toolStripSeparator5,
            this.mnuAutoFireOption,
            this.pasteOverwritesFromCurrentPositionToolStripMenuItem,
            this.mnuEditingPrompt});
            this.mnuOptions.Name = "mnuOptions";
            this.mnuOptions.Size = new System.Drawing.Size(59, 20);
            this.mnuOptions.Text = "&Options";
            // 
            // mnuMessageHistory
            // 
            this.mnuMessageHistory.Name = "mnuMessageHistory";
            this.mnuMessageHistory.ShortcutKeys = System.Windows.Forms.Keys.F4;
            this.mnuMessageHistory.Size = new System.Drawing.Size(208, 22);
            this.mnuMessageHistory.Text = "View &Message History";
            this.mnuMessageHistory.Click += new System.EventHandler(this.mnuMessageHistory_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(205, 6);
            // 
            // mnuAutoFireOption
            // 
            this.mnuAutoFireOption.CheckOnClick = true;
            this.mnuAutoFireOption.Name = "mnuAutoFireOption";
            this.mnuAutoFireOption.Size = new System.Drawing.Size(208, 22);
            this.mnuAutoFireOption.Text = "&Auto-Fire Update";
            this.mnuAutoFireOption.CheckStateChanged += new System.EventHandler(this.mnuAutoFireOption_CheckStateChanged);
            // 
            // pasteOverwritesFromCurrentPositionToolStripMenuItem
            // 
            this.pasteOverwritesFromCurrentPositionToolStripMenuItem.CheckOnClick = true;
            this.pasteOverwritesFromCurrentPositionToolStripMenuItem.Enabled = false;
            this.pasteOverwritesFromCurrentPositionToolStripMenuItem.Name = "pasteOverwritesFromCurrentPositionToolStripMenuItem";
            this.pasteOverwritesFromCurrentPositionToolStripMenuItem.Size = new System.Drawing.Size(208, 22);
            this.pasteOverwritesFromCurrentPositionToolStripMenuItem.Text = "&Overwrite on Paste";
            // 
            // mnuEditingPrompt
            // 
            this.mnuEditingPrompt.CheckOnClick = true;
            this.mnuEditingPrompt.Name = "mnuEditingPrompt";
            this.mnuEditingPrompt.Size = new System.Drawing.Size(208, 22);
            this.mnuEditingPrompt.Text = "&Confirm Editing Actions";
            this.mnuEditingPrompt.CheckStateChanged += new System.EventHandler(this.mnuEditingPrompt_CheckStateChanged);
            // 
            // mnuHelp
            // 
            this.mnuHelp.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.mnuHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuContent,
            this.toolStripSeparator9,
            this.mnuAbout});
            this.mnuHelp.Name = "mnuHelp";
            this.mnuHelp.Size = new System.Drawing.Size(41, 20);
            this.mnuHelp.Text = "&Help";
            // 
            // mnuContent
            // 
            this.mnuContent.Name = "mnuContent";
            this.mnuContent.Size = new System.Drawing.Size(130, 22);
            this.mnuContent.Text = "Content...";
            this.mnuContent.Click += new System.EventHandler(this.mnuContent_Click);
            // 
            // toolStripSeparator9
            // 
            this.toolStripSeparator9.Name = "toolStripSeparator9";
            this.toolStripSeparator9.Size = new System.Drawing.Size(127, 6);
            // 
            // mnuAbout
            // 
            this.mnuAbout.Name = "mnuAbout";
            this.mnuAbout.Size = new System.Drawing.Size(130, 22);
            this.mnuAbout.Text = "&About";
            this.mnuAbout.Click += new System.EventHandler(this.mnuAbout_Click);
            // 
            // statMain
            // 
            this.statMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.sbarFrameCount,
            this.toolStripStatusLabel3,
            this.sbarCopyBufferSize,
            this.toolStripStatusLabel4,
            this.sbarCopyBufferType});
            this.statMain.Location = new System.Drawing.Point(0, 355);
            this.statMain.Name = "statMain";
            this.statMain.Size = new System.Drawing.Size(842, 22);
            this.statMain.TabIndex = 8;
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(77, 17);
            this.toolStripStatusLabel1.Text = "Frame Count:";
            // 
            // sbarFrameCount
            // 
            this.sbarFrameCount.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Right;
            this.sbarFrameCount.BorderStyle = System.Windows.Forms.Border3DStyle.Raised;
            this.sbarFrameCount.ForeColor = System.Drawing.Color.Blue;
            this.sbarFrameCount.Name = "sbarFrameCount";
            this.sbarFrameCount.Size = new System.Drawing.Size(15, 17);
            this.sbarFrameCount.Text = "0";
            // 
            // toolStripStatusLabel3
            // 
            this.toolStripStatusLabel3.Name = "toolStripStatusLabel3";
            this.toolStripStatusLabel3.Size = new System.Drawing.Size(107, 17);
            this.toolStripStatusLabel3.Text = "Copy Buffer Size:";
            // 
            // sbarCopyBufferSize
            // 
            this.sbarCopyBufferSize.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Right;
            this.sbarCopyBufferSize.BorderStyle = System.Windows.Forms.Border3DStyle.Raised;
            this.sbarCopyBufferSize.ForeColor = System.Drawing.Color.Blue;
            this.sbarCopyBufferSize.Name = "sbarCopyBufferSize";
            this.sbarCopyBufferSize.Size = new System.Drawing.Size(15, 17);
            this.sbarCopyBufferSize.Text = "0";
            // 
            // toolStripStatusLabel4
            // 
            this.toolStripStatusLabel4.Name = "toolStripStatusLabel4";
            this.toolStripStatusLabel4.Size = new System.Drawing.Size(107, 17);
            this.toolStripStatusLabel4.Text = "Copy Buffer Type:";
            // 
            // sbarCopyBufferType
            // 
            this.sbarCopyBufferType.ForeColor = System.Drawing.Color.Blue;
            this.sbarCopyBufferType.Name = "sbarCopyBufferType";
            this.sbarCopyBufferType.Size = new System.Drawing.Size(29, 17);
            this.sbarCopyBufferType.Text = "None";
            // 
            // cmnu_lvInput
            // 
            this.cmnu_lvInput.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmnuitemInsertFrame,
            this.cmnuitemAppendFrame,
            this.cmnuitemRemoveFrames,
            this.toolStripSeparator3,
            this.undoToolStripMenuItem,
            this.toolStripSeparator10,
            this.cmnuitemCopyFrames,
            this.cmnuitemCutFrames,
            this.cmnuitemPasteFramesBefore,
            this.cmnuitemPasteFramesAfter});
            this.cmnu_lvInput.Name = "cmnuInput";
            this.cmnu_lvInput.Size = new System.Drawing.Size(268, 192);
            // 
            // cmnuitemInsertFrame
            // 
            this.cmnuitemInsertFrame.Name = "cmnuitemInsertFrame";
            this.cmnuitemInsertFrame.ShortcutKeys = System.Windows.Forms.Keys.Insert;
            this.cmnuitemInsertFrame.Size = new System.Drawing.Size(267, 22);
            this.cmnuitemInsertFrame.Text = "&Insert Frame(s)";
            this.cmnuitemInsertFrame.Click += new System.EventHandler(this.cmnuitemInsertFrame_Click);
            // 
            // cmnuitemAppendFrame
            // 
            this.cmnuitemAppendFrame.Name = "cmnuitemAppendFrame";
            this.cmnuitemAppendFrame.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Shift | System.Windows.Forms.Keys.Insert)));
            this.cmnuitemAppendFrame.Size = new System.Drawing.Size(267, 22);
            this.cmnuitemAppendFrame.Text = "&Append Frame(s)";
            this.cmnuitemAppendFrame.Click += new System.EventHandler(this.cmnuitemAppendFrame_Click);
            // 
            // cmnuitemRemoveFrames
            // 
            this.cmnuitemRemoveFrames.Name = "cmnuitemRemoveFrames";
            this.cmnuitemRemoveFrames.ShortcutKeys = System.Windows.Forms.Keys.Delete;
            this.cmnuitemRemoveFrames.Size = new System.Drawing.Size(267, 22);
            this.cmnuitemRemoveFrames.Text = "&Remove Frame(s)";
            this.cmnuitemRemoveFrames.Click += new System.EventHandler(this.cmnuitemRemoveFrames_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(264, 6);
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.undoToolStripMenuItem.Text = "&Undo";
            this.undoToolStripMenuItem.Click += new System.EventHandler(this.undoToolStripMenuItem_Click);
            // 
            // toolStripSeparator10
            // 
            this.toolStripSeparator10.Name = "toolStripSeparator10";
            this.toolStripSeparator10.Size = new System.Drawing.Size(264, 6);
            // 
            // cmnuitemCopyFrames
            // 
            this.cmnuitemCopyFrames.Name = "cmnuitemCopyFrames";
            this.cmnuitemCopyFrames.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.cmnuitemCopyFrames.Size = new System.Drawing.Size(267, 22);
            this.cmnuitemCopyFrames.Text = "&Copy Frame(s)";
            this.cmnuitemCopyFrames.Click += new System.EventHandler(this.cmnuitemCopyFrames_Click);
            // 
            // cmnuitemPasteFramesBefore
            // 
            this.cmnuitemPasteFramesBefore.Enabled = false;
            this.cmnuitemPasteFramesBefore.Name = "cmnuitemPasteFramesBefore";
            this.cmnuitemPasteFramesBefore.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.cmnuitemPasteFramesBefore.Size = new System.Drawing.Size(267, 22);
            this.cmnuitemPasteFramesBefore.Text = "&Paste Frame(s) Before";
            this.cmnuitemPasteFramesBefore.Click += new System.EventHandler(this.cmnuitemPasteFramesBefore_Click);
            // 
            // cmnuitemPasteFramesAfter
            // 
            this.cmnuitemPasteFramesAfter.Enabled = false;
            this.cmnuitemPasteFramesAfter.Name = "cmnuitemPasteFramesAfter";
            this.cmnuitemPasteFramesAfter.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.V)));
            this.cmnuitemPasteFramesAfter.Size = new System.Drawing.Size(267, 22);
            this.cmnuitemPasteFramesAfter.Text = "&Paste Frame(s) After";
            this.cmnuitemPasteFramesAfter.Click += new System.EventHandler(this.cmnuitemPasteFramesAfter_Click);
            // 
            // tvInfo
            // 
            this.tvInfo.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tvInfo.BackColor = System.Drawing.SystemColors.Window;
            this.tvInfo.Font = new System.Drawing.Font("Courier New", 8F);
            this.tvInfo.FullRowSelect = true;
            this.tvInfo.Location = new System.Drawing.Point(7, 51);
            this.tvInfo.Name = "tvInfo";
            this.tvInfo.Size = new System.Drawing.Size(400, 262);
            this.tvInfo.TabIndex = 27;
            // 
            // grpMovieInfo
            // 
            this.grpMovieInfo.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.grpMovieInfo.Controls.Add(this.pbFormat);
            this.grpMovieInfo.Controls.Add(this.txtMovieFilename);
            this.grpMovieInfo.Controls.Add(this.tvInfo);
            this.grpMovieInfo.Location = new System.Drawing.Point(4, 25);
            this.grpMovieInfo.Name = "grpMovieInfo";
            this.grpMovieInfo.Size = new System.Drawing.Size(417, 320);
            this.grpMovieInfo.TabIndex = 28;
            this.grpMovieInfo.TabStop = false;
            this.grpMovieInfo.Text = "Movie Information";
            // 
            // pbFormat
            // 
            this.pbFormat.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pbFormat.Location = new System.Drawing.Point(374, 16);
            this.pbFormat.Name = "pbFormat";
            this.pbFormat.Size = new System.Drawing.Size(32, 30);
            this.pbFormat.TabIndex = 31;
            this.pbFormat.TabStop = false;
            // 
            // txtMovieFilename
            // 
            this.txtMovieFilename.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMovieFilename.Location = new System.Drawing.Point(8, 16);
            this.txtMovieFilename.Multiline = true;
            this.txtMovieFilename.Name = "txtMovieFilename";
            this.txtMovieFilename.ReadOnly = true;
            this.txtMovieFilename.Size = new System.Drawing.Size(360, 30);
            this.txtMovieFilename.TabIndex = 29;
            this.txtMovieFilename.TabStop = false;
            // 
            // grpFrameData
            // 
            this.grpFrameData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.grpFrameData.Controls.Add(this.lvInput);
            this.grpFrameData.Location = new System.Drawing.Point(427, 25);
            this.grpFrameData.Name = "grpFrameData";
            this.grpFrameData.Size = new System.Drawing.Size(410, 320);
            this.grpFrameData.TabIndex = 29;
            this.grpFrameData.TabStop = false;
            this.grpFrameData.Text = "Frame Data";
            // 
            // lvInput
            // 
            this.lvInput.AllowDrop = true;
            this.lvInput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lvInput.ContextMenuStrip = this.cmnu_lvInput;
            this.lvInput.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lvInput.FullRowSelect = true;
            this.lvInput.GridLines = true;
            this.lvInput.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lvInput.HideSelection = false;
            this.lvInput.Location = new System.Drawing.Point(8, 16);
            this.lvInput.Name = "lvInput";
            this.lvInput.Size = new System.Drawing.Size(395, 297);
            this.lvInput.TabIndex = 26;
            this.lvInput.UseCompatibleStateImageBehavior = false;
            this.lvInput.View = System.Windows.Forms.View.Details;
            this.lvInput.VirtualMode = true;
            this.lvInput.DoubleClick += new System.EventHandler(this.mnuEditing_Click);
            this.lvInput.DragDrop += new System.Windows.Forms.DragEventHandler(this.lvInput_DragDrop);
            this.lvInput.DragEnter += new System.Windows.Forms.DragEventHandler(this.lvInput_DragEnter);
            this.lvInput.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.lvInput_KeyPress);
            this.lvInput.KeyUp += new System.Windows.Forms.KeyEventHandler(this.lvInput_KeyUp);
            this.lvInput.Click += new System.EventHandler(this.lvInput_Clicked);
            // 
            // cmnuitemCutFrames
            // 
            this.cmnuitemCutFrames.Name = "cmnuitemCutFrames";
            this.cmnuitemCutFrames.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.cmnuitemCutFrames.Size = new System.Drawing.Size(267, 22);
            this.cmnuitemCutFrames.Text = "Cu&t Frame(s)";
            this.cmnuitemCutFrames.Click += new System.EventHandler(this.cmnuitemCutFrames_Click);
            // 
            // mnuCut
            // 
            this.mnuCut.Name = "mnuCut";
            this.mnuCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.mnuCut.Size = new System.Drawing.Size(213, 22);
            this.mnuCut.Text = "Cu&t Selection";
            this.mnuCut.Click += new System.EventHandler(this.mnuCut_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(842, 377);
            this.Controls.Add(this.statMain);
            this.Controls.Add(this.grpMovieInfo);
            this.Controls.Add(this.grpFrameData);
            this.Controls.Add(this.mnuMain);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.mnuMain;
            this.MinimumSize = new System.Drawing.Size(850, 404);
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.mnuMain.ResumeLayout(false);
            this.mnuMain.PerformLayout();
            this.statMain.ResumeLayout(false);
            this.statMain.PerformLayout();
            this.cmnu_lvInput.ResumeLayout(false);
            this.grpMovieInfo.ResumeLayout(false);
            this.grpMovieInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbFormat)).EndInit();
            this.grpFrameData.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip mnuMain;
        private System.Windows.Forms.ToolStripMenuItem mnuFile;
        private System.Windows.Forms.ToolStripMenuItem mnuOpen;
        private System.Windows.Forms.ToolStripMenuItem mnuSave;
        private System.Windows.Forms.ToolStripMenuItem mnuSaveAs;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem mnuQuit;
        private System.Windows.Forms.ToolStripMenuItem mnuEdit;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem mnuClose;
        private System.Windows.Forms.StatusStrip statMain;
        private System.Windows.Forms.ContextMenuStrip cmnu_lvInput;
        private System.Windows.Forms.ToolStripMenuItem cmnuitemInsertFrame;
        private System.Windows.Forms.ToolStripMenuItem cmnuitemAppendFrame;
        private System.Windows.Forms.ToolStripMenuItem cmnuitemRemoveFrames;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel3;
        private System.Windows.Forms.ToolStripStatusLabel sbarCopyBufferSize;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel4;
        private System.Windows.Forms.ToolStripStatusLabel sbarCopyBufferType;
        private System.Windows.Forms.ToolStripMenuItem cmnuitemCopyFrames;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem cmnuitemPasteFramesBefore;
        private System.Windows.Forms.ToolStripMenuItem cmnuitemPasteFramesAfter;
        private System.Windows.Forms.ToolStripMenuItem mnuCopy;
        private System.Windows.Forms.ToolStripMenuItem mnuPasteBefore;
        private System.Windows.Forms.ToolStripMenuItem mnuPasteAfter;
        private MovieSplicer.Components.TASListView lvInput;
        private System.Windows.Forms.TreeView tvInfo;
        private System.Windows.Forms.GroupBox grpMovieInfo;
        private System.Windows.Forms.TextBox txtMovieFilename;
        private System.Windows.Forms.GroupBox grpFrameData;
        private System.Windows.Forms.ToolStripMenuItem mnuOptions;
        private System.Windows.Forms.ToolStripMenuItem mnuAutoFireOption;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel sbarFrameCount;
        private System.Windows.Forms.ToolStripMenuItem pasteOverwritesFromCurrentPositionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mnuMessageHistory;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem mnuEditingPrompt;
        private System.Windows.Forms.ToolStripMenuItem mnuHelp;
        private System.Windows.Forms.ToolStripMenuItem mnuUndoChange;
        private System.Windows.Forms.ToolStripMenuItem mnuTools;
        private System.Windows.Forms.ToolStripMenuItem mnuSplice;
        private System.Windows.Forms.ToolStripMenuItem mnuCompareMovieInputRange;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.PictureBox pbFormat;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripMenuItem mnuExportSRT;
        private System.Windows.Forms.ToolStripMenuItem mnuOpenRecent;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripMenuItem goToToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem runMovieGeneratorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findNextToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem replaceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem selectAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem mnuEditing;
        private System.Windows.Forms.ToolStripMenuItem mnuViewBuffer;
        private System.Windows.Forms.ToolStripMenuItem mnuContent;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
        private System.Windows.Forms.ToolStripMenuItem mnuAbout;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
        private System.Windows.Forms.ToolStripMenuItem mnuInputDescriptions;
        private System.Windows.Forms.ToolStripMenuItem cmnuitemCutFrames;
        private System.Windows.Forms.ToolStripMenuItem mnuCut;       
    }
}

