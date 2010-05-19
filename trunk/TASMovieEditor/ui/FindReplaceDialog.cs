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

        public void SelectedTabIn(int value )
        {
            tabControl.SelectedIndex = value;
            if (tabControl.SelectedIndex == 0)
            {
                txtFind.Focus();
            }
            else
            {
                txtReplace.Focus();
            }
        }

        private void btnReplaceCancel_Click(object sender, EventArgs e)
        {
            this.Visible = false;
        }

        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tabControl.SelectedIndex == 0)
            {
                txtFind.Text = txtReplace.Text;
                radUp.Checked = radUpReplace.Checked;
                radDown.Checked = radDownReplace.Checked; 
                txtFind.Focus();
            }
            else
            {
                txtReplace.Text = txtFind.Text;
                radUpReplace.Checked = radUp.Checked;
                radDownReplace.Checked = radDown.Checked;
                txtReplace.Focus();
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
                btnCancel.PerformClick();
        }

        private void lblFind_Click(object sender, EventArgs e)
        {
            txtFind.Focus();
        }


        private void btnFind_Click(object sender, EventArgs e)
        {
            m_parent.findNext(txtFind.Text, radDown.Checked);
            Focus();
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
                btnCancel.PerformClick();
        }

        private void txtReplaceWith_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                btnReplaceFindNext.PerformClick();
            else if (e.KeyChar == 27)
                btnCancel.PerformClick();
        }

        private void btnReplaceFindNext_Click(object sender, EventArgs e)
        {
            m_parent.findNext(txtReplace.Text, radDownReplace.Checked);
            Focus();
        }

        private void btnReplace_Click(object sender, EventArgs e)
        {
            m_parent.replaceCurrentSelection(txtReplaceWith.Text);
        }

        private void btnReplaceAll_Click(object sender, EventArgs e)
        {
            m_parent.replaceAllItems(txtReplace.Text, txtReplaceWith.Text, radDownReplace.Checked);
            Focus();
        }

        public string FindOut
        {
            get
            {
                if (tabControl.SelectedIndex == 0)
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
                if (tabControl.SelectedIndex == 0)
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
                btnCancel.PerformClick();
        }

 
    }
}