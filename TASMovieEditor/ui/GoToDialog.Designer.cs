namespace MovieSplicer.UI
{
    partial class GoToDialog
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
            this.txtFrameNumber = new System.Windows.Forms.TextBox();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOk = new System.Windows.Forms.Button();
            this.lblFrameNumber = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // txtFrameNumber
            // 
            this.txtFrameNumber.Location = new System.Drawing.Point(111, 6);
            this.txtFrameNumber.MaxLength = 20;
            this.txtFrameNumber.Name = "txtFrameNumber";
            this.txtFrameNumber.Size = new System.Drawing.Size(76, 20);
            this.txtFrameNumber.TabIndex = 2;
            this.txtFrameNumber.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtFrameNumber_KeyPress);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(111, 38);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(65, 24);
            this.btnCancel.TabIndex = 4;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnOk
            // 
            this.btnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOk.Location = new System.Drawing.Point(33, 39);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(60, 23);
            this.btnOk.TabIndex = 3;
            this.btnOk.Text = "&OK";
            this.btnOk.UseVisualStyleBackColor = true;
            // 
            // lblFrameNumber
            // 
            this.lblFrameNumber.AutoSize = true;
            this.lblFrameNumber.Location = new System.Drawing.Point(12, 9);
            this.lblFrameNumber.Name = "lblFrameNumber";
            this.lblFrameNumber.Size = new System.Drawing.Size(48, 13);
            this.lblFrameNumber.TabIndex = 1;
            this.lblFrameNumber.Text = "Frame ():";
            this.lblFrameNumber.Click += new System.EventHandler(this.lblFrameNumber_Click);
            this.lblFrameNumber.Enter += new System.EventHandler(this.lblFrameNumber_Click);
            // 
            // GoToDialog
            // 
            this.AcceptButton = this.btnOk;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(209, 71);
            this.Controls.Add(this.txtFrameNumber);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOk);
            this.Controls.Add(this.lblFrameNumber);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "GoToDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Go To...";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.GoToDialog_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtFrameNumber;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.Label lblFrameNumber;
    }
}