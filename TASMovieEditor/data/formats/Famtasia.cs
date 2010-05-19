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

namespace MovieSplicer.Data.Formats
{
    public class Famtasia : TASMovie
    {
        /// <summary>
        /// Contains Format Specific items
        /// </summary>
        public struct FormatSpecific
        {
            public bool FDS;
        }   

        const byte HEADER_SIZE = 144;      
        public FormatSpecific FMVSpecific;

        private byte     BytesPerFrame = 0;
        private string[] InputValues = { ">", "<", "^", "v", "B", "A", "s", "S" };
        private int[]    Offsets = {
            0x00, // 4-byte signature: 46 4D 56 1A "FMV\x1A"
            0x04, // 1-byte flags:
                  //      bit 7: 0=reset-based, 1=savestate-based
                  //      other bits: unknown, set to 0
            0x05, // 1-byte flags:
                  //      bit 7: uses controller 1
                  //      bit 6: uses controller 2
                  //      bit 5: is a FDS recording
                  //      other bits: unknown, set to 0
            0x06, // 4-byte little-endian unsigned int: unknown, set to 00000000
            0x0A, // 4-byte little-endian unsigned int: rerecord count minus 1
            0x0E, // 2-byte little-endian unsigned int: unknown, set to 0000
            0x10, // 64-byte zero-terminated emulator identifier string
            0x50, // 64-byte zero-terminated movie title string
            0x90  // frame data begins here
        };

        public Famtasia(string FMVFile)
        {                        
            Filename = FMVFile;
            FillByteArrayFromFile(FMVFile, ref FileContents);
         
            Options = new TASOptions(true);
            Options.MovieStartFlag[0] = (1 & (FileContents[Offsets[1]]) >> 7) == 0 ? true : false;
            Options.MovieStartFlag[1] = (1 & (FileContents[Offsets[1]]) >> 7) == 1 ? true : false;

            Extra = new TASExtra();
            Extra.Description = ReadChars(ref FileContents, Offsets[7], 64);
           
            Input = new TASInput(2, false);
            for (int i = 0; i < 2; i++)
            {
                if ((1 & (FileContents[Offsets[2]]) >> 7 - i) == 1)
                {
                    Input.Controllers[i] = true;
                    BytesPerFrame++;
                }
            }

            FMVSpecific = new FormatSpecific();
            if ((1 & (FileContents[Offsets[2]]) >> 5) == 1)
            {
                FMVSpecific.FDS = true;
                BytesPerFrame++;
            }
            else
                FMVSpecific.FDS = false;
           
            Header = new TASHeader();
            Header.Signature     = ReadHEX(ref FileContents, Offsets[0], 4);
            Header.RerecordCount = Read32(ref FileContents, Offsets[4]);
            Header.EmulatorID    = ReadChars(ref FileContents, Offsets[6], 64);
            Header.FrameCount    = FileContents.Length - HEADER_SIZE / BytesPerFrame;

            getFrameInput(ref FileContents);
        }

        /// <summary>
        /// Convert FMV binary data to a readable string representation
        /// </summary>
        private void getFrameInput(ref byte[] byteArray)
        {
            Input.FrameData = new TASMovieInput[Header.FrameCount];
            
            for (int i = 0; i < Header.FrameCount; i++)
            {
                Input.FrameData[i] = new TASMovieInput();
                for (int j = 0; j < BytesPerFrame; j++)                
                    Input.FrameData[i].Controller[j] = parseControllerData(byteArray[HEADER_SIZE + i + j]);                                    
                i += BytesPerFrame - 1;
            }
        }

        /// <summary>
        /// FMV input convertion (bit position -> string)
        /// </summary> 
        private string parseControllerData(byte frameInput)
        {
            string input = "";

            for (int i = 0; i < 8; i++)
                if ((1 & (frameInput >> i)) == 1) input += InputValues[i];
            
            return input;
        }
       
        /// <summary>
        /// FMV input conversion (string -> bit position)
        /// </summary> 
        private byte parseControllerData(string frameValue)
        {
            byte input = 0x00;

            if (frameValue == null || frameValue == "") return input;
                
            for (int i = 0; i < 8; i++)
                if (frameValue.Contains(InputValues[i])) input |= (byte)(1 << i);

            return input;
        }

        /// <summary>
        /// Save input changes back out to file
        /// 
        /// TODO::Save might fail if this is an FDS movie (since FDS is factored into the BytesPerFrame)
        /// </summary>        
        public override void Save(string filename, ref TASMovieInput[] input)
        {
            byte[] head = ReadBytes(ref FileContents, 0, Offsets[8]);

            // create the output array and copy in the contents
            byte[] outputFile = new byte[head.Length + input.Length * BytesPerFrame];
            head.CopyTo(outputFile, 0);
            
            int position = 0;
            int controllers = Input.ControllerCount;
            for (int i = 0; i < input.Length; i++)            
                for (int j = 0; j < controllers; j++)                
                    outputFile[head.Length + position++] = parseControllerData(input[i].Controller[j]);

            // update the movie description
            for (int i = 0; i < 64; i++)
                if (i < Extra.Description.Length)
                    outputFile[Offsets[7] + i] = Convert.ToByte(Extra.Description[i]);
                else
                    outputFile[Offsets[7] + i] = 0;

            // NOTE::FMV files calculate frameCount based on filesize
            WriteByteArrayToFile(ref outputFile, filename, 0, 0);  
        }


        public override string[] GetUsableInputs()
        {
            System.Collections.ArrayList inputsArray = new System.Collections.ArrayList();
            for (int i = 0; i < InputValues.Length; i++)
            {
                if (InputValues[i] == "") continue;
                inputsArray.Add(InputValues[i]);
            }
            return (string[])inputsArray.ToArray(typeof(string));
        }

    }
}
