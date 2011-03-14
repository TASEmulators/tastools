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

namespace MovieSplicer.Data
{
    public class UndoBuffer
    {
        public TASMovieInput[][] Changes;
        
        /// <summary>
        /// Add a frame data collection to the undo buffer
        /// </summary>        
        public static void Add(ref UndoBuffer buffer, ref TASMovieInput[] change)
        {
            TASMovieInput[][] temp = new TASMovieInput[buffer.Changes.Length + 1][];

            if (buffer.Changes.Length > 0) buffer.Changes.CopyTo(temp, 0);

            temp[temp.Length - 1] = new TASMovieInput[change.Length];
            change.CopyTo(temp[temp.Length - 1], 0);       
            
            buffer.Changes = temp;
        }

        /// <summary>
        /// Return the last buffer value and remove it from the collections
        /// </summary>        
        public static void Undo(ref UndoBuffer buffer)
        {
            TASMovieInput[][] temp = new TASMovieInput[buffer.Changes.Length - 1][];
            for (int i = 0; i < temp.Length; i++)
                temp[i] = buffer.Changes[i];

            buffer.Changes = temp;
        }
    }
}
