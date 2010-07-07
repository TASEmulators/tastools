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
using System.Text;
using System.IO;
using System.Windows.Forms;

using MovieSplicer.Data;
using MovieSplicer.Components;

namespace MovieSplicer.UI.Methods
{
    /// <summary>
    /// Handle saving/loading of buffered movie data
    /// </summary>
    class MovieBufferIO
    {
        /// <summary>
        /// Load the contents of an external file to the copy buffer
        /// Returns the column count
        /// </summary>        
        public static void Load(string filename, ref TASMovieInputCollection buffer)
        {
            StreamReader reader = File.OpenText(filename);
            string header = reader.ReadLine();            

            int controllers = 0;

            // TODO::This is a weak validation routine
            if (header.Length != 3)
            {
                MessageBox.Show(frmMain.frm, "Buffer file appears to be invalid", "Oops",
                    MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
                return;
            }

            string lineItem = null;            
            while ((lineItem = reader.ReadLine()) != null)
            {
                TASMovieInput frame = new TASMovieInput();
                string[]      split = lineItem.Split('|');
                for (int i = 0; i < split.Length; i++)                
                    frame.Controller[i] = split[i];
                                    
                TASMovieInput.Insert(ref buffer.Input, frame, buffer.Input.Length, 1);
                controllers = split.Length;
            }

            reader.Close(); reader.Dispose();

            buffer.Format      = (TASForm.MovieType)Enum.Parse(typeof(TASForm.MovieType), header);
            buffer.Controllers = controllers;
        }

        /// <summary>
        /// Save the contents of the copy buffer out to file
        /// </summary>        
        public static void Save(string filename, ref TASMovieInputCollection buffer)
        {
            TextWriter writer = File.CreateText(filename);

            writer.WriteLine(buffer.Format);

            for (int i = 0; i < buffer.Input.Length; i++)
            {
                string lineItem = "";
                for (int j = 0; j < buffer.Controllers; j++)
                {
                    lineItem += buffer.Input[i].Controller[j] + "|";
                }
                lineItem = lineItem.Remove(lineItem.Length - 1); // trim last char
                writer.WriteLine(lineItem);
            }            
            writer.Close(); writer.Dispose();
        }
    }
}
