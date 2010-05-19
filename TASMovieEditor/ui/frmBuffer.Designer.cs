namespace MovieSplicer.UI
{
    partial class frmBuffer
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmBuffer));
            this.btnClear = new System.Windows.Forms.Button();
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.sbarMovieType = new System.Windows.Forms.ToolStripStatusLabel();
            this.lvInputBuffer = new MovieSplicer.Components.TASListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.btnClose = new System.Windows.Forms.Button();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(307, 223);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(75, 23);
            this.btnClear.TabIndex = 4;
            this.btnClear.Text = "&Clear";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(2, 252);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(75, 23);
            this.btnLoad.TabIndex = 4;
            this.btnLoad.Text = "&Load";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(2, 223);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 4;
            this.btnSave.Text = "&Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.sbarMovieType});
            this.statusStrip1.Location = new System.Drawing.Point(0, 282);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(386, 22);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 5;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(94, 17);
            this.toolStripStatusLabel1.Text = "Buffer Data Type:";
            // 
            // sbarMovieType
            // 
            this.sbarMovieType.ForeColor = System.Drawing.Color.Blue;
            this.sbarMovieType.Name = "sbarMovieType";
            this.sbarMovieType.Size = new System.Drawing.Size(32, 17);
            this.sbarMovieType.Text = "None";
            // 
            // lvInputBuffer
            // 
            this.lvInputBuffer.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.lvInputBuffer.FullRowSelect = true;
            this.lvInputBuffer.GridLines = true;
            this.lvInputBuffer.Location = new System.Drawing.Point(2, 3);
            this.lvInputBuffer.MultiSelect = false;
            this.lvInputBuffer.Name = "lvInputBuffer";
            this.lvInputBuffer.Size = new System.Drawing.Size(380, 214);
            this.lvInputBuffer.TabIndex = 7;
            this.lvInputBuffer.UseCompatibleStateImageBehavior = false;
            this.lvInputBuffer.View = System.Windows.Forms.View.Details;
            this.lvInputBuffer.VirtualMode = true;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Frame";
            this.columnHeader1.Width = 376;
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(307, 252);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 8;
            this.btnClose.Text = "C&lose";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // frmBuffer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(386, 304);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.lvInputBuffer);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnClear);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmBuffer";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Copy Buffer";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel sbarMovieType;
        private MovieSplicer.Components.TASListView lvInputBuffer;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.Button btnClose;
    }
}