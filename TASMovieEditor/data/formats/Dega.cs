/******************************************************************************** 
 * TAS Movie Editor                                                             *
 *                                                                              *
 * Copyright notice for this file:                                              *
 *  Copyright (C) 2006-7 Maximus, 2009-10 aquanull                              *
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

namespace MovieSplicer.Data.Formats
{
    public class Dega : TASMovie
    {
        /// <summary>
        /// Contains Format Specific items
        /// </summary>
        public struct FormatSpecific
        {
            public int PackSize;
            public bool Japan;
            public bool GameGear; 
        }   

        const byte HEADER_SIZE = 64;
        public FormatSpecific MMVSpecific;
        
        private string[] InputValues = { "^", "v", "<", ">", "1", "2", "S", "s" };
        private int[]    Offsets = {
            0x00, // 4-byte signature: "MMV\0"
            0x04, // 4-byte little endian unsigned int: dega version
            0x08, // 4-byte little endian unsigned int: frame count
            0x0C, // 4-byte little endian unsigned int: rerecord count
            0x10, // 4-byte little endian flag: begin from reset?
            0x14, // 4-byte little endian unsigned int: offset of state information
            0x18, // 4-byte little endian unsigned int: offset of input data
            0x1C, // 4-byte little endian unsigned int: size of input packet
            0x20, // string: author info (UTF-8)
            0x60, // 4-byte little endian flags
                  //    bit 0: unused
                  //    bit 1: PAL
                  //    bit 2: Japan
                  //    bit 3: Game Gear (version 1.16+)
                  //    bits 4-31: unused
            0x64, // string: rom name (ASCII)
            0xE4, // 16-byte binary: rom MD5 digest
            0xF4  // binary: free space
        };

        public Dega(string MMVFile)
        {
            Filename = MMVFile;
            FillByteArrayFromFile(MMVFile, ref FileContents);

            ControllerDataOffset = Read32(ref FileContents, Offsets[6]);
            SaveStateOffset = Read32(ref FileContents, Offsets[5]);

            Header = new TASHeader();
            Header.Signature     = ReadHEX(ref FileContents, Offsets[0], 4);
            Header.Version       = Read32(ref FileContents, Offsets[1]);
            Header.FrameCount    = Read32(ref FileContents, Offsets[2]);
            Header.RerecordCount = Read32(ref FileContents, Offsets[3]);         

            Options = new TASOptions(true);
            Options.MovieStartFlag[0]  = FileContents[Offsets[4]] == 0 ? true : false;
            Options.MovieStartFlag[1]  = FileContents[Offsets[4]] != 0 ? true : false;
            Options.MovieTimingFlag[0] = (1 & (FileContents[Offsets[9]] >> 1)) == 0 ? true : false;
            Options.MovieTimingFlag[1] = (1 & (FileContents[Offsets[9]] >> 1)) == 1 ? true : false;
                     
            Extra = new TASExtra();
            Extra.Author = ReadChars(ref FileContents, Offsets[8], Offsets[9] - Offsets[8]);
            Extra.ROM = ReadChars(ref FileContents, Offsets[10], Offsets[11] - Offsets[10]);
            Extra.CRC = ReadHEX(ref FileContents, Offsets[11], 16);

            MMVSpecific = new FormatSpecific();
            MMVSpecific.PackSize = Read32(ref FileContents, Offsets[7]);
            MMVSpecific.Japan = (1 & (FileContents[Offsets[9]] >> 2)) == 1 ? true : false;
            MMVSpecific.GameGear = (1 & (FileContents[Offsets[9]] >> 3)) == 1 ? true : false;

            Input = new TASInput(2, true);
            if (MMVSpecific.GameGear)
                Input.Controllers[1] = false;

            getFrameInput(ref FileContents);
        }

        /// <summary>
        /// Populate the TASMovieInput array's FrameData object with the string representation
        /// of the movie file's input
        /// </summary>
        /// <param name="byteArray"></param>
        private void getFrameInput(ref byte[] byteArray)
        {
            Input.FrameData = new TASMovieInput[Header.FrameCount];

            // there are currently 2 bytes per frame of input
            for (int i = 0; i < Header.FrameCount; i++)
            {
                Input.FrameData[i] = new TASMovieInput();

                // there are always 2 controllers to deal with, so no need to 
                // run the array through a loop to be populated
                Input.FrameData[i].Controller[0] = parseControllerData(byteArray[ControllerDataOffset + i * 2]);
                Input.FrameData[i].Controller[1] = parseControllerData(byteArray[ControllerDataOffset + i * 2 + 1]);
            }
        }

        /// <summary>
        /// Convert byte values to string input
        /// </summary>
        private string parseControllerData(byte frameValue)
        {
            string input = "";

            for (int i = 0; i < 8; i++)
                if (((1 & frameValue >> i)) == 1) input += InputValues[i];
           
            return input;
        }

        /// <summary>
        /// Convert string input to a byte value
        /// </summary> 
        private byte parseControllerData(string frameInput)
        {
            byte input = 0;
            if (frameInput != null && frameInput != "")
            {                
                for (int i = 0; i < 8; i++)
                    if (frameInput.Contains(InputValues[i])) input |= (byte)(1 << i);                
            }
            return input;
        }


        /// <summary>
        /// Save an MMV file back out to disk
        /// </summary>
        public override void Save(string filename, ref TASMovieInput[] input)
        {
            byte[] head = ReadBytes(ref FileContents, 0, ControllerDataOffset);
            int controllers = Input.ControllerCount;
            int packSize = MMVSpecific.PackSize;

            // get the size of the file byte[] (minus the header)
            int size = input.Length * packSize;

            // create the output array and copy in the contents
            byte[] outputFile = new byte[head.Length + size];
            head.CopyTo(outputFile, 0);

            // add the controller data
            int position = 0;
            for (int i = 0; i < input.Length; i++)
            {
                outputFile[head.Length + position++] = parseControllerData(input[i].Controller[0]);
                outputFile[head.Length + position++] = parseControllerData(input[i].Controller[1]);
            }

            updateMetadata(ref outputFile);

            WriteByteArrayToFile(ref outputFile, filename, input.Length, Offsets[2]);
        }

        /// <summary>
        /// Update the metadata information in the MMV
        /// </summary>        
        private void updateMetadata(ref byte[] byteArray)
        {
            byte[] author = WriteChars(Extra.Author);
            for (int i = 0; i < Math.Min(author.Length, Offsets[9] - Offsets[8]); i++)
                byteArray[i + Offsets[8]] = author[i];
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
