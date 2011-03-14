namespace MovieSplicer.UI
{
    partial class frmEditing
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmEditing));
            this.grpEditing = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.button4 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.txtFrameData = new System.Windows.Forms.TextBox();
            this.chkAppendInput = new System.Windows.Forms.CheckBox();
            this.chkFrameDataC4 = new System.Windows.Forms.CheckBox();
            this.chkFrameDataC3 = new System.Windows.Forms.CheckBox();
            this.chkFrameDataC2 = new System.Windows.Forms.CheckBox();
            this.txtFrameDataC3 = new System.Windows.Forms.TextBox();
            this.chkFrameDataC1 = new System.Windows.Forms.CheckBox();
            this.txtFrameDataC2 = new System.Windows.Forms.TextBox();
            this.txtFrameDataC4 = new System.Windows.Forms.TextBox();
            this.btnUpdate = new System.Windows.Forms.Button();
            this.txtFrameDataC1 = new System.Windows.Forms.TextBox();
            this.btnFindInput = new System.Windows.Forms.Button();
            this.btnGotoFrame = new System.Windows.Forms.Button();
            this.txtJumpToFrame = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.grpEditing.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpEditing
            // 
            this.grpEditing.Controls.Add(this.label2);
            this.grpEditing.Controls.Add(this.button4);
            this.grpEditing.Controls.Add(this.button3);
            this.grpEditing.Controls.Add(this.button2);
            this.grpEditing.Controls.Add(this.button1);
            this.grpEditing.Controls.Add(this.txtFrameData);
            this.grpEditing.Controls.Add(this.chkAppendInput);
            this.grpEditing.Controls.Add(this.chkFrameDataC4);
            this.grpEditing.Controls.Add(this.chkFrameDataC3);
            this.grpEditing.Controls.Add(this.chkFrameDataC2);
            this.grpEditing.Controls.Add(this.txtFrameDataC3);
            this.grpEditing.Controls.Add(this.chkFrameDataC1);
            this.grpEditing.Controls.Add(this.txtFrameDataC2);
            this.grpEditing.Controls.Add(this.txtFrameDataC4);
            this.grpEditing.Controls.Add(this.btnUpdate);
            this.grpEditing.Controls.Add(this.txtFrameDataC1);
            this.grpEditing.Location = new System.Drawing.Point(12, 11);
            this.grpEditing.Name = "grpEditing";
            this.grpEditing.Size = new System.Drawing.Size(350, 90);
            this.grpEditing.TabIndex = 10;
            this.grpEditing.TabStop = false;
            this.grpEditing.Text = "Input Editor";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(90, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(83, 12);
            this.label2.TabIndex = 15;
            this.label2.Text = "Controller ID";
            // 
            // button4
            // 
            this.button4.Enabled = false;
            this.button4.Location = new System.Drawing.Point(260, 69);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(78, 21);
            this.button4.TabIndex = 8;
            this.button4.Text = "Update #&4";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // button3
            // 
            this.button3.Enabled = false;
            this.button3.Location = new System.Drawing.Point(176, 69);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(78, 21);
            this.button3.TabIndex = 7;
            this.button3.Text = "Update #&3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // button2
            // 
            this.button2.Enabled = false;
            this.button2.Location = new System.Drawing.Point(92, 69);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(78, 21);
            this.button2.TabIndex = 6;
            this.button2.Text = "Update #&2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // button1
            // 
            this.button1.Enabled = false;
            this.button1.Location = new System.Drawing.Point(7, 69);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(78, 21);
            this.button1.TabIndex = 5;
            this.button1.Text = "Update #&1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // txtFrameData
            // 
            this.txtFrameData.Location = new System.Drawing.Point(7, 14);
            this.txtFrameData.Name = "txtFrameData";
            this.txtFrameData.Size = new System.Drawing.Size(79, 21);
            this.txtFrameData.TabIndex = 10;
            this.txtFrameData.DoubleClick += new System.EventHandler(this.txtFrameData_DoubleClick);
            // 
            // chkAppendInput
            // 
            this.chkAppendInput.AutoSize = true;
            this.chkAppendInput.Location = new System.Drawing.Point(243, -1);
            this.chkAppendInput.Name = "chkAppendInput";
            this.chkAppendInput.Size = new System.Drawing.Size(96, 16);
            this.chkAppendInput.TabIndex = 9;
            this.chkAppendInput.Text = "Append Input";
            this.chkAppendInput.UseVisualStyleBackColor = true;
            // 
            // chkFrameDataC4
            // 
            this.chkFrameDataC4.AutoSize = true;
            this.chkFrameDataC4.Enabled = false;
            this.chkFrameDataC4.Location = new System.Drawing.Point(200, 19);
            this.chkFrameDataC4.Name = "chkFrameDataC4";
            this.chkFrameDataC4.Size = new System.Drawing.Size(30, 16);
            this.chkFrameDataC4.TabIndex = 14;
            this.chkFrameDataC4.Text = "4";
            this.chkFrameDataC4.UseVisualStyleBackColor = true;
            // 
            // chkFrameDataC3
            // 
            this.chkFrameDataC3.AutoSize = true;
            this.chkFrameDataC3.Enabled = false;
            this.chkFrameDataC3.Location = new System.Drawing.Point(164, 19);
            this.chkFrameDataC3.Name = "chkFrameDataC3";
            this.chkFrameDataC3.Size = new System.Drawing.Size(30, 16);
            this.chkFrameDataC3.TabIndex = 13;
            this.chkFrameDataC3.Text = "3";
            this.chkFrameDataC3.UseVisualStyleBackColor = true;
            // 
            // chkFrameDataC2
            // 
            this.chkFrameDataC2.AutoSize = true;
            this.chkFrameDataC2.Enabled = false;
            this.chkFrameDataC2.Location = new System.Drawing.Point(128, 19);
            this.chkFrameDataC2.Name = "chkFrameDataC2";
            this.chkFrameDataC2.Size = new System.Drawing.Size(30, 16);
            this.chkFrameDataC2.TabIndex = 12;
            this.chkFrameDataC2.Text = "2";
            this.chkFrameDataC2.UseVisualStyleBackColor = true;
            // 
            // txtFrameDataC3
            // 
            this.txtFrameDataC3.Enabled = false;
            this.txtFrameDataC3.Location = new System.Drawing.Point(176, 42);
            this.txtFrameDataC3.Name = "txtFrameDataC3";
            this.txtFrameDataC3.Size = new System.Drawing.Size(79, 21);
            this.txtFrameDataC3.TabIndex = 2;
            this.txtFrameDataC3.DoubleClick += new System.EventHandler(this.txtFrameDataC3_DoubleClick);
            this.txtFrameDataC3.EnabledChanged += new System.EventHandler(this.txtFrameDataC3_EnabledChanged);
            // 
            // chkFrameDataC1
            // 
            this.chkFrameDataC1.AutoSize = true;
            this.chkFrameDataC1.Enabled = false;
            this.chkFrameDataC1.Location = new System.Drawing.Point(92, 19);
            this.chkFrameDataC1.Name = "chkFrameDataC1";
            this.chkFrameDataC1.Size = new System.Drawing.Size(30, 16);
            this.chkFrameDataC1.TabIndex = 11;
            this.chkFrameDataC1.Text = "1";
            this.chkFrameDataC1.UseVisualStyleBackColor = true;
            // 
            // txtFrameDataC2
            // 
            this.txtFrameDataC2.Enabled = false;
            this.txtFrameDataC2.Location = new System.Drawing.Point(91, 42);
            this.txtFrameDataC2.Name = "txtFrameDataC2";
            this.txtFrameDataC2.Size = new System.Drawing.Size(79, 21);
            this.txtFrameDataC2.TabIndex = 1;
            this.txtFrameDataC2.DoubleClick += new System.EventHandler(this.txtFrameDataC2_DoubleClick);
            this.txtFrameDataC2.EnabledChanged += new System.EventHandler(this.txtFrameDataC2_EnabledChanged);
            // 
            // txtFrameDataC4
            // 
            this.txtFrameDataC4.Enabled = false;
            this.txtFrameDataC4.Location = new System.Drawing.Point(261, 42);
            this.txtFrameDataC4.Name = "txtFrameDataC4";
            this.txtFrameDataC4.Size = new System.Drawing.Size(79, 21);
            this.txtFrameDataC4.TabIndex = 3;
            this.txtFrameDataC4.DoubleClick += new System.EventHandler(this.txtFrameDataC4_DoubleClick);
            this.txtFrameDataC4.EnabledChanged += new System.EventHandler(this.txtFrameDataC4_EnabledChanged);
            // 
            // btnUpdate
            // 
            this.btnUpdate.Location = new System.Drawing.Point(236, 16);
            this.btnUpdate.Name = "btnUpdate";
            this.btnUpdate.Size = new System.Drawing.Size(104, 21);
            this.btnUpdate.TabIndex = 19;
            this.btnUpdate.Text = "&Update Checked";
            this.btnUpdate.UseVisualStyleBackColor = true;
            this.btnUpdate.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // txtFrameDataC1
            // 
            this.txtFrameDataC1.Enabled = false;
            this.txtFrameDataC1.Location = new System.Drawing.Point(7, 42);
            this.txtFrameDataC1.Name = "txtFrameDataC1";
            this.txtFrameDataC1.Size = new System.Drawing.Size(79, 21);
            this.txtFrameDataC1.TabIndex = 0;
            this.txtFrameDataC1.DoubleClick += new System.EventHandler(this.txtFrameDataC1_DoubleClick);
            this.txtFrameDataC1.EnabledChanged += new System.EventHandler(this.txtFrameDataC1_EnabledChanged);
            // 
            // btnFindInput
            // 
            this.btnFindInput.Location = new System.Drawing.Point(91, 63);
            this.btnFindInput.Name = "btnFindInput";
            this.btnFindInput.Size = new System.Drawing.Size(81, 21);
            this.btnFindInput.TabIndex = 31;
            this.btnFindInput.Text = "&Find Input";
            this.btnFindInput.UseVisualStyleBackColor = true;
            this.btnFindInput.Click += new System.EventHandler(this.btnFindInput_Click);
            // 
            // btnGotoFrame
            // 
            this.btnGotoFrame.Location = new System.Drawing.Point(6, 63);
            this.btnGotoFrame.Name = "btnGotoFrame";
            this.btnGotoFrame.Size = new System.Drawing.Size(79, 21);
            this.btnGotoFrame.TabIndex = 30;
            this.btnGotoFrame.Text = "&Goto Frame";
            this.btnGotoFrame.UseVisualStyleBackColor = true;
            this.btnGotoFrame.Click += new System.EventHandler(this.btnGo_Click);
            // 
            // txtJumpToFrame
            // 
            this.txtJumpToFrame.Location = new System.Drawing.Point(6, 34);
            this.txtJumpToFrame.MaxLength = 15;
            this.txtJumpToFrame.Name = "txtJumpToFrame";
            this.txtJumpToFrame.Size = new System.Drawing.Size(166, 21);
            this.txtJumpToFrame.TabIndex = 29;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnFindInput);
            this.groupBox1.Controls.Add(this.btnGotoFrame);
            this.groupBox1.Controls.Add(this.txtJumpToFrame);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(368, 11);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(182, 90);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Input Locator";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(215, 12);
            this.label1.TabIndex = 31;
            this.label1.Text = "Enter frame number or input pattern";
            // 
            // frmEditing
            // 
            this.AcceptButton = this.btnUpdate;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(560, 111);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.grpEditing);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmEditing";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Editing Console";
            this.Load += new System.EventHandler(this.frmEditing_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmEditing_FormClosing);
            this.grpEditing.ResumeLayout(false);
            this.grpEditing.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox grpEditing;
        private System.Windows.Forms.CheckBox chkFrameDataC4;
        private System.Windows.Forms.CheckBox chkFrameDataC3;
        private System.Windows.Forms.CheckBox chkFrameDataC2;
        private System.Windows.Forms.TextBox txtFrameDataC3;
        private System.Windows.Forms.CheckBox chkFrameDataC1;
        private System.Windows.Forms.TextBox txtFrameDataC2;
        private System.Windows.Forms.TextBox txtFrameDataC4;
        private System.Windows.Forms.Button btnUpdate;
        private System.Windows.Forms.TextBox txtFrameDataC1;
        private System.Windows.Forms.Button btnFindInput;
        private System.Windows.Forms.Button btnGotoFrame;
        private System.Windows.Forms.TextBox txtJumpToFrame;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox chkAppendInput;
        private System.Windows.Forms.TextBox txtFrameData;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
    }
}