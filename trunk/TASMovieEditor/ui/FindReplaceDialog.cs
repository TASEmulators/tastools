using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MovieSplicer.UI
{
    public partial class FindReplaceDialog : Form
    {
        private frmMain m_parent;

        internal FindReplaceDialog( frmMain parent )
        {
            InitializeComponent();
            m_parent = parent;
        }

        private void FindReplaceDialog_Load(object sender, EventArgs e)
        {
        }

        private void btnReplaceCancel_Click(object sender, EventArgs e)
        {
            this.Visible = false;
        }

        public void SelectedTabIn(int value)
        {
            tagControl.SelectedIndex = value;
            if (tagControl.SelectedIndex == 0)
            {
                txtFind.Focus();
            }
            else if (tagControl.SelectedIndex == 1)
            {
                txtReplace.Focus();
            }
            else
            {
                txtFrameNumber.Focus();
            }
        }

        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tagControl.SelectedIndex == 0)
            {
                txtFind.Text = txtReplace.Text;
                radUp.Checked = radUpReplace.Checked;
                radDown.Checked = radDownReplace.Checked; 
                txtFind.Focus();
            }
            else if (tagControl.SelectedIndex == 1)
            {
                txtReplace.Text = txtFind.Text;
                radUpReplace.Checked = radUp.Checked;
                radDownReplace.Checked = radDown.Checked;
                txtReplace.Focus();
            }
            else
            {
                txtFrameNumber.Focus();
            }
        }

        private void FindReplaceDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Visible = false;
            if( e.CloseReason == CloseReason.UserClosing )
                e.Cancel = true;
        }

        private void txtFind_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                btnFind.PerformClick();
            else if (e.KeyChar == 27)
                btnFindCancel.PerformClick();
        }

        private void lblFind_Click(object sender, EventArgs e)
        {
            txtFind.Focus();
        }


        private void btnFind_Click(object sender, EventArgs e)
        {
            m_parent.findNext(txtFind.Text, radDown.Checked);
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Visible = false;
        }

        private void txtReplace_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                txtReplaceWith.Focus();
            else if (e.KeyChar == 27)
                btnFindCancel.PerformClick();
        }

        private void txtReplaceWith_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                btnReplaceFindNext.PerformClick();
            else if (e.KeyChar == 27)
                btnFindCancel.PerformClick();
        }

        private void btnReplaceFindNext_Click(object sender, EventArgs e)
        {
            m_parent.findNext(txtReplace.Text, radDownReplace.Checked);
        }

        private void btnReplace_Click(object sender, EventArgs e)
        {
            m_parent.replaceCurrentSelection(txtReplaceWith.Text);
        }

        private void btnReplaceAll_Click(object sender, EventArgs e)
        {
            m_parent.replaceAllItems(txtReplace.Text, txtReplaceWith.Text, radDownReplace.Checked);
        }

        private void btnGoto_Click(object sender, EventArgs e)
        {
            m_parent.gotoFrameNumber(txtFrameNumber.Text);
        }

        private void btnGotoCancel_Click(object sender, EventArgs e)
        {
            this.Visible = false;
        }

        private void txtFrameNumber_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                btnGotoOk.PerformClick();
            else if (e.KeyChar == 27)
                btnGotoCancel.PerformClick();
            e.Handled = !Char.IsDigit(e.KeyChar) && e.KeyChar != '\b';
            /*if (e.KeyChar < '0' || e.KeyChar > '9')
                e.Handled = true;*/
        }

        public string FindOut
        {
            get
            {
                if (tagControl.SelectedIndex == 0)
                {
                    return txtFind.Text;
                }
                else
                {
                    return txtReplace.Text;
                }
            }
        }
        public bool FindUpOrDownOut
        {
            get
            {
                if (tagControl.SelectedIndex == 0)
                {
                    return radDown.Checked;
                }
                else
                {
                    return radDownReplace.Checked;
                }
            }
        }
        public string ReplaceWithOut
        {
            get { return txtReplaceWith.Text; }
        }

        private void FindReplaceDialog_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 27)
                btnFindCancel.PerformClick();
        }

        public string GotoFrameNumber
        {
            get { return txtFrameNumber.Text; }
        }

        public string GotoFrameNumberLabel
        {
            set
            {
                lblFrameNumber.Text = "Frame (" + value + "):";
            }
        }
    }
}