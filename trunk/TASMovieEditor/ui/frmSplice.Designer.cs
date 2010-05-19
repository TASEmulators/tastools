namespace MovieSplicer.UI
{
    partial class frmSplice
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmSplice));
            this.lvSplice = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.label1 = new System.Windows.Forms.Label();
            this.txtStart = new System.Windows.Forms.TextBox();
            this.txtEnd = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnUpdate = new System.Windows.Forms.Button();
            this.btnSplice = new System.Windows.Forms.Button();
            this.btnRemove = new System.Windows.Forms.Button();
            this.btnMoveUp = new System.Windows.Forms.Button();
            this.btnMoveDown = new System.Windows.Forms.Button();
            this.btnMoveStart = new System.Windows.Forms.Button();
            this.btnMoveEnd = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lvSplice
            // 
            this.lvSplice.AllowDrop = true;
            this.lvSplice.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader5,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.lvSplice.FullRowSelect = true;
            this.lvSplice.GridLines = true;
            this.lvSplice.HideSelection = false;
            this.lvSplice.Location = new System.Drawing.Point(51, 12);
            this.lvSplice.MultiSelect = false;
            this.lvSplice.Name = "lvSplice";
            this.lvSplice.Size = new System.Drawing.Size(511, 158);
            this.lvSplice.TabIndex = 0;
            this.lvSplice.UseCompatibleStateImageBehavior = false;
            this.lvSplice.View = System.Windows.Forms.View.Details;
            this.lvSplice.DragEnter += new System.Windows.Forms.DragEventHandler(this.lvSplice_DragEnter);
            this.lvSplice.DragDrop += new System.Windows.Forms.DragEventHandler(this.lvSplice_DragDrop);
            this.lvSplice.Click += new System.EventHandler(this.lvSplice_Click);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Type";
            this.columnHeader1.Width = 52;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Filename";
            this.columnHeader5.Width = 206;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Frames";
            this.columnHeader2.Width = 85;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Start";
            this.columnHeader3.Width = 81;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "End";
            this.columnHeader4.Width = 83;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(218, 182);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Start";
            // 
            // txtStart
            // 
            this.txtStart.Location = new System.Drawing.Point(253, 179);
            this.txtStart.Name = "txtStart";
            this.txtStart.Size = new System.Drawing.Size(100, 20);
            this.txtStart.TabIndex = 2;
            // 
            // txtEnd
            // 
            this.txtEnd.Location = new System.Drawing.Point(391, 179);
            this.txtEnd.Name = "txtEnd";
            this.txtEnd.Size = new System.Drawing.Size(100, 20);
            this.txtEnd.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(359, 182);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(26, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "End";
            // 
            // btnUpdate
            // 
            this.btnUpdate.Location = new System.Drawing.Point(497, 177);
            this.btnUpdate.Name = "btnUpdate";
            this.btnUpdate.Size = new System.Drawing.Size(65, 23);
            this.btnUpdate.TabIndex = 5;
            this.btnUpdate.Text = "Update";
            this.btnUpdate.UseVisualStyleBackColor = true;
            this.btnUpdate.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // btnSplice
            // 
            this.btnSplice.Location = new System.Drawing.Point(568, 12);
            this.btnSplice.Name = "btnSplice";
            this.btnSplice.Size = new System.Drawing.Size(62, 23);
            this.btnSplice.TabIndex = 8;
            this.btnSplice.Text = "Splice";
            this.btnSplice.UseVisualStyleBackColor = true;
            this.btnSplice.Click += new System.EventHandler(this.btnSplice_Click);
            // 
            // btnRemove
            // 
            this.btnRemove.Location = new System.Drawing.Point(119, 176);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(62, 23);
            this.btnRemove.TabIndex = 11;
            this.btnRemove.Text = "Remove";
            this.btnRemove.UseVisualStyleBackColor = true;
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // btnMoveUp
            // 
            this.btnMoveUp.Image = ((System.Drawing.Image)(resources.GetObject("btnMoveUp.Image")));
            this.btnMoveUp.Location = new System.Drawing.Point(12, 48);
            this.btnMoveUp.Name = "btnMoveUp";
            this.btnMoveUp.Size = new System.Drawing.Size(31, 30);
            this.btnMoveUp.TabIndex = 12;
            this.btnMoveUp.UseVisualStyleBackColor = true;
            this.btnMoveUp.Click += new System.EventHandler(this.btnMoveUp_Click);
            // 
            // btnMoveDown
            // 
            this.btnMoveDown.Image = ((System.Drawing.Image)(resources.GetObject("btnMoveDown.Image")));
            this.btnMoveDown.Location = new System.Drawing.Point(12, 104);
            this.btnMoveDown.Name = "btnMoveDown";
            this.btnMoveDown.Size = new System.Drawing.Size(31, 30);
            this.btnMoveDown.TabIndex = 13;
            this.btnMoveDown.UseVisualStyleBackColor = true;
            this.btnMoveDown.Click += new System.EventHandler(this.btnMoveDown_Click);
            // 
            // btnMoveStart
            // 
            this.btnMoveStart.Image = ((System.Drawing.Image)(resources.GetObject("btnMoveStart.Image")));
            this.btnMoveStart.Location = new System.Drawing.Point(12, 12);
            this.btnMoveStart.Name = "btnMoveStart";
            this.btnMoveStart.Size = new System.Drawing.Size(31, 30);
            this.btnMoveStart.TabIndex = 14;
            this.btnMoveStart.UseVisualStyleBackColor = true;
            this.btnMoveStart.Click += new System.EventHandler(this.btnMoveStart_Click);
            // 
            // btnMoveEnd
            // 
            this.btnMoveEnd.Image = ((System.Drawing.Image)(resources.GetObject("btnMoveEnd.Image")));
            this.btnMoveEnd.Location = new System.Drawing.Point(12, 140);
            this.btnMoveEnd.Name = "btnMoveEnd";
            this.btnMoveEnd.Size = new System.Drawing.Size(31, 30);
            this.btnMoveEnd.TabIndex = 15;
            this.btnMoveEnd.UseVisualStyleBackColor = true;
            this.btnMoveEnd.Click += new System.EventHandler(this.btnMoveEnd_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(568, 147);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(62, 23);
            this.btnClose.TabIndex = 16;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(51, 176);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(62, 23);
            this.btnAdd.TabIndex = 17;
            this.btnAdd.Text = "Add";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // frmSplice
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(639, 214);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnMoveEnd);
            this.Controls.Add(this.btnMoveStart);
            this.Controls.Add(this.btnMoveDown);
            this.Controls.Add(this.btnMoveUp);
            this.Controls.Add(this.btnRemove);
            this.Controls.Add(this.btnSplice);
            this.Controls.Add(this.btnUpdate);
            this.Controls.Add(this.txtEnd);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtStart);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lvSplice);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmSplice";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Splice";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView lvSplice;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtStart;
        private System.Windows.Forms.TextBox txtEnd;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnUpdate;
        private System.Windows.Forms.Button btnSplice;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.Button btnRemove;
        private System.Windows.Forms.Button btnMoveUp;
        private System.Windows.Forms.Button btnMoveDown;
        private System.Windows.Forms.Button btnMoveStart;
        private System.Windows.Forms.Button btnMoveEnd;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Button btnAdd;
    }
}