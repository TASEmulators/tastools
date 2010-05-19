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
            this.btnCancel = new System.Windows.Forms.Button();
            this.tabControl = new System.Windows.Forms.TabControl();
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
            this.tabControl.SuspendLayout();
            this.tabFind.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabReplace.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnFind
            // 
            this.btnFind.Location = new System.Drawing.Point(218, 21);
            this.btnFind.Name = "btnFind";
            this.btnFind.Size = new System.Drawing.Size(77, 21);
            this.btnFind.TabIndex = 6;
            this.btnFind.Text = "&Find next";
            this.btnFind.UseVisualStyleBackColor = true;
            this.btnFind.Click += new System.EventHandler(this.btnFind_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(218, 48);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(77, 21);
            this.btnCancel.TabIndex = 7;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabFind);
            this.tabControl.Controls.Add(this.tabReplace);
            this.tabControl.Location = new System.Drawing.Point(8, 8);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(322, 160);
            this.tabControl.TabIndex = 0;
            this.tabControl.SelectedIndexChanged += new System.EventHandler(this.tabControl_SelectedIndexChanged);
            // 
            // tabFind
            // 
            this.tabFind.Controls.Add(this.groupBox1);
            this.tabFind.Controls.Add(this.btnCancel);
            this.tabFind.Controls.Add(this.txtFind);
            this.tabFind.Controls.Add(this.btnFind);
            this.tabFind.Controls.Add(this.lblFind);
            this.tabFind.Location = new System.Drawing.Point(4, 22);
            this.tabFind.Name = "tabFind";
            this.tabFind.Padding = new System.Windows.Forms.Padding(3);
            this.tabFind.Size = new System.Drawing.Size(314, 134);
            this.tabFind.TabIndex = 0;
            this.tabFind.Text = "Find";
            this.tabFind.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radDown);
            this.groupBox1.Controls.Add(this.radUp);
            this.groupBox1.Location = new System.Drawing.Point(21, 69);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(165, 42);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Search direction";
            // 
            // radDown
            // 
            this.radDown.AutoSize = true;
            this.radDown.Checked = true;
            this.radDown.Location = new System.Drawing.Point(81, 20);
            this.radDown.Name = "radDown";
            this.radDown.Size = new System.Drawing.Size(53, 17);
            this.radDown.TabIndex = 5;
            this.radDown.TabStop = true;
            this.radDown.Text = "Down";
            this.radDown.UseVisualStyleBackColor = true;
            // 
            // radUp
            // 
            this.radUp.AutoSize = true;
            this.radUp.Location = new System.Drawing.Point(20, 20);
            this.radUp.Name = "radUp";
            this.radUp.Size = new System.Drawing.Size(39, 17);
            this.radUp.TabIndex = 4;
            this.radUp.Text = "Up";
            this.radUp.UseVisualStyleBackColor = true;
            // 
            // txtFind
            // 
            this.txtFind.Location = new System.Drawing.Point(80, 21);
            this.txtFind.Name = "txtFind";
            this.txtFind.Size = new System.Drawing.Size(106, 20);
            this.txtFind.TabIndex = 2;
            this.txtFind.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtFind_KeyPress);
            // 
            // lblFind
            // 
            this.lblFind.AutoSize = true;
            this.lblFind.Location = new System.Drawing.Point(18, 24);
            this.lblFind.Name = "lblFind";
            this.lblFind.Size = new System.Drawing.Size(56, 13);
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
            this.tabReplace.Location = new System.Drawing.Point(4, 22);
            this.tabReplace.Name = "tabReplace";
            this.tabReplace.Padding = new System.Windows.Forms.Padding(3);
            this.tabReplace.Size = new System.Drawing.Size(314, 134);
            this.tabReplace.TabIndex = 1;
            this.tabReplace.Text = "Replace";
            this.tabReplace.UseVisualStyleBackColor = true;
            // 
            // btnReplaceCancel
            // 
            this.btnReplaceCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnReplaceCancel.Location = new System.Drawing.Point(218, 95);
            this.btnReplaceCancel.Name = "btnReplaceCancel";
            this.btnReplaceCancel.Size = new System.Drawing.Size(84, 21);
            this.btnReplaceCancel.TabIndex = 15;
            this.btnReplaceCancel.Text = "&Cancel";
            this.btnReplaceCancel.UseVisualStyleBackColor = true;
            this.btnReplaceCancel.Click += new System.EventHandler(this.btnReplaceCancel_Click);
            // 
            // btnReplace
            // 
            this.btnReplace.Location = new System.Drawing.Point(218, 41);
            this.btnReplace.Name = "btnReplace";
            this.btnReplace.Size = new System.Drawing.Size(84, 21);
            this.btnReplace.TabIndex = 13;
            this.btnReplace.Text = "&Replace";
            this.btnReplace.UseVisualStyleBackColor = true;
            this.btnReplace.Click += new System.EventHandler(this.btnReplace_Click);
            // 
            // btnReplaceFindNext
            // 
            this.btnReplaceFindNext.Location = new System.Drawing.Point(218, 15);
            this.btnReplaceFindNext.Name = "btnReplaceFindNext";
            this.btnReplaceFindNext.Size = new System.Drawing.Size(84, 21);
            this.btnReplaceFindNext.TabIndex = 12;
            this.btnReplaceFindNext.Text = "&Find next";
            this.btnReplaceFindNext.UseVisualStyleBackColor = true;
            this.btnReplaceFindNext.Click += new System.EventHandler(this.btnReplaceFindNext_Click);
            // 
            // btnReplaceAll
            // 
            this.btnReplaceAll.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnReplaceAll.Location = new System.Drawing.Point(218, 68);
            this.btnReplaceAll.Name = "btnReplaceAll";
            this.btnReplaceAll.Size = new System.Drawing.Size(84, 21);
            this.btnReplaceAll.TabIndex = 14;
            this.btnReplaceAll.Text = "Replace &All";
            this.btnReplaceAll.UseVisualStyleBackColor = true;
            this.btnReplaceAll.Click += new System.EventHandler(this.btnReplaceAll_Click);
            // 
            // txtReplaceWith
            // 
            this.txtReplaceWith.Location = new System.Drawing.Point(89, 46);
            this.txtReplaceWith.Name = "txtReplaceWith";
            this.txtReplaceWith.Size = new System.Drawing.Size(94, 20);
            this.txtReplaceWith.TabIndex = 9;
            this.txtReplaceWith.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtReplaceWith_KeyPress);
            // 
            // lblReplaceWith
            // 
            this.lblReplaceWith.AutoSize = true;
            this.lblReplaceWith.Location = new System.Drawing.Point(30, 49);
            this.lblReplaceWith.Name = "lblReplaceWith";
            this.lblReplaceWith.Size = new System.Drawing.Size(29, 13);
            this.lblReplaceWith.TabIndex = 9;
            this.lblReplaceWith.Text = "with:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.radDownReplace);
            this.groupBox2.Controls.Add(this.radUpReplace);
            this.groupBox2.Location = new System.Drawing.Point(12, 77);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(171, 45);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Search direction";
            // 
            // radDownReplace
            // 
            this.radDownReplace.AutoSize = true;
            this.radDownReplace.Checked = true;
            this.radDownReplace.Location = new System.Drawing.Point(88, 19);
            this.radDownReplace.Name = "radDownReplace";
            this.radDownReplace.Size = new System.Drawing.Size(53, 17);
            this.radDownReplace.TabIndex = 11;
            this.radDownReplace.TabStop = true;
            this.radDownReplace.Text = "Down";
            this.radDownReplace.UseVisualStyleBackColor = true;
            // 
            // radUpReplace
            // 
            this.radUpReplace.AutoSize = true;
            this.radUpReplace.Location = new System.Drawing.Point(21, 19);
            this.radUpReplace.Name = "radUpReplace";
            this.radUpReplace.Size = new System.Drawing.Size(39, 17);
            this.radUpReplace.TabIndex = 10;
            this.radUpReplace.Text = "Up";
            this.radUpReplace.UseVisualStyleBackColor = true;
            // 
            // txtReplace
            // 
            this.txtReplace.Location = new System.Drawing.Point(89, 16);
            this.txtReplace.Name = "txtReplace";
            this.txtReplace.Size = new System.Drawing.Size(94, 20);
            this.txtReplace.TabIndex = 8;
            this.txtReplace.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtReplace_KeyPress);
            // 
            // lblReplace
            // 
            this.lblReplace.AutoSize = true;
            this.lblReplace.Location = new System.Drawing.Point(22, 19);
            this.lblReplace.Name = "lblReplace";
            this.lblReplace.Size = new System.Drawing.Size(50, 13);
            this.lblReplace.TabIndex = 8;
            this.lblReplace.Text = "Replace:";
            // 
            // FindReplaceDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(335, 175);
            this.Controls.Add(this.tabControl);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FindReplaceDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Find/Replace";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.FindReplaceDialog_Load);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.FindReplaceDialog_KeyPress);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FindReplaceDialog_FormClosing);
            this.tabControl.ResumeLayout(false);
            this.tabFind.ResumeLayout(false);
            this.tabFind.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabReplace.ResumeLayout(false);
            this.tabReplace.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnFind;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.TabControl tabControl;
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
    }
}