namespace MovieSplicer.UI
{
    partial class frmCompare
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmCompare));
            this.btnLoadSource = new System.Windows.Forms.Button();
            this.btnLoadTarget = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.txtSource = new System.Windows.Forms.TextBox();
            this.txtTarget = new System.Windows.Forms.TextBox();
            this.btnProcess = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtSourceFrames = new System.Windows.Forms.TextBox();
            this.txtTargetFrames = new System.Windows.Forms.TextBox();
            this.txtSourceStart = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtSourceEnd = new System.Windows.Forms.TextBox();
            this.txtTargetEnd = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtTargetStart = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.lvOutput = new MovieSplicer.Components.TASCompareListView();
            this.SuspendLayout();
            // 
            // btnLoadSource
            // 
            this.btnLoadSource.Location = new System.Drawing.Point(12, 377);
            this.btnLoadSource.Name = "btnLoadSource";
            this.btnLoadSource.Size = new System.Drawing.Size(101, 23);
            this.btnLoadSource.TabIndex = 32;
            this.btnLoadSource.Text = "Load &Source";
            this.btnLoadSource.UseVisualStyleBackColor = true;
            this.btnLoadSource.Click += new System.EventHandler(this.btnLoadSource_Click);
            // 
            // btnLoadTarget
            // 
            this.btnLoadTarget.Location = new System.Drawing.Point(12, 406);
            this.btnLoadTarget.Name = "btnLoadTarget";
            this.btnLoadTarget.Size = new System.Drawing.Size(101, 23);
            this.btnLoadTarget.TabIndex = 32;
            this.btnLoadTarget.Text = "Load &Target";
            this.btnLoadTarget.UseVisualStyleBackColor = true;
            this.btnLoadTarget.Click += new System.EventHandler(this.btnLoadTarget_Click);
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(687, 407);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(101, 23);
            this.btnClear.TabIndex = 33;
            this.btnClear.Text = "&Clear";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // txtSource
            // 
            this.txtSource.Location = new System.Drawing.Point(119, 379);
            this.txtSource.Name = "txtSource";
            this.txtSource.ReadOnly = true;
            this.txtSource.Size = new System.Drawing.Size(254, 20);
            this.txtSource.TabIndex = 34;
            // 
            // txtTarget
            // 
            this.txtTarget.Location = new System.Drawing.Point(119, 408);
            this.txtTarget.Name = "txtTarget";
            this.txtTarget.ReadOnly = true;
            this.txtTarget.Size = new System.Drawing.Size(254, 20);
            this.txtTarget.TabIndex = 35;
            // 
            // btnProcess
            // 
            this.btnProcess.Location = new System.Drawing.Point(687, 378);
            this.btnProcess.Name = "btnProcess";
            this.btnProcess.Size = new System.Drawing.Size(101, 23);
            this.btnProcess.TabIndex = 36;
            this.btnProcess.Text = "&Process";
            this.btnProcess.UseVisualStyleBackColor = true;
            this.btnProcess.Click += new System.EventHandler(this.btnProcess_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(379, 382);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 13);
            this.label1.TabIndex = 37;
            this.label1.Text = "Frames";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(379, 411);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 13);
            this.label2.TabIndex = 38;
            this.label2.Text = "Frames";
            // 
            // txtSourceFrames
            // 
            this.txtSourceFrames.Location = new System.Drawing.Point(426, 380);
            this.txtSourceFrames.Name = "txtSourceFrames";
            this.txtSourceFrames.ReadOnly = true;
            this.txtSourceFrames.Size = new System.Drawing.Size(81, 20);
            this.txtSourceFrames.TabIndex = 39;
            // 
            // txtTargetFrames
            // 
            this.txtTargetFrames.Location = new System.Drawing.Point(426, 409);
            this.txtTargetFrames.Name = "txtTargetFrames";
            this.txtTargetFrames.ReadOnly = true;
            this.txtTargetFrames.Size = new System.Drawing.Size(81, 20);
            this.txtTargetFrames.TabIndex = 40;
            // 
            // txtSourceStart
            // 
            this.txtSourceStart.Location = new System.Drawing.Point(557, 380);
            this.txtSourceStart.Name = "txtSourceStart";
            this.txtSourceStart.Size = new System.Drawing.Size(51, 20);
            this.txtSourceStart.TabIndex = 42;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(513, 382);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(39, 13);
            this.label3.TabIndex = 41;
            this.label3.Text = "Range";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(612, 383);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(16, 13);
            this.label4.TabIndex = 43;
            this.label4.Text = "->";
            // 
            // txtSourceEnd
            // 
            this.txtSourceEnd.Location = new System.Drawing.Point(630, 380);
            this.txtSourceEnd.Name = "txtSourceEnd";
            this.txtSourceEnd.Size = new System.Drawing.Size(51, 20);
            this.txtSourceEnd.TabIndex = 44;
            // 
            // txtTargetEnd
            // 
            this.txtTargetEnd.Location = new System.Drawing.Point(630, 408);
            this.txtTargetEnd.Name = "txtTargetEnd";
            this.txtTargetEnd.Size = new System.Drawing.Size(51, 20);
            this.txtTargetEnd.TabIndex = 48;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(612, 412);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(16, 13);
            this.label5.TabIndex = 47;
            this.label5.Text = "->";
            // 
            // txtTargetStart
            // 
            this.txtTargetStart.Location = new System.Drawing.Point(558, 408);
            this.txtTargetStart.Name = "txtTargetStart";
            this.txtTargetStart.Size = new System.Drawing.Size(51, 20);
            this.txtTargetStart.TabIndex = 46;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(513, 411);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(39, 13);
            this.label6.TabIndex = 45;
            this.label6.Text = "Range";
            // 
            // lvOutput
            // 
            this.lvOutput.FullRowSelect = true;
            this.lvOutput.GridLines = true;
            this.lvOutput.Location = new System.Drawing.Point(12, 12);
            this.lvOutput.MultiSelect = false;
            this.lvOutput.Name = "lvOutput";
            this.lvOutput.Size = new System.Drawing.Size(776, 362);
            this.lvOutput.TabIndex = 49;
            this.lvOutput.UseCompatibleStateImageBehavior = false;
            this.lvOutput.View = System.Windows.Forms.View.Details;
            this.lvOutput.VirtualMode = true;
            // 
            // frmCompare
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 442);
            this.Controls.Add(this.lvOutput);
            this.Controls.Add(this.txtTargetEnd);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.txtTargetStart);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.txtSourceEnd);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.txtSourceStart);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtTargetFrames);
            this.Controls.Add(this.txtSourceFrames);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnProcess);
            this.Controls.Add(this.txtTarget);
            this.Controls.Add(this.txtSource);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.btnLoadTarget);
            this.Controls.Add(this.btnLoadSource);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmCompare";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Compare Input";
            this.ResumeLayout(false);
            this.PerformLayout();

        }        

        #endregion

        private System.Windows.Forms.Button btnLoadSource;
        private System.Windows.Forms.Button btnLoadTarget;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.TextBox txtSource;
        private System.Windows.Forms.TextBox txtTarget;
        private System.Windows.Forms.Button btnProcess;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtSourceFrames;
        private System.Windows.Forms.TextBox txtTargetFrames;
        private System.Windows.Forms.TextBox txtSourceStart;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtSourceEnd;
        private System.Windows.Forms.TextBox txtTargetEnd;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtTargetStart;
        private System.Windows.Forms.Label label6;
        private MovieSplicer.Components.TASCompareListView lvOutput;
    }
}