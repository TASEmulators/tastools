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

namespace MovieSplicer.UI
{
    public partial class frmSaveAs : TASForm
    {
        private TASMovie                Movie;
        private TASMovieInputCollection MovieData;
        private string                  Filename;

        /// <summary>
        /// Create a new SaveAs for with a reference to a movie and its (updated) input
        /// </summary>        
        public frmSaveAs(ref TASMovie movie, ref TASMovieInputCollection movieData, string newFilePrefix)
        {
            InitializeComponent();

            Movie     = movie;
            MovieData = movieData;

            txtFilename.Text    = DirectoryFromPath(Movie.Filename) + newFilePrefix + FilenameFromPath(Movie.Filename);
            txtAuthor.Text      = Movie.Extra.Author;
            txtDescription.Text = Movie.Extra.Description;

            txtAuthor.Focus();

            if (Movie.Extra.Author == null) txtAuthor.Enabled = false;
            if (movie.Extra.Description == null) txtDescription.Enabled = false;
        }

        /// <summary>
        /// Save the selected movie
        /// </summary>        
        private void btnSave_Click(object sender, EventArgs e)
        {
            Movie.Extra.Author      = txtAuthor.Text;
            Movie.Extra.Description = txtDescription.Text;

            Filename = txtFilename.Text;

            Movie.Save(Filename, ref MovieData.Input);

            this.Close();
        }

        /// <summary>
        /// Browse to a file to save
        /// </summary>        
        private void btnBrowse_Click(object sender, EventArgs e)
        {
            saveDlg = new SaveFileDialog();

            switch (MovieData.Format)
            {
                case MovieType.FCM: saveDlg.Filter = FCM_FILTER; break;
                case MovieType.FMV: saveDlg.Filter = FMV_FILTER; break;
                case MovieType.GMV: saveDlg.Filter = GMV_FILTER; break;
                case MovieType.M64: saveDlg.Filter = M64_FILTER; break;
                case MovieType.MMV: saveDlg.Filter = MMV_FILTER; break;
                case MovieType.PXM: saveDlg.Filter = PXM_FILTER; break;
                case MovieType.SMV: saveDlg.Filter = SMV_FILTER; break;
                case MovieType.VBM: saveDlg.Filter = VBM_FILTER; break;
            }
           
            saveDlg.FileName = txtFilename.Text;
            saveDlg.ShowDialog();                        

            if (saveDlg.FileName.Length > 0) Filename = saveDlg.FileName;
            txtFilename.Text = Filename;
        }        
    }
}