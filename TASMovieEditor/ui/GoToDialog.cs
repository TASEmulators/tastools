using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MovieSplicer.UI
{
    public partial class GoToDialog : Form
    {
        public GoToDialog()
        {
            InitializeComponent();
        }

        private void GoToDialog_Load(object sender, EventArgs e)
        {
            txtFrameNumber.Focus();
        }

        private void lblFrameNumber_Click(object sender, EventArgs e)
        {
            txtFrameNumber.Focus();
        }

        private void txtFrameNumber_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                btnOk.PerformClick();
            e.Handled = !Char.IsDigit(e.KeyChar) && e.KeyChar != '\b';
            /*if (e.KeyChar < '0' || e.KeyChar > '9')
                e.Handled = true;*/
        }

        /*
        public bool EnableApplyButton
        {
            get { return btnOk.Enabled; }
            set { btnOk.Enabled = value; }
        }
        */
        public string TextOut
        {
            get { return txtFrameNumber.Text; }
        }
        public string LabelIn
        {
            set { lblFrameNumber.Text = "Frame: ("+value+")"; }
        }
    }
}