/******************************************************************************** 
 * TAS Movie Editor                                                             *
 *                                                                              *
 * Copyright notice for this file:                                              *
 *  Copyright (C) 2006-7 Maximus                                                *
 *                                                                              *
 * This program is free software; you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * This program is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * along with this program; if not, write to the Free Software                  *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    *
 *******************************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using MovieSplicer.Data;
using MovieSplicer.Components;
using MovieSplicer.UI.Methods;

namespace MovieSplicer.UI
{
    public partial class frmSubtitles : TASForm
    {
        const string SRT_FILTER = "SRT - SubRip Subtitle|*.srt";

        TASMovieInputCollection Input;

        public frmSubtitles(ref TASMovieInputCollection input)
        {
            InitializeComponent();
            Input = input;          
        }

        /// <summary>
        /// Close the subtitles form
        /// </summary>        
        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// Browse for an output location
        /// </summary>        
        private void btnBrowse_Click(object sender, EventArgs e)
        {
            saveDlg = new SaveFileDialog();
            saveDlg.Filter = SRT_FILTER;
            saveDlg.ShowDialog();            

            txtFilename.Text = saveDlg.FileName;            
        }

        /// <summary>
        /// Export the current movie out as a subtitle file
        /// </summary>        
        private void btnExport_Click(object sender, EventArgs e)
        {
            // validate objects and values first
            if (Input.Input == null && txtFilename.Text.Length == 0) return;
            if (!IsNumeric(txtAVITiming.Text) || !IsNumeric(txtOffset.Text))
            {
                MessageBox.Show(frmMain.frm, "Timing/Offset values must be numeric", "Ooops",
                    MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
                return;
            }

            // create the subgenerator object
            SubtitleGenerator subGen = new SubtitleGenerator(ref Input, txtFilename.Text);
            subGen.Offset = Convert.ToInt32(txtOffset.Text);
            subGen.FPS    = Convert.ToDouble(txtAVITiming.Text);
            subGen.Export();
            subGen = null;

            MessageBox.Show(frmMain.frm, "Subtitle file exported", "Congrats",
                MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
        }           
    }
}