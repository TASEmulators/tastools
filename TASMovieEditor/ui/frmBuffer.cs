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
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using MovieSplicer.Data;
using MovieSplicer.UI.Methods;
using MovieSplicer.Components;

namespace MovieSplicer.UI
{    
    public partial class frmBuffer : TASForm
    {
        TASMovieInputCollection bufferedInput;        
        
        public frmBuffer()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Create the frmBuffer form object and populate the listview based on the 
        /// arrayList of string[] content
        /// </summary>
        public frmBuffer(ref TASMovieInputCollection buffer)
        {
            InitializeComponent();

            if (buffer.Input != null)
            {
                bufferedInput = buffer;               
                bindControllerDataToListview();
            }
        }

        /// <summary>
        /// Destroy the form object, but pass the buffer back
        /// </summary>        
        public void Dispose(ref TASMovieInputCollection buffer)
        {
            buffer = bufferedInput;
            this.Dispose();
        }


        /// <summary>
        /// Initialize the controller data list from the buffer
        /// </summary>
        private void bindControllerDataToListview()
        {
            sbarMovieType.Text = Enum.GetName(typeof(MovieType), bufferedInput.Format);

            lvInputBuffer.ClearVirtualCache();
            lvInputBuffer.VirtualListSize   = bufferedInput.Input.Length;
            lvInputBuffer.VirtualListSource = bufferedInput.Input;
            lvInputBuffer.SetColumns(bufferedInput.Controllers);
        }        


        /// <summary>
        /// Perform the clear actions to reset the form 
        /// </summary>
        private void clearBuffer()
        {
            bufferedInput.Input  = null;
            bufferedInput.Format = MovieType.None;            

            lvInputBuffer.VirtualListSize = 0;
            lvInputBuffer.Clear(); lvInputBuffer.Columns.Add("Frame");

            sbarMovieType.Text = "None";            
        }


        /// <summary>
        /// Clear the Buffer (listview and related objects)
        /// </summary>        
        private void btnClear_Click(object sender, EventArgs e)
        {
            // if this object hasn't been created yet, return
            if (bufferedInput.Input == null) return;
            clearBuffer();
        }

        /// <summary>
        /// Save the contents of the copy buffer to an external file
        /// </summary>        
        private void btnSave_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "TAS Movie Editor Copy Buffer (*.tmb)|*.tmb";
            dlg.ShowDialog();
            if (dlg.FileName.Length > 0)            
                Methods.MovieBufferIO.Save(dlg.FileName, ref bufferedInput);
            
        }

        /// <summary>
        /// Load the contents of an external file to the copy buffer
        /// </summary>        
        private void btnLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "TAS Movie Editor Copy Buffer (*.tmb)|*.tmb";
            dlg.ShowDialog();
            if (dlg.FileName.Length > 0)
            {
                TASMovieInputCollection buffer = new TASMovieInputCollection(true);                                
                MovieBufferIO.Load(dlg.FileName, ref buffer);

                sbarMovieType.Text = Enum.GetName(typeof(MovieType), buffer.Format);

                if (buffer.Input.Length > 0)
                {
                    lvInputBuffer.Clear(); lvInputBuffer.Columns.Add("Frame");
                    bufferedInput = buffer;                                        
                    bindControllerDataToListview();
                }              
                else 
                    clearBuffer(); 
            }
        }

        /// <summary>
        /// Close the View Buffer form
        /// </summary>        
        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}