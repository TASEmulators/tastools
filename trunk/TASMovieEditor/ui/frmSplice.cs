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

using MovieSplicer.Components;
using MovieSplicer.Data;
using MovieSplicer.Data.Formats;

namespace MovieSplicer.UI
{
    public partial class frmSplice : TASForm
    {        
        public struct DroppedMovie
        {
            public TASMovie  Movie;
            public MovieType MovieType;
            public int       Start;
            public int       End;
        }

        DroppedMovie[] Movies;

        /// <summary>
        /// Initialize
        /// </summary>
        public frmSplice()
        {
            InitializeComponent();
            
            // initialize the array (save the grief of hitting a null reference error)
            Movies = new DroppedMovie[0];
        }
        
        /// <summary>
        /// Populate the start/end values in the edit fields from the selected row
        /// </summary>        
        private void lvSplice_Click(object sender, EventArgs e)
        {
            // make sure an item is selected
            if (lvSplice.SelectedIndices.Count == 0) return;

            int position  = lvSplice.SelectedIndices[0];
            txtStart.Text = lvSplice.Items[position].SubItems[3].Text;
            txtEnd.Text   = lvSplice.Items[position].SubItems[4].Text;
        }

        /// <summary>
        /// Check for a valid TAS file and add it to the DroppedMovie array
        /// </summary>        
        private DroppedMovie[] populateMovieList(string filename)
        {
            MovieType fileType = IsValid(filename);
            if (fileType != MovieType.None)
            {
                DroppedMovie[] temp = new DroppedMovie[Movies.Length + 1];                
                Movies.CopyTo(temp, 0);

                temp[Movies.Length].MovieType = fileType;
                temp[Movies.Length].Movie = LoadMovie(filename, fileType);                
                return temp;
            }
            return null;
        }

       
        // Change the drag cursor type
        private void lvSplice_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Move;
        }

        /// <summary>
        /// Validate dropped files and add them to the list
        /// </summary>        
        private void lvSplice_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

            foreach (string file in files)
                Movies = populateMovieList(file);

            addMoviesToList();               
        }

        /// <summary>
        /// Refresh the dropped movies list
        /// </summary>
        private void addMoviesToList()
        {
            // clear the control
            // DEBUG::Not efficient, find a way to just update new movies into the list
            // intead of redrawing on each drop
            lvSplice.Items.Clear();

            foreach (DroppedMovie movie in Movies)
            {
                ListViewItem lvi = new ListViewItem(movie.MovieType.ToString());
                lvi.SubItems.Add(FilenameFromPath(movie.Movie.Filename));
                lvi.SubItems.Add(String.Format("{0:0,0}", movie.Movie.Header.FrameCount));
                lvi.SubItems.Add(movie.Start.ToString());
                lvi.SubItems.Add(movie.End.ToString()); 
                lvSplice.Items.Add(lvi);
            }         
        }

        /// <summary>
        /// Add a movie to the splicer from an OpenDialog call
        /// </summary>        
        private void btnAdd_Click(object sender, EventArgs e)
        {
            openDlg = new OpenFileDialog();
            openDlg.Filter = TAS_FILTER;
            openDlg.ShowDialog();

            Movies = populateMovieList(openDlg.FileName);            
            openDlg.Dispose();

            addMoviesToList();
        }

        /// <summary>
        /// Remove an entry from the list
        /// </summary>        
        private void btnRemove_Click(object sender, EventArgs e)
        {
            // make sure an item is selected
            if (lvSplice.SelectedIndices.Count == 0) return;
            
            // remove the item entry from the list
            int position = lvSplice.SelectedIndices[0];
            lvSplice.Items.RemoveAt(position);

            DroppedMovie[] temp = new DroppedMovie[Movies.Length - 1];

            // remove the item from the array
            for (int i = 0; i < position; i++)
                temp[i] = Movies[i];
            for (int j = position; j < temp.Length; j++)
                temp[j] = Movies[j + 1];

            Movies = temp;                
        }

        /// <summary>
        /// Add the frame range to the selected row
        /// </summary>        
        private void btnUpdate_Click(object sender, EventArgs e)
        {
            // make sure an item is selected
            if (lvSplice.SelectedIndices.Count == 0) return;
            
            // range has to be numeric to proceed
            if(!IsNumeric(txtStart.Text) || !IsNumeric(txtEnd.Text)) return;
                        
            int position = lvSplice.SelectedIndices[0];
            int frames = Movies[position].Movie.Header.FrameCount;
            int start = Convert.ToInt32(txtStart.Text);
            int end = Convert.ToInt32(txtEnd.Text);

            // check for valid range
            if (start < 0 || start > frames || end < start || end > frames)
            {
                MessageBox.Show(frmMain.frm, "Invalid range for selected movie", "Ooops",
                    MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
                return;
            }

            // HACK::Just populate the necessary row. This avoids the need for a full redraw
            lvSplice.Items[position].SubItems[3].Text = start.ToString();
            lvSplice.Items[position].SubItems[4].Text = end.ToString();

            // update the movie with the new range
            Movies[position].Start = start;
            Movies[position].End   = end;
        }
        
        /// <summary>
        /// Perform the splice
        /// </summary>        
        private void btnSplice_Click(object sender, EventArgs e)
        {
            // exit if not enough movies are opened
            if (Movies.Length < 2)
            {
                MessageBox.Show(frmMain.frm, "At least two (2) movies must be loaded for splicing to be performed", "Movie Count Too Low",
                    MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
                return;
            }

            // ensure all loaded movies are of the same type
            for (int i = 0; i < Movies.Length - 1; i++)
            {
                if (Movies[i].MovieType != Movies[i + 1].MovieType)
                {
                    MessageBox.Show(frmMain.frm, "Movies aren't all of the same type", "Type Mismatch",
                        MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
                    return;
                }
            }

            TASMovieInput[] spliced = new TASMovieInput[0];
            
            for (int i = 0; i < Movies.Length; i++)
            {
                // handle zeroes                
                if (Movies[i].End == 0) Movies[i].End = Movies[i].Movie.Header.FrameCount;
                
                // NOTE::increase VBM by 1 frame since we enumerate from zero
                if (Movies[i].MovieType == MovieType.VBM) Movies[i].End++;
                
                spliced = TASMovieInput.Splice(ref spliced, ref Movies[i].Movie.Input.FrameData, 0, spliced.Length, Movies[i].Start, Movies[i].End);
            }                                    

            TASMovieInputCollection temp = new TASMovieInputCollection();
            temp.Format = Movies[0].MovieType;
            temp.Input  = spliced;
            frmSaveAs frm = new frmSaveAs(ref Movies[0].Movie, ref temp, "spliced-");
            frm.ShowDialog(); frm.Dispose(); 
        }        

        
        /// <summary>
        /// After reordering items, refresh and select the moved item
        /// </summary>        
        private void updateList(int selectedPosition)
        {
            addMoviesToList();

            lvSplice.Items[selectedPosition].Selected = true;
            lvSplice.Focus();
            lvSplice.EnsureVisible(selectedPosition);
        }

        /// <summary>
        /// Move a movie to the first position in the list
        /// </summary>        
        private void btnMoveStart_Click(object sender, EventArgs e)
        {
            if (lvSplice.SelectedIndices.Count > 0 && lvSplice.SelectedIndices[0] > 0)
            {
                int position = lvSplice.SelectedIndices[0];
                DroppedMovie temp = Movies[position];
                for (int i = position; i > 0; i--)
                    Movies[i] = Movies[i - 1];
                Movies[0] = temp;
                updateList(0);                
            }
        }

        /// <summary>
        /// Move a movie up one position in the list
        /// </summary>        
        private void btnMoveUp_Click(object sender, EventArgs e)
        {
            if (lvSplice.SelectedIndices.Count > 0 && lvSplice.SelectedIndices[0] > 0)
            {
                int position = lvSplice.SelectedIndices[0];
                DroppedMovie temp = Movies[position];
                Movies[position] = Movies[position - 1];
                Movies[position - 1] = temp;

                updateList(position - 1);            
            }
        }

        /// <summary>
        /// Move a move down one position in the list
        /// </summary>        
        private void btnMoveDown_Click(object sender, EventArgs e)
        {
            if (lvSplice.SelectedIndices.Count > 0 && lvSplice.SelectedIndices[0] < lvSplice.Items.Count - 1)
            {
                int position = lvSplice.SelectedIndices[0];
                DroppedMovie temp = Movies[position];
                Movies[position] = Movies[position + 1];
                Movies[position + 1] = temp;

                updateList(position + 1);
            }
        }

        /// <summary>
        /// Move a movie to the end of the list
        /// </summary>        
        private void btnMoveEnd_Click(object sender, EventArgs e)
        {
            if (lvSplice.SelectedIndices.Count > 0 && lvSplice.SelectedIndices[0] < lvSplice.Items.Count - 1)
            {
                int position = lvSplice.SelectedIndices[0];
                DroppedMovie temp = Movies[position];
                for (int i = 0; i < lvSplice.Items.Count - 1; i++)
                    Movies[i] = Movies[i + 1];
                Movies[lvSplice.Items.Count - 1] = temp;
                updateList(lvSplice.Items.Count - 1);
            }
        }

        /// <summary>
        /// Close the form
        /// </summary>        
        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

    }
}