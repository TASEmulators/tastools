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

namespace MovieSplicer.Data.Formats
{
    public class Gens : TASMovie
    {
        /// <summary>
        /// Contains Format Specific items
        /// </summary>
        public struct FormatSpecific
        {
            public string Player1Config;
            public string Player2Config; 
        }   

        const byte HEADER_SIZE = 64;
        public FormatSpecific GMVSpecific;
        
        private string[] InputValues = { "^", "v", "<", ">", "A", "B", "C", "S", "X", "Y", "Z", "M" };
        private int[]    Offsets = {
            0x00, // 16-byte signature and format version: "Gens Movie TEST9"
            0x0F, // ASCII-encoded GMV file format version. The most recent is 'A'. (?)
            0x10, // 4-byte little-endian unsigned int: rerecord count
            0x14, // ASCII-encoded controller config for player 1. '3' or '6'.
            0x15, // ASCII-encoded controller config for player 2. '3' or '6'.
            0x16, // has some special flags (Version A and up only):
                  //    bit 7 (most significant) Frames per second. 1 for 50 Hz, 0 for 60Hz.
                  //    bit 6 is savestate required. 1 = savestate required.
                  //    bit 5 is 3 player movie. 1 = 3 players, 0 = 2 players.
            0x18, // 40-byte zero-terminated ascii movie name string
            0x40  // frame data begins here           
        };

        public Gens(string GMVFile)
        {
            Filename = GMVFile;
            FillByteArrayFromFile(GMVFile, ref FileContents);            

            Header = new TASHeader();
            Header.Signature = ReadChars(ref FileContents, Offsets[0], 16);
            Header.Version = FileContents[Offsets[1]] - 55;
            Header.FrameCount = Convert.ToInt32((FileContents.Length - HEADER_SIZE) / 3);
            Header.RerecordCount = Read32(ref FileContents, Offsets[2]);         

            if (Header.Version > 9)
            {
                Options = new TASOptions(true);
                Options.FPS = (1 & (FileContents[Offsets[5]] >> 7)) == 1 ? 50 : 60;
                Options.MovieStartFlag[0] = (1 & (FileContents[Offsets[5]] >> 6)) == 1 ? true : false;
                Options.MovieStartFlag[1] = (1 & (FileContents[Offsets[5]] >> 6)) == 0 ? true : false;
                Input = (1 & (FileContents[Offsets[5]] >> 5)) == 1 ? new TASInput(3, true) : new TASInput(2, true);
            }
            else
                Input = new TASInput(2, true);
                         
            Extra = new TASExtra();
            try { Extra.Description = ReadCharsNullTerminated(ref FileContents, Offsets[6]); }
            catch { Extra.Description = ReadChars(ref FileContents, Offsets[6], Offsets[7] - Offsets[6]); }

            GMVSpecific = new FormatSpecific();
            GMVSpecific.Player1Config = ReadChars(ref FileContents, Offsets[3], 1);
            GMVSpecific.Player2Config = ReadChars(ref FileContents, Offsets[4], 1);

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
            int frame = 0;

            // there are 3 bytes per frame of input
            for (int i = 0; i < Header.FrameCount * 3; i++)
            {
                Input.FrameData[frame] = new TASMovieInput();

                // there are only 3 bytes to deal with, so no need to 
                // run the array through a loop to be populated
                Input.FrameData[frame].Controller[0] = parseControllerData(byteArray[Offsets[7] + i]);
                Input.FrameData[frame].Controller[1] = parseControllerData(byteArray[Offsets[7] + i + 1]);

                if (Input.ControllerCount == 3)
                    Input.FrameData[frame].Controller[2] = parseControllerData(byteArray[Offsets[7] + i + 2]);
                else
                    parseExtraControllerData(byteArray[Offsets[7] + i + 2],
                        ref Input.FrameData[frame].Controller[0],
                        ref Input.FrameData[frame].Controller[1]);
                
                // increase the iterator (since we've dealt with values > i)
                i += 2; frame++;
            }
        }

        /// <summary>
        /// Convert byte values to string input
        /// </summary>
        private string parseControllerData(byte frameValue)
        {
            string input = "";

            for (int i = 0; i < 8; i++)
                if (((1 & frameValue >> i)) == 0) input += InputValues[i];
           
            return input;
        }

        /// <summary>
        /// Convert string input to a byte value
        /// </summary> 
        private byte parseControllerData(string frameInput)
        {
            byte input = 0xFF;
            if (frameInput != null && frameInput != "")
            {                
                for (int i = 0; i < 8; i++)
                    if (frameInput.Contains(InputValues[i])) input -= (byte)(1 << i);                
            }
            return input;
        }

        /// <summary>
        /// Convert byte values to string input if 2-player 6-button
        /// </summary>
        private void parseExtraControllerData(byte frameValue, ref string player1, ref string player2)
        {            
            for (int i = 0; i < 4; i++)
            {
                if (((1 & frameValue >> i)) == 0)     player1 += InputValues[i + 8];
                if (((1 & frameValue >> i + 4)) == 0) player2 += InputValues[i + 8];
            }
        }

        /// <summary>
        /// Convert byte values to string input if 2-player 6-button
        /// </summary>
        private byte parseExtraControllerData(string player1, string player2)
        {
            byte input = 0xFF;

            if (player1 != null && player1 != "")
            {
                for (int i = 0; i < 4; i++)
                {
                    if (player1.Contains(InputValues[i + 8])) input -= (byte)(1 << i);
                }
            }

            if (player2 != null && player2 != "")
            {
                for (int i = 0; i < 4; i++)
                {
                    if (player2.Contains(InputValues[i + 8])) input -= (byte)(1 << i + 4);
                }
            }

            return input;
        }

        /// <summary>
        /// Save a new GMV file (if a string null is passed, prompt for overwrite of the source file)
        /// </summary>        
        public override void Save(string filename, ref TASMovieInput[] input)
        {         
            byte[] head = ReadBytes(ref FileContents, 0, HEADER_SIZE);

            // create the output array and copy in the contents
            byte[] outputFile = new byte[head.Length + input.Length * 3];
            head.CopyTo(outputFile, 0);

            // add the controller data
            int position = 0;
            int controllers = Input.ControllerCount;
            for (int i = 0; i < input.Length; i++)
            {
                outputFile[head.Length + position++] = parseControllerData(input[i].Controller[0]);
                outputFile[head.Length + position++] = parseControllerData(input[i].Controller[1]);

                if (controllers == 3)
                    outputFile[head.Length + position++] = parseControllerData(input[i].Controller[2]);
                else if (GMVSpecific.Player1Config == "6" || GMVSpecific.Player2Config == "6")
                    outputFile[head.Length + position++] = 
                        parseExtraControllerData(input[i].Controller[0], input[i].Controller[1]);
                else
                    outputFile[head.Length + position++] = 0xFF;
            }
            
            
            // update the movie description
            for (int i = 0; i < 40; i++)
                if  (i < Extra.Description.Length) 
                    outputFile[Offsets[6] + i] = Convert.ToByte(Extra.Description[i]);
                else
                    outputFile[Offsets[6] + i] = 0;

            // NOTE::GMV files calculate frameCount based on filesize
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
