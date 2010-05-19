/******************************************************************************** 
 * TAS Movie Editor                                                             *
 *                                                                              *
 * Copyright notice for this file:                                              *
 *  Copyright (C) 2006-7 Maximus                                                *
 *  Copyright (C) 2008   bkDJ                                                   *
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
using System.IO;

namespace MovieSplicer.Data.Formats
{
    public class Mupen64 : TASMovie
    {
        /// <summary>
        /// Contains Format Specific items
        /// </summary>
        public struct FormatSpecific
        {
            public ControllerConfig[] Controller;
            public string VideoPlugin;
            public string AudioPlugin;
            public string InputPlugin;
            public string RSPPlugin;            

            public FormatSpecific(byte[] controllers)
            {
                Controller = new ControllerConfig[4];                

                for (int i = 0; i < 4; i++)
                {
                    Controller[i].Option = new bool[3];
                    if ((1 & (controllers[i] << 0)) == 1) Controller[i].Option[0] = true;                        
                    if ((1 & (controllers[i] << 4)) == 1) Controller[i].Option[1] = true;
                    if ((1 & (controllers[i] << 8)) == 1) Controller[i].Option[2] = true;
                }

                // directly initialized when instantiated
                VideoPlugin = null;
                AudioPlugin = null;
                InputPlugin = null;
                RSPPlugin = null;
            }
        }

        /// <summary>
        /// The Option (bool array) contains configuration information about a controller                
        /// </summary>
        public struct ControllerConfig
        {
            public bool[] Option;
        }

        const short HEADER_SIZE     = 1024;
        const byte  BYTES_PER_FRAME = 4;

        public FormatSpecific M64Specific;

        private string[] InputValues = { "D", "S", "Z", "B", "A", "C", "R", "L", "X", "Y" };
        private string[] InputDir    = { ">", "<", "v", "^" };
        private int      ControllerInputs;
        private int[]    Offsets = {
          0x00, // 4-byte signature: 4D 36 34 1A "M64\x1A"
          0x04, // 4-byte little-endian unsigned int: version number, should be 3
          0x08, // 4-byte little-endian integer: movie "uid" - identifies the movie-savestate relationship,
                //      also used as the recording time in Unix epoch format
          0x0C, // 4-byte little-endian unsigned int: number of frames (vertical interrupts)
          0x10, // 4-byte little-endian unsigned int: rerecord count
          0x14, // 1-byte unsigned int: frames (vertical interrupts) per second
          0x15, // 1-byte unsigned int: number of controllers
          0x16, // 2-byte unsigned int: reserved, should be 0
          0x18, // 4-byte little-endian unsigned int: number of input samples for any controllers
          0x1C, // 2-byte unsigned int: movie start type
                //      value 1: movie begins from snapshot (the snapshot will be loaded from an external file
                //               with the movie's filename and a .st extension)
                //      value 2: movie begins from poweron  
                //      other values: invalid movie
          0x1E, // 2-byte unsigned int: reserved, should be 0
          0x20, // 4-byte unsigned int: controller flags
                //      bit 0: controller 1 present
                //      bit 4: controller 1 has mempak
                //      bit 8: controller 1 has rumblepak
                //      +1..3 for controllers 2..4.
          0x24, // 160 bytes: reserved, should be 0
          0xC4, // 32-byte ASCII string: internal name of ROM used when recording, directly from ROM
          0xE4, // 4-byte unsigned int: CRC32 of ROM used when recording, directly from ROM
          0xE8, // 2-byte unsigned int: country code of ROM used when recording, directly from ROM
          0xEA, // 56 bytes: reserved, should be 0
          0x122,// 64-byte ASCII string: name of video plugin used when recording, directly from plugin
          0x162,// 64-byte ASCII string: name of sound plugin used when recording, directly from plugin
          0x1A2,// 64-byte ASCII string: name of input plugin used when recording, directly from plugin
          0x1E2,// 64-byte ASCII string: name of rsp plugin used when recording, directly from plugin
          0x222,// 222-byte UTF-8 string: author's name info
          0x300 // 256-byte UTF-8 string: author's movie description info
        };

        /// <summary>
        /// Create a fully instantiated M64 object from the passed file
        /// </summary>        
        public Mupen64(string M64File)
        {
            Filename = M64File;
            FillByteArrayFromFile(M64File, ref FileContents);                        
            
            Header = new TASHeader();
            Header.Signature     = ReadHEX(ref FileContents, Offsets[0], 4);
            Header.Version       = Read32(ref FileContents, Offsets[1]);
            Header.UID           = ConvertUNIXTime(Read32(ref FileContents, Offsets[2]));
            Header.FrameCount    = Read32(ref FileContents, Offsets[3]);
            Header.RerecordCount = Read32(ref FileContents, Offsets[4]);

            // Though ControllerInputs is stored in the header, it can be determined 
            // mathematically, so we may as well do it once instead of twice ;)
            //ControllerInputs     = Read32(ref FileContents, Offsets[8]);
            ControllerInputs = Read32(ref FileContents, Offsets[8]);

            if (Header.Version >= 3)
            {
                //ControllerInputs = (FileContents.Length - 0x400) / BYTES_PER_FRAME;
                ControllerDataOffset = 0x400;
            }
            else
            {
                //ControllerInputs = (FileContents.Length - 0x200) / BYTES_PER_FRAME;
                ControllerDataOffset = 0x200;
            }

            Options = new TASOptions(true);
            Options.MovieStartFlag[0] = (FileContents[Offsets[9]] | FileContents[Offsets[9] + 1]) == 1 ? true : false;
            Options.MovieStartFlag[2] = (FileContents[Offsets[9]] | FileContents[Offsets[9] + 1]) == 2 ? true : false;
            Options.FPS = FileContents[Offsets[5]];

            Extra = new TASExtra();
            Extra.ROM         = ReadChars(ref FileContents, Offsets[13], 64);
            Extra.CRC         = ReadHEXUnicode(ref FileContents, Offsets[14], 4);
            Extra.Country     = ReadChars(ref FileContents, Offsets[15], 2);
            Extra.Author      = ReadChars(ref FileContents, Offsets[21], 222);
            Extra.Description = ReadChars(ref FileContents, Offsets[22], 256);

            M64Specific = new FormatSpecific(ReadBytes(ref FileContents, Offsets[11], 4));
            M64Specific.VideoPlugin = ReadChars(ref FileContents, Offsets[17], 64);
            M64Specific.AudioPlugin = ReadChars(ref FileContents, Offsets[18], 64);
            M64Specific.InputPlugin = ReadChars(ref FileContents, Offsets[19], 64);
            M64Specific.RSPPlugin   = ReadChars(ref FileContents, Offsets[20], 64);

            Input = new TASInput(4, false);
            for (int i = 0; i < 4; i++)
                Input.Controllers[i] = M64Specific.Controller[i].Option[0];                       

            getFrameInput(ref FileContents);
        }

        private void getFrameInput(ref byte[] byteArray)
        {
            // NOTE::ControllerInputs is how many times the game polled for data, NOT how many VIs happen during a movie
            Input.FrameData = new TASMovieInput[ControllerInputs];                        

            // parse frame data
            for (int i = 0; i < ControllerInputs; i++)            
            {
                Input.FrameData[i] = new TASMovieInput();
               
                // cycle through the controller data for the current frame
                for (int j = 0; j < Input.ControllerCount; j++)
                {
                    byte[] frame = ReadBytes(ref byteArray,
                        ControllerDataOffset + (i * BYTES_PER_FRAME) + (j * BYTES_PER_FRAME),
                        BYTES_PER_FRAME);

                    Input.FrameData[i].Controller[j] = parseControllerData(frame);
                }                   
            }                       
        } 
     
        /// <summary>
        /// Convert the binary representation of input to meaningful values 
        /// </summary>
        private string parseControllerData(byte[] byteArray)
        {
            bool[] D = { false, false, false, false };
            bool[] C = { false, false, false, false };
            sbyte x = 0;
            sbyte y = 0;

            string frame = "";

            D[0] = (byteArray[0] & (0x01)) != 0;
            D[1] = (byteArray[0] & (0x02)) != 0;
            D[2] = (byteArray[0] & (0x04)) != 0;
            D[3] = (byteArray[0] & (0x08)) != 0;
            if ((byteArray[0] & (0x10)) != 0) frame += InputValues[1];
            if ((byteArray[0] & (0x20)) != 0) frame += InputValues[2];
            if ((byteArray[0] & (0x40)) != 0) frame += InputValues[3];
            if ((byteArray[0] & (0x80)) != 0) frame += InputValues[4];
            C[0] = (byteArray[1] & (0x01)) != 0;
            C[1] = (byteArray[1] & (0x02)) != 0;
            C[2] = (byteArray[1] & (0x04)) != 0;
            C[3] = (byteArray[1] & (0x08)) != 0;
            if ((byteArray[1] & (0x10)) != 0) frame += InputValues[6];
            if ((byteArray[1] & (0x20)) != 0) frame += InputValues[7];
            x = (sbyte)byteArray[2];
            y = (sbyte)byteArray[3];

            if (D[0] || D[1] || D[2] || D[3])
            {
                frame += "(" + InputValues[0];
                for (int i = 0; i < 4; i++)
                    if (D[i]) frame += InputDir[i];
                frame += ")";
            }
            if (C[0] || C[1] || C[2] || C[3])
            {
                frame += "(" + InputValues[5];
                for (int i = 0; i < 4; i++)
                    if (C[i]) frame += InputDir[i];
                frame += ")";
            }

            if (x != 0)
            {
                frame += "(";
                frame += InputValues[8];
                frame += (x < 0) ? InputDir[0] : InputDir[1];
                int xprint = 0;
                xprint = (x < 0) ? -(int)x : (int)x;
                frame += xprint.ToString();
                frame += ")";
            }
            if (y != 0)
            {
                frame += "(";
                frame += InputValues[9];
                frame += (y < 0) ? InputDir[2] : InputDir[3];
                int yprint = 0;
                yprint = (y < 0) ? -(int)y : (int)y;
                frame += yprint.ToString();
                frame += ")";
            }

            return frame;
        }


        /// <summary>
        /// Convert the string representation of input back to binary values
        /// </summary>
        private byte[] parseControllerData(string frameInput)
        {
            byte[] input = { 0x00, 0x00, 0x00, 0x00 };

            if (frameInput == null || frameInput == "") return input;

            //first we take care of analog
            if (frameInput.Contains(InputValues[1])) input[0] |= (byte)(0x10);  // Start
            if (frameInput.Contains(InputValues[2])) input[0] |= (byte)(0x20);  // Z
            if (frameInput.Contains(InputValues[3])) input[0] |= (byte)(0x40);  // B
            if (frameInput.Contains(InputValues[4])) input[0] |= (byte)(0x80);  // A
            if (frameInput.Contains(InputValues[6])) input[1] |= (byte)(0x10);  // R
            if (frameInput.Contains(InputValues[7])) input[1] |= (byte)(0x20);  // L

            //then we take care of directions for D-pad...
            if (frameInput.Contains(InputValues[0]))
            {
                int nStart = frameInput.IndexOf(InputValues[0][0]) + 1;
                int nEnd = frameInput.IndexOf(')', nStart);
                string dirs = frameInput.Substring(nStart, nEnd - nStart);

                if (frameInput.Contains(InputDir[0])) input[0] |= (byte)(0x01);  // Right
                if (frameInput.Contains(InputDir[1])) input[0] |= (byte)(0x02);  // Left
                if (frameInput.Contains(InputDir[2])) input[0] |= (byte)(0x04);  // Down
                if (frameInput.Contains(InputDir[3])) input[0] |= (byte)(0x08);  // Up
            }

            //..and directions for C
            if (frameInput.Contains(InputValues[5]))
            {
                int nStart = frameInput.IndexOf(InputValues[5][0]) + 1;
                int nEnd = frameInput.IndexOf(')', nStart);
                string dirs = frameInput.Substring(nStart, nEnd - nStart);

                if (frameInput.Contains(InputDir[0])) input[1] |= (byte)(0x01);  // Right
                if (frameInput.Contains(InputDir[1])) input[1] |= (byte)(0x02);  // Left
                if (frameInput.Contains(InputDir[2])) input[1] |= (byte)(0x04);  // Down
                if (frameInput.Contains(InputDir[3])) input[1] |= (byte)(0x08);  // Up
            }

            // Analog stick X axis
            if (frameInput.Contains(InputValues[8]))
            {
                int nStart = frameInput.IndexOf(InputValues[8][0]) + 2;
                int nEnd = frameInput.IndexOf(')', nStart);
                string num = frameInput.Substring(nStart, nEnd - nStart);
                Int32 xaxis = Convert.ToInt32(num);
                if (frameInput[nStart - 1] == InputDir[0][0])
                {
                    // number should be negative
                    if (xaxis > 128) xaxis = 128;
                    xaxis = -xaxis;
                }
                else
                {
                    // number is positive
                    if (xaxis > 127) xaxis = 127;
                }
                byte x = (byte)xaxis;
                input[2] = x;
            }

            // Analog stick Y axis
            if (frameInput.Contains(InputValues[9]))
            {
                int nStart = frameInput.IndexOf(InputValues[9][0]) + 2;
                int nEnd = frameInput.IndexOf(')', nStart);
                string num = frameInput.Substring(nStart, nEnd - nStart);
                Int32 yaxis = Convert.ToInt32(num);
                if (frameInput[nStart - 1] == InputDir[2][0])
                {
                    // number should be negative
                    if (yaxis > 128) yaxis = 128;
                    yaxis = -yaxis;
                }
                else
                {
                    // number is positive
                    if (yaxis > 127) yaxis = 127;
                }
                byte y = (byte)yaxis;
                input[3] = y;
            }

            return input;
        }

        /// <summary>
        /// Save the content of the frame data back out to an M64 file
        /// </summary>        
        public override void Save(string filename, ref TASMovieInput[] input)
        {
            byte[] head = ReadBytes(ref FileContents, 0, ControllerDataOffset);
            int size = 0;
            int controllers = Input.ControllerCount;

            // get the size of the file byte[] (minus the header)
            for (int i = 0; i < input.Length; i++)
                for (int j = 0; j < controllers; j++)
                    size += BYTES_PER_FRAME;

            // create the output array and copy in the contents
            byte[] outputFile = new byte[head.Length + size + (BYTES_PER_FRAME * controllers)];
            head.CopyTo(outputFile, 0);

            // add the controller data
            int position = 0;
            for (int i = 0; i < input.Length; i++)
                for (int j = 0; j < controllers; j++)
                    // check if the controller we're about to process is used
                    if (Input.Controllers[j])
                    {
                        byte[] parsed = parseControllerData(input[i].Controller[j]);
                        outputFile[head.Length + position++] = parsed[0];
                        outputFile[head.Length + position++] = parsed[1];
                        outputFile[head.Length + position++] = parsed[2];
                        outputFile[head.Length + position++] = parsed[3];
                    }

            updateMetadata(ref outputFile);

            WriteByteArrayToFile(ref outputFile, filename, input.Length, Offsets[8]);

            //return true;
        }

        /// <summary>
        /// Update the metadata information in the M64
        /// </summary>        
        private void updateMetadata(ref byte[] byteArray)
        {
            // TODO: convert this to be m64 compliant insted of stolen from smv

            //int startPos = Offsets[11];
            //int extraROMLength = (SMVSpecific.HASROMINFO) ? Convert.ToInt32(EXTRAROMINFO_SIZE) : 0;
            //byte[] author = WriteChars16(Extra.Author);
            //byte[] temp = new byte[startPos + author.Length + (byteArray.Length - SaveStateOffset + extraROMLength)];

            //int newSaveStateOffset = startPos + author.Length + extraROMLength;
            //int newCDataOffset = ControllerDataOffset + (newSaveStateOffset - SaveStateOffset);

            //for (int i = 0; i < startPos; i++)
            //    temp[i] = byteArray[i];
            //for (int j = 0; j < author.Length; j++)
            //    temp[j + startPos] = author[j];
            //for (int k = 0; k < byteArray.Length - SaveStateOffset + extraROMLength; k++)
            //    temp[k + newSaveStateOffset - extraROMLength] = byteArray[k + SaveStateOffset - extraROMLength];

            //Write32(ref temp, Offsets[9], newSaveStateOffset);
            //Write32(ref temp, Offsets[10], newCDataOffset);

            //byteArray = temp;
        }

        public override string[] GetUsableInputs()
        {
            System.Collections.ArrayList inputsArray = new System.Collections.ArrayList();
            for (int i = 0; i < InputValues.Length; i++)
            {
                if (InputValues[i] == "") continue;
                inputsArray.Add(InputValues[i]);
            }
            for (int i = 0; i < InputDir.Length; i++)
            {
                if (InputDir[i] == "") continue;
                inputsArray.Add(InputDir[i]);
            }
            return (string[])inputsArray.ToArray(typeof(string));
        }

    }    
}
