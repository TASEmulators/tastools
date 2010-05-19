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
        public TASMovieInput[] Changes;
        
        /// <summary>
        /// Add a frame data collection to the undo buffer
        /// </summary>        
        public static void Add(ref UndoBuffer[] buffer, ref TASMovieInput[] change)
        {
            UndoBuffer[] temp = new UndoBuffer[buffer.Length + 1];            
                  
            if (buffer.Length > 0) buffer.CopyTo(temp, 0);

            temp[temp.Length - 1] = new UndoBuffer();
            temp[temp.Length - 1].Changes = new TASMovieInput[change.Length];
            change.CopyTo(temp[temp.Length - 1].Changes, 0);       
            
            buffer = temp;
        }

        /// <summary>
        /// Return the last buffer value and remove it from the collections
        /// 
        /// TODO::after an undo, this method should remove the last entry in the collection. 
        /// Currently, this is done externally (after the call, inline)
        /// </summary>        
        public static void Undo(ref UndoBuffer[] buffer)
        {            
            UndoBuffer[] temp = new UndoBuffer[buffer.Length - 1];
            for (int i = 0; i < temp.Length; i++)
                temp[i] = buffer[i];

            buffer = temp;            
        }
    }
}
