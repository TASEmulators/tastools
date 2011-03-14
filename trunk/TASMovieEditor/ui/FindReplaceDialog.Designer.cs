namespace MovieSplicer.UI
{
    partial class FindReplaceDialog
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
            this.btnFind = new System.Windows.Forms.Button();
            this.btnFindCancel = new System.Windows.Forms.Button();
            this.tagControl = new System.Windows.Forms.TabControl();
            this.tabFind = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radDown = new System.Windows.Forms.RadioButton();
            this.radUp = new System.Windows.Forms.RadioButton();
            this.txtFind = new System.Windows.Forms.TextBox();
            this.lblFind = new System.Windows.Forms.Label();
            this.tabReplace = new System.Windows.Forms.TabPage();
            this.btnReplaceCancel = new System.Windows.Forms.Button();
            this.btnReplace = new System.Windows.Forms.Button();
            this.btnReplaceFindNext = new System.Windows.Forms.Button();
            this.btnReplaceAll = new System.Windows.Forms.Button();
            this.txtReplaceWith = new System.Windows.Forms.TextBox();
            this.lblReplaceWith = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.radDownReplace = new System.Windows.Forms.RadioButton();
            this.radUpReplace = new System.Windows.Forms.RadioButton();
            this.txtReplace = new System.Windows.Forms.TextBox();
            this.lblReplace = new System.Windows.Forms.Label();
            this.tagGoto = new System.Windows.Forms.TabPage();
            this.btnGotoCancel = new System.Windows.Forms.Button();
            this.btnGotoOk = new System.Windows.Forms.Button();
            this.lblFrameNumber = new System.Windows.Forms.Label();
            this.txtFrameNumber = new System.Windows.Forms.TextBox();
            this.tagControl.SuspendLayout();
            this.tabFind.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabReplace.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tagGoto.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnFind
            // 
            this.btnFind.Location = new System.Drawing.Point(218, 19);
            this.btnFind.Name = "btnFind";
            this.btnFind.Size = new System.Drawing.Size(77, 21);
            this.btnFind.TabIndex = 6;
            this.btnFind.Text = "&Find next";
            this.btnFind.UseVisualStyleBackColor = true;
            this.btnFind.Click += new System.EventHandler(this.btnFind_Click);
            // 
            // btnFindCancel
            // 
            this.btnFindCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnFindCancel.Location = new System.Drawing.Point(218, 46);
            this.btnFindCancel.Name = "btnFindCancel";
            this.btnFindCancel.Size = new System.Drawing.Size(77, 20);
            this.btnFindCancel.TabIndex = 7;
            this.btnFindCancel.Text = "&Cancel";
            this.btnFindCancel.UseVisualStyleBackColor = true;
            this.btnFindCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // tagControl
            // 
            this.tagControl.Controls.Add(this.tabFind);
            this.tagControl.Controls.Add(this.tabReplace);
            this.tagControl.Controls.Add(this.tagGoto);
            this.tagControl.Location = new System.Drawing.Point(12, 2);
            this.tagControl.Name = "tagControl";
            this.tagControl.SelectedIndex = 0;
            this.tagControl.Size = new System.Drawing.Size(322, 148);
            this.tagControl.TabIndex = 0;
            this.tagControl.SelectedIndexChanged += new System.EventHandler(this.tabControl_SelectedIndexChanged);
            // 
            // tabFind
            // 
            this.tabFind.Controls.Add(this.groupBox1);
            this.tabFind.Controls.Add(this.btnFindCancel);
            this.tabFind.Controls.Add(this.txtFind);
            this.tabFind.Controls.Add(this.btnFind);
            this.tabFind.Controls.Add(this.lblFind);
            this.tabFind.Location = new System.Drawing.Point(4, 21);
            this.tabFind.Name = "tabFind";
            this.tabFind.Padding = new System.Windows.Forms.Padding(3);
            this.tabFind.Size = new System.Drawing.Size(314, 123);
            this.tabFind.TabIndex = 0;
            this.tabFind.Text = "Find";
            this.tabFind.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radDown);
            this.groupBox1.Controls.Add(this.radUp);
            this.groupBox1.Location = new System.Drawing.Point(12, 69);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(172, 42);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Search direction";
            // 
            // radDown
            // 
            this.radDown.AutoSize = true;
            this.radDown.Checked = true;
            this.radDown.Location = new System.Drawing.Point(78, 18);
            this.radDown.Name = "radDown";
            this.radDown.Size = new System.Drawing.Size(47, 16);
            this.radDown.TabIndex = 5;
            this.radDown.TabStop = true;
            this.radDown.Text = "Down";
            this.radDown.UseVisualStyleBackColor = true;
            // 
            // radUp
            // 
            this.radUp.AutoSize = true;
            this.radUp.Location = new System.Drawing.Point(20, 18);
            this.radUp.Name = "radUp";
            this.radUp.Size = new System.Drawing.Size(35, 16);
            this.radUp.TabIndex = 4;
            this.radUp.Text = "Up";
            this.radUp.UseVisualStyleBackColor = true;
            // 
            // txtFind
            // 
            this.txtFind.Location = new System.Drawing.Point(81, 19);
            this.txtFind.Name = "txtFind";
            this.txtFind.Size = new System.Drawing.Size(105, 21);
            this.txtFind.TabIndex = 2;
            this.txtFind.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtFind_KeyPress);
            // 
            // lblFind
            // 
            this.lblFind.AutoSize = true;
            this.lblFind.Location = new System.Drawing.Point(10, 22);
            this.lblFind.Name = "lblFind";
            this.lblFind.Size = new System.Drawing.Size(65, 12);
            this.lblFind.TabIndex = 1;
            this.lblFind.Text = "Find what:";
            this.lblFind.Click += new System.EventHandler(this.lblFind_Click);
            // 
            // tabReplace
            // 
            this.tabReplace.Controls.Add(this.btnReplaceCancel);
            this.tabReplace.Controls.Add(this.btnReplace);
            this.tabReplace.Controls.Add(this.btnReplaceFindNext);
            this.tabReplace.Controls.Add(this.btnReplaceAll);
            this.tabReplace.Controls.Add(this.txtReplaceWith);
            this.tabReplace.Controls.Add(this.lblReplaceWith);
            this.tabReplace.Controls.Add(this.groupBox2);
            this.tabReplace.Controls.Add(this.txtReplace);
            this.tabReplace.Controls.Add(this.lblReplace);
            this.tabReplace.Location = new System.Drawing.Point(4, 21);
            this.tabReplace.Name = "tabReplace";
            this.tabReplace.Padding = new System.Windows.Forms.Padding(3);
            this.tabReplace.Size = new System.Drawing.Size(314, 123);
            this.tabReplace.TabIndex = 1;
            this.tabReplace.Text = "Replace";
            this.tabReplace.UseVisualStyleBackColor = true;
            // 
            // btnReplaceCancel
            // 
            this.btnReplaceCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnReplaceCancel.Location = new System.Drawing.Point(218, 88);
            this.btnReplaceCancel.Name = "btnReplaceCancel";
            this.btnReplaceCancel.Size = new System.Drawing.Size(84, 19);
            this.btnReplaceCancel.TabIndex = 15;
            this.btnReplaceCancel.Text = "&Cancel";
            this.btnReplaceCancel.UseVisualStyleBackColor = true;
            this.btnReplaceCancel.Click += new System.EventHandler(this.btnReplaceCancel_Click);
            // 
            // btnReplace
            // 
            this.btnReplace.Location = new System.Drawing.Point(218, 38);
            this.btnReplace.Name = "btnReplace";
            this.btnReplace.Size = new System.Drawing.Size(84, 19);
            this.btnReplace.TabIndex = 13;
            this.btnReplace.Text = "&Replace";
            this.btnReplace.UseVisualStyleBackColor = true;
            this.btnReplace.Click += new System.EventHandler(this.btnReplace_Click);
            // 
            // btnReplaceFindNext
            // 
            this.btnReplaceFindNext.Location = new System.Drawing.Point(218, 14);
            this.btnReplaceFindNext.Name = "btnReplaceFindNext";
            this.btnReplaceFindNext.Size = new System.Drawing.Size(84, 19);
            this.btnReplaceFindNext.TabIndex = 12;
            this.btnReplaceFindNext.Text = "&Find next";
            this.btnReplaceFindNext.UseVisualStyleBackColor = true;
            this.btnReplaceFindNext.Click += new System.EventHandler(this.btnReplaceFindNext_Click);
            // 
            // btnReplaceAll
            // 
            this.btnReplaceAll.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnReplaceAll.Location = new System.Drawing.Point(218, 63);
            this.btnReplaceAll.Name = "btnReplaceAll";
            this.btnReplaceAll.Size = new System.Drawing.Size(84, 19);
            this.btnReplaceAll.TabIndex = 14;
            this.btnReplaceAll.Text = "Replace &All";
            this.btnReplaceAll.UseVisualStyleBackColor = true;
            this.btnReplaceAll.Click += new System.EventHandler(this.btnReplaceAll_Click);
            // 
            // txtReplaceWith
            // 
            this.txtReplaceWith.Location = new System.Drawing.Point(89, 42);
            this.txtReplaceWith.Name = "txtReplaceWith";
            this.txtReplaceWith.Size = new System.Drawing.Size(95, 21);
            this.txtReplaceWith.TabIndex = 9;
            this.txtReplaceWith.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtReplaceWith_KeyPress);
            // 
            // lblReplaceWith
            // 
            this.lblReplaceWith.AutoSize = true;
            this.lblReplaceWith.Location = new System.Drawing.Point(30, 45);
            this.lblReplaceWith.Name = "lblReplaceWith";
            this.lblReplaceWith.Size = new System.Drawing.Size(35, 12);
            this.lblReplaceWith.TabIndex = 9;
            this.lblReplaceWith.Text = "with:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.radDownReplace);
            this.groupBox2.Controls.Add(this.radUpReplace);
            this.groupBox2.Location = new System.Drawing.Point(12, 69);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(172, 42);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Search direction";
            // 
            // radDownReplace
            // 
            this.radDownReplace.AutoSize = true;
            this.radDownReplace.Checked = true;
            this.radDownReplace.Location = new System.Drawing.Point(78, 18);
            this.radDownReplace.Name = "radDownReplace";
            this.radDownReplace.Size = new System.Drawing.Size(47, 16);
            this.radDownReplace.TabIndex = 11;
            this.radDownReplace.TabStop = true;
            this.radDownReplace.Text = "Down";
            this.radDownReplace.UseVisualStyleBackColor = true;
            // 
            // radUpReplace
            // 
            this.radUpReplace.AutoSize = true;
            this.radUpReplace.Location = new System.Drawing.Point(20, 18);
            this.radUpReplace.Name = "radUpReplace";
            this.radUpReplace.Size = new System.Drawing.Size(35, 16);
            this.radUpReplace.TabIndex = 10;
            this.radUpReplace.Text = "Up";
            this.radUpReplace.UseVisualStyleBackColor = true;
            // 
            // txtReplace
            // 
            this.txtReplace.Location = new System.Drawing.Point(89, 15);
            this.txtReplace.Name = "txtReplace";
            this.txtReplace.Size = new System.Drawing.Size(95, 21);
            this.txtReplace.TabIndex = 8;
            this.txtReplace.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtReplace_KeyPress);
            // 
            // lblReplace
            // 
            this.lblReplace.AutoSize = true;
            this.lblReplace.Location = new System.Drawing.Point(22, 18);
            this.lblReplace.Name = "lblReplace";
            this.lblReplace.Size = new System.Drawing.Size(53, 12);
            this.lblReplace.TabIndex = 8;
            this.lblReplace.Text = "Replace:";
            // 
            // tagGoto
            // 
            this.tagGoto.Controls.Add(this.btnGotoCancel);
            this.tagGoto.Controls.Add(this.btnGotoOk);
            this.tagGoto.Controls.Add(this.lblFrameNumber);
            this.tagGoto.Controls.Add(this.txtFrameNumber);
            this.tagGoto.Location = new System.Drawing.Point(4, 21);
            this.tagGoto.Name = "tagGoto";
            this.tagGoto.Padding = new System.Windows.Forms.Padding(3);
            this.tagGoto.Size = new System.Drawing.Size(314, 123);
            this.tagGoto.TabIndex = 2;
            this.tagGoto.Text = "Goto";
            this.tagGoto.UseVisualStyleBackColor = true;
            // 
            // btnGotoCancel
            // 
            this.btnGotoCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnGotoCancel.Location = new System.Drawing.Point(171, 86);
            this.btnGotoCancel.Name = "btnGotoCancel";
            this.btnGotoCancel.Size = new System.Drawing.Size(65, 22);
            this.btnGotoCancel.TabIndex = 6;
            this.btnGotoCancel.Text = "&Cancel";
            this.btnGotoCancel.UseVisualStyleBackColor = true;
            this.btnGotoCancel.Click += new System.EventHandler(this.btnGotoCancel_Click);
            // 
            // btnGotoOk
            // 
            this.btnGotoOk.Location = new System.Drawing.Point(81, 87);
            this.btnGotoOk.Name = "btnGotoOk";
            this.btnGotoOk.Size = new System.Drawing.Size(60, 21);
            this.btnGotoOk.TabIndex = 5;
            this.btnGotoOk.Text = "&Goto";
            this.btnGotoOk.UseVisualStyleBackColor = true;
            this.btnGotoOk.Click += new System.EventHandler(this.btnGoto_Click);
            // 
            // lblFrameNumber
            // 
            this.lblFrameNumber.AutoSize = true;
            this.lblFrameNumber.Location = new System.Drawing.Point(45, 31);
            this.lblFrameNumber.Name = "lblFrameNumber";
            this.lblFrameNumber.Size = new System.Drawing.Size(107, 12);
            this.lblFrameNumber.TabIndex = 4;
            this.lblFrameNumber.Text = "Frame (no movie):";
            // 
            // txtFrameNumber
            // 
            this.txtFrameNumber.Location = new System.Drawing.Point(171, 28);
            this.txtFrameNumber.MaxLength = 20;
            this.txtFrameNumber.Name = "txtFrameNumber";
            this.txtFrameNumber.Size = new System.Drawing.Size(103, 21);
            this.txtFrameNumber.TabIndex = 3;
            this.txtFrameNumber.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtFrameNumber_KeyPress);
            // 
            // FindReplaceDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(335, 162);
            this.Controls.Add(this.tagControl);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FindReplaceDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Find/Replace/Goto";
            this.Load += new System.EventHandler(this.FindReplaceDialog_Load);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.FindReplaceDialog_KeyPress);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FindReplaceDialog_FormClosing);
            this.tagControl.ResumeLayout(false);
            this.tabFind.ResumeLayout(false);
            this.tabFind.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabReplace.ResumeLayout(false);
            this.tabReplace.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.tagGoto.ResumeLayout(false);
            this.tagGoto.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnFind;
        private System.Windows.Forms.Button btnFindCancel;
        private System.Windows.Forms.TabControl tagControl;
        private System.Windows.Forms.TabPage tabFind;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton radDown;
        private System.Windows.Forms.RadioButton radUp;
        private System.Windows.Forms.TextBox txtFind;
        private System.Windows.Forms.Label lblFind;
        private System.Windows.Forms.TabPage tabReplace;
        private System.Windows.Forms.TextBox txtReplaceWith;
        private System.Windows.Forms.Label lblReplaceWith;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton radDownReplace;
        private System.Windows.Forms.RadioButton radUpReplace;
        private System.Windows.Forms.TextBox txtReplace;
        private System.Windows.Forms.Label lblReplace;
        private System.Windows.Forms.Button btnReplace;
        private System.Windows.Forms.Button btnReplaceFindNext;
        private System.Windows.Forms.Button btnReplaceAll;
        private System.Windows.Forms.Button btnReplaceCancel;
        private System.Windows.Forms.TabPage tagGoto;
        private System.Windows.Forms.TextBox txtFrameNumber;
        private System.Windows.Forms.Label lblFrameNumber;
        private System.Windows.Forms.Button btnGotoOk;
        private System.Windows.Forms.Button btnGotoCancel;
    }
}