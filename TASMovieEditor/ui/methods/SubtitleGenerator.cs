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

using MovieSplicer.Data;

namespace MovieSplicer.UI.Methods
{
    /// <summary>
    /// Output frame input to an SRT subtitle file
    /// </summary>
    public class SubtitleGenerator
    {      
        string[] inputOut = { "Start", "Select", "Up", "Down", "Left", "Right", "A", "B", "C", "X", "Y", "Z", "L", "R", "Mode" };
        string[] inputIn  = { "S", "s", "^", "v", "<", ">", "A", "B", "C", "X", "Y", "Z", "L", "R", "Mode" };

        TASMovieInputCollection Movie;
        string        Filename;
        public double FPS;
        public int    Offset;

        /// <summary>
        /// Instantiate the SubtitleGenerator with an inputCollection and a filename
        /// </summary>        
        public SubtitleGenerator(ref TASMovieInputCollection movie, string filename)
        {
            Movie    = movie;
            Filename = filename;
            
            // set default values
            Offset   = 0;
            FPS      = 60;
        }

        /// <summary>
        /// Export current InputCollection to a subtitle file
        /// </summary>
        public void Export()
        {
            int lastChange = 0;
            int position   = 0; // array accessor
            int block      = 0; // subtitle block            

            TASMovieInput temp;
            TextWriter    writer = File.CreateText(Filename);            
            
            while (position < Movie.Input.Length - 1)
            {
                temp = Movie.Input[position]; position++;
                if (areFramesEqual(temp, Movie.Input[position], Movie.Controllers) == false)                
                {                    
                    block++;
                    writer.WriteLine(block.ToString() + "\n" +
                        timeEncode(lastChange + Offset) + " --> " + timeEncode(position + Offset) + "\n" +
                        frameEncode(Movie.Input[position - 1], Movie.Controllers) + "\n\n");
                    
                    lastChange = position;
                }
                
            }
            // write the last entry                                       
            // NOTE::adding 1 to the position throws it out of bounds of the array, but we're
            // not using as an accessor, so I'm not sure if this is correct (prolly not =P)
            writer.WriteLine((block + 1).ToString() + "\n" +
                timeEncode(lastChange + Offset) + " --> " + timeEncode(position + 1 + Offset) + "\n" +
                frameEncode(Movie.Input[position], Movie.Controllers) + "\n\n");            

            writer.Close(); writer.Dispose();            
        }
        
        /// <summary>
        /// Convert a frame number to a timecode
        /// </summary>        
        private string timeEncode(int frame)
        {
            double seconds = frame / FPS;
            double ms = (seconds - (int)seconds) * 1000;
            if ((ms - (int)ms) >= 0.5) ms = ms + 1;

            int ss = (int)(seconds % 60);
            int mm = (int)(seconds / 60) % 60;
            int hh = mm / 60;

            return String.Format("{0}:{1}:{2},{3}", hh, mm, ss, (int)ms);
        }

        /// <summary>
        /// Convert an input's controller values to a string
        /// </summary>        
        private string frameEncode(TASMovieInput input, int controllers)
        {
            string temp = "";
            for (int i = 0; i < controllers; i++)
            {
                temp += "Controller #" + (i + 1) + ": ";
                for (int j = 0; j < inputIn.Length; j++)                
                    if(input.Controller[i] != null)
                        if (input.Controller[i].Contains(inputIn[j])) temp += inputOut[j] + " ";
                temp += "\n";
            }
            return temp;
        }

        /// <summary>
        /// Check if two TASMovieInput instances are equal
        /// </summary>        
        private bool areFramesEqual(TASMovieInput left, TASMovieInput right, int controllers)
        {
            for (int i = 0; i < controllers; i++)
                if (left.Controller[i] == null && right.Controller[i] == null) return true;                
                else if (left.Controller[i] == right.Controller[i]) return true;
            return false;
        }
    }
}
