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
using MovieSplicer.Data.Formats;
using MovieSplicer.Components;

namespace MovieSplicer.UI
{
    public partial class frmCompare : TASForm
    {       
        TASMovieInputCollection Source;
        TASMovieInputCollection Target;

        public frmCompare()
        {
            InitializeComponent();            
        }        
       
        /// <summary>
        /// Launch an OpenFileDialog an populate the TASMovieInputCollection object
        /// according to the type of movie file that's selected
        /// </summary>        
        private string loadMovie(ref TASMovieInputCollection location)
        {
            openDlg = new OpenFileDialog();
            openDlg.Filter = TAS_FILTER;
            openDlg.ShowDialog();
            string filename = openDlg.FileName;
            openDlg.Dispose();            
            
            if (filename.Length == 0) return null;               
            
            TASMovie movie = new TASMovie();
            location = new TASMovieInputCollection();            
            location.Format = IsValid(filename);                        
                        
            // load the movie object up with the correct format 
            switch (location.Format)
            {                
                case MovieType.SMV: movie = new SNES9x(filename); break;
                case MovieType.FCM: movie = new FCEU(filename); break;
                case MovieType.GMV: movie = new Gens(filename); break;
                case MovieType.FMV: movie = new Famtasia(filename); break;
                case MovieType.VBM: movie = new VisualBoyAdvance(filename); break;
                case MovieType.M64: movie = new Mupen64(filename); break;
                case MovieType.MMV: movie = new Dega(filename); break;
                case MovieType.PXM: movie = new PCSX(filename); break;  // shares with PJM
                case MovieType.PJM: movie = new PCSX(filename); break;  // shares with PXM
            }
            location.Input       = movie.Input.FrameData;
            location.Controllers = movie.Input.ControllerCount;

            return filename;
        }

        /// <summary>
        /// Load the Source movie (Movie 1)
        /// </summary>        
        private void btnLoadSource_Click(object sender, EventArgs e)
        {
            txtSource.Text = FilenameFromPath(loadMovie(ref Source));
            if (txtSource.Text.Length > 0)
            {
                btnLoadSource.Enabled = false;
                txtSourceFrames.Text = String.Format("{0:0,0}", Source.Input.Length);
            }
        }

        /// <summary>
        /// Load the Target movie (Movie 2)
        /// </summary>        
        private void btnLoadTarget_Click(object sender, EventArgs e)
        {
            txtTarget.Text = FilenameFromPath(loadMovie(ref Target));
            if (txtTarget.Text.Length > 0) 
            {
                btnLoadTarget.Enabled = false;
                txtTargetFrames.Text = String.Format("{0:0,0}", Target.Input.Length);
            }
        }

        /// <summary>
        /// Process the two movies, applying a range (if specified)
        /// </summary>        
        private void btnProcess_Click(object sender, EventArgs e)
        {
            TASMovieInputCollection source = Source;
            TASMovieInputCollection target = Target;
            
            // default offsets to 1 so the frame representation starts from 1 instead of zero
            lvOutput.SourceOffset = 1;
            lvOutput.TargetOffset = 1;
            
            if(IsNumeric(txtSourceStart.Text) && IsNumeric(txtSourceEnd.Text) && (txtSourceStart.Text.Length > 0) && (txtSourceEnd.Text.Length > 0))
            {
                int start = CInt(txtSourceStart.Text);
                int end   = CInt(txtSourceEnd.Text);
                
                source.Input = new TASMovieInput[end - start];

                for (int i = 0; i < end - start; i++)                                   
                    source.Input[i] = Source.Input[i + start - 1];

                lvOutput.SourceOffset = start;
            }
            if (IsNumeric(txtTargetStart.Text) && IsNumeric(txtTargetEnd.Text) && (txtTargetStart.Text.Length > 0) && (txtTargetEnd.Text.Length > 0))
            {
                int start = CInt(txtTargetStart.Text);
                int end   = CInt(txtTargetEnd.Text);

                target.Input = new TASMovieInput[end - start];

                for (int j = 0; j < end - start; j++)
                    target.Input[j] = Target.Input[j + start - 1];

                lvOutput.TargetOffset = start;
            }
            lvOutput.ClearVirtualCache();
            lvOutput.VirtualListSize = 0;
            lvOutput.Source = source;
            lvOutput.Target = target;
            lvOutput.SetColumns();
            lvOutput.VirtualListSize = (source.Input.Length > target.Input.Length) ? source.Input.Length : target.Input.Length;                       
        }

        /// <summary>
        /// Clear selected movies and re-enable the load controls
        /// </summary>        
        private void btnClear_Click(object sender, EventArgs e)
        {
            txtSource.Text = ""; txtSourceFrames.Text = ""; txtSourceStart.Text = ""; txtSourceEnd.Text = "";
            txtTarget.Text = ""; txtTargetFrames.Text = ""; txtTargetStart.Text = ""; txtTargetEnd.Text = "";
            btnLoadSource.Enabled = true;
            btnLoadTarget.Enabled = true;            
            lvOutput.ClearVirtualCache();
            lvOutput.Clear();
        }

       
    }
}