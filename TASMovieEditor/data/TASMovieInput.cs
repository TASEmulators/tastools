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
using System.Text;

using MovieSplicer.Components;

namespace MovieSplicer.Data
{
    /// <summary>
    /// Contains the input collection from a movie file, as well as what kind of movie and
    /// how many controllers are in use
    /// </summary>
    public struct TASMovieInputCollection
    {
        public int               Controllers;
        public TASForm.MovieType Format;                
        public TASMovieInput[]   Input;        

        /// <summary>
        /// Initialize the Input array with zero elements and default data
        /// (just pass 'true' ... this is more a hack than anything else)
        /// </summary>        
        public TASMovieInputCollection(bool initialize)
        {            
            Input       = new TASMovieInput[0];
            Format      = TASForm.MovieType.None;
            Controllers = 0;        
        }
    }

    /// <summary>
    /// Contains an array to hold controller data in string form as well as 
    /// all necessary methods for data forming
    /// </summary>
    public class TASMovieInput
    {
        public string[] Controller = new string[5];

    #region Methods

        /// <summary>
        /// Insert a given number of assigned frames (TASMovieInput()) at the
        /// desired position
        /// </summary>        
        public static void Insert(ref TASMovieInput[] input, TASMovieInput frame, int position, int length)
        {
            TASMovieInput[] temp = new TASMovieInput[input.Length + length];
            
            for (int i = 0; i < position; i++)
                temp[i] = input[i];
            for (int j = 0; j < length; j++)
                temp[position + j] = frame;
            for (int k = position; k < input.Length; k++)
                temp[k + length] = input[k];

            input = temp;
        }

        /// <summary>
        /// Insert a given number of frames passed through a buffer at the
        /// desired position
        /// </summary>        
        public static void Insert(ref TASMovieInput[] input, ref TASMovieInput[] buffer, int position)
        {
            TASMovieInput[] temp = new TASMovieInput[input.Length + buffer.Length];

            for (int i = 0; i < position; i++)
                temp[i] = input[i];
            for (int j = 0; j < buffer.Length; j++)
                temp[position + j] = buffer[j];
            for (int k = position; k < input.Length; k++)
                temp[k + buffer.Length] = input[k];

            input = temp;
        }

        /// <summary>
        /// Insert repeatedly a given number of frames passed through a buffer at each
        /// desired position
        /// </summary>        
        public static void InsertMultiple(ref TASMovieInput[] input, ref TASMovieInput[] buffer, int[] indices)
        {
            int blockCount = indices.Length;
            int blockSize  = buffer.Length;
            TASMovieInput[] temp = new TASMovieInput[input.Length + blockSize * blockCount];

            bool pastEnd = (indices[blockCount - 1] < input.Length ? false : true);

            int i = 0;
            for (int j = 0; j < blockCount; i++)
            {
                if (i == indices[j])
                {
                    for (int k = 0; k < blockSize; k++)
                    {
                        temp[i + blockSize * j + k] = buffer[k];
                    }
                    j++;

                    // takes care of paste-after
                    if (pastEnd && j == blockCount)
                    {
                        input = temp;
                        return;
                    }
                }
                temp[i + blockSize * j] = input[i];
            }

            for (int k = indices[blockCount - 1] + 1; k < input.Length; k++)
                temp[blockSize * blockCount + k] = input[k];

            input = temp;
        }

        /// <summary>
        /// Insert the passed in TASMovieInput at the desired position
        /// updateFlag is a collection of bool values to indicate which controller(s) to update
        /// 
        /// NOTE::The logic is a little weird, but if I tried looping through the number of updates,
        /// checking against the flag, unexpected results occured (ie. updating 1 frame updated every
        /// similar frame)
        /// 
        /// NOTE::Autofire update just adds the selected input to alternating frames
        /// </summary>        
        public static void Update(ref TASMovieInput[] input, TASMovieInput frame, bool[] updateFlag, bool autofireUpdate, int[] indices)
        {
            TASMovieInput[] temp = new TASMovieInput[indices.Length];
            for (int i = 0; i < indices.Length; i++)
            {                                
                temp[i] = new TASMovieInput();
                for (int j = 0; j < updateFlag.Length; j++)
                {
                    // if autofire is checked, insert a blank frame on alternating frames
                    // NOTE::mod check set to 1 so if option is checked and only 1 frame needs to be updated
                    // the changes won't be skipped :)
                    if (autofireUpdate)
                    {
                        if (i % 2 == 1)
                            temp[i].Controller[j] = (updateFlag[j]) ? null : input[indices[i]].Controller[j];
                        else
                            temp[i].Controller[j] = (updateFlag[j]) ? frame.Controller[j] : input[indices[i]].Controller[j];
                    }
                    else
                        temp[i].Controller[j] = (updateFlag[j]) ? frame.Controller[j] : input[indices[i]].Controller[j];
                }
            }

            for (int i = 0; i < indices.Length; i++)
                input[indices[i]] = temp[i];

            // OLD .. DOESN'T WORK 100%
            //for (int i = position; i < position + length; i++)
            //    for (int j = 0; j < updateFlag.Length; j++)
            //        if (updateFlag[j]) input[i] = frame;
        }

        /// <summary>
        /// Update the desired frames with the TASMovieInput
        /// updateFlag is a collection of bool values to indicate which controller(s) to update
        /// 
        /// NOTE::Autofire update just adds the selected input to alternating frames
        /// </summary>        
        public static void UpdatePlus(ref TASMovieInput[] input, TASMovieInput frame, bool[] updateFlag, bool autofireUpdate, int[] indices)
        {
            TASMovieInput[] temp = new TASMovieInput[indices.Length];
            for (int i = 0; i < indices.Length; i++)
            {
                temp[i] = new TASMovieInput();
                for (int j = 0; j < updateFlag.Length; j++)
                {
                    // if autofire is checked, insert a blank frame on alternating frames
                    // NOTE::mod check set to 1 so if option is checked and only 1 frame needs to be updated
                    // the changes won't be skipped :)
                    if (autofireUpdate)
                    {
                        if (i % 2 == 1)
                            temp[i].Controller[j] = input[indices[i]].Controller[j];
                        else
                            temp[i].Controller[j] = (updateFlag[j]) ? input[indices[i]].Controller[j] + frame.Controller[j] : input[indices[i]].Controller[j];
                    }
                    else
                        temp[i].Controller[j] = (updateFlag[j]) ? input[indices[i]].Controller[j] + frame.Controller[j] : input[indices[i]].Controller[j];
                }
            }

            for (int i = 0; i < indices.Length; i++)
            {
                input[indices[i]] = temp[i];
            }
        }

        /// <summary>
        /// Remove a given number of frames at the desired position
        /// </summary>        
        public static void Remove(ref TASMovieInput[] input, int[] indices)
        {
            TASMovieInput[] temp = new TASMovieInput[input.Length - indices.Length];

            int i = 0;
            for (int j = 0; j < indices.Length; )
            {
                if (i + j != indices[j])
                {
                    temp[i] = input[i + j];
                    i++;
                }
                else
                    j++;
            }

            for (int k = indices[indices.Length - 1] + 1; k < input.Length; i++, k++)
                temp[i] = input[k];

            input = temp;
        }

        /// <summary>
        /// Copy a given number of frames at the desired position by extracting them to a new
        /// array
        /// </summary>        
        public static TASMovieInput[] Copy(ref TASMovieInput[] input, int[] indices)
        {
            TASMovieInput[] temp = new TASMovieInput[indices.Length];

            for (int i = 0; i < indices.Length; i++)
                temp[i] = input[indices[i]];

            return temp;
        }

        /// <summary>
        /// Splice two TASMovieInput collections together
        /// </summary>        
        public static TASMovieInput[] Splice(ref TASMovieInput[] source, ref TASMovieInput[] target, int sourceStart, int sourceEnd, int targetStart, int targetEnd)
        {
            // NOTE::zero means start from the beginning, but if not, since it's an index, subtract 1                        
            if (targetStart != 0) targetStart--;

            TASMovieInput[] spliced = new TASMovieInput[(sourceEnd - sourceStart) + (targetEnd - targetStart)];
            for (int i = sourceStart; i < sourceEnd; i++)
                spliced[i] = source[i];
            
            int position = 0;
            for (int j = targetStart; j < targetEnd; j++)
                spliced[sourceEnd + position++] = target[j];

            return spliced;
        }

        /// <summary>
        /// Cycle through the input collection returning an index of the first available match
        /// from the given offset
        /// </summary>                
        public static int Search(ref TASMovieInput[] input, string pattern, int startPosition, int endPosition)
        {
            if (startPosition < endPosition)
            {
                for (int i = startPosition; i < endPosition; i++)
                    for (int j = 0; j < input[i].Controller.Length; j++)
                        if (input[i].Controller[j] != null)
                            if (input[i].Controller[j] == pattern) return i;
            }
            else
            {
                for (int i = startPosition; i > endPosition; i--)
                    for (int j = 0; j < input[i].Controller.Length; j++)
                        if (input[i].Controller[j] != null)
                            if (input[i].Controller[j] == pattern) return i;
            }
            return -1;
        }
    
        /// <summary>
        /// append controller data to an existing TASMovieInput object
        /// </summary>        
        public static TASMovieInput operator +(TASMovieInput left, TASMovieInput right)
        {
            TASMovieInput result = new TASMovieInput();
            for (int i = 0; i < left.Controller.Length; i++)
                result.Controller[i] = left.Controller[i] + right.Controller[i];
            return result;
        }

    #endregion

    }
}
