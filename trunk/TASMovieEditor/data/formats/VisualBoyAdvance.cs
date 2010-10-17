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
    public class VisualBoyAdvance : TASMovie
    {
        const byte HEADER_SIZE = 64;
        const byte INFO_SIZE   = 192;

        /// <summary>
        /// Contains Format Specific items
        /// </summary>
        public struct FormatSpecific
        {
            public bool[] SystemType;
            public bool[] BIOSFlags;

            public FormatSpecific(int sys, int bios)
            {
                SystemType = new bool[4];
                BIOSFlags  = new bool[7];
 
                for (int i = 0; i < SystemType.Length; i++)
                    SystemType[i] = ((sys >> i) == 1) ? true : false;

                // if no other system is set, system type is GB
                if (SystemType[0] == false && SystemType[1] == false && SystemType[2] == false)
                    SystemType[3] = true;

                for (int j = 0; j < BIOSFlags.Length; j++)
                    BIOSFlags[j] = ((1 & (bios >> j)) == 1) ? true : false;
            }
        }        
        
        public FormatSpecific VBMSpecific;

        private string[] InputValues = { "A", "B", "s", "S", ">", "<", "^", "v", "R", 
                                         "L", "(reset)", "", "(left)", "(right)", "(down)", "(up)" };       
        private int[]    Offsets = {
            0x00, // 4-byte signature: 56 42 4D 1A "VBM\x1A"
            0x04, // 4-byte little-endian unsigned int: version number, must be 1
            0x08, // 4-byte little-endian integer: movie "uid" - identifies the movie-savestate relationship,
                  //    also used as the recording time in Unix epoch format
            0x0C, // 4-byte little-endian unsigned int: number of frames
            0x10, // 4-byte little-endian unsigned int: rerecord count
            0x14, // 1-byte flags (movie start flags):
                  //    bit 0: if "1", movie starts from an embedded "quicksave" snapshot; if "0", it doesn't
                  //    bit 1: if "1", movie starts from reset with an embedded SRAM; if "0", it doesn't
                  //    (NOTE: if both bits 0 and 1 are "0", then movie begins from power on with no SRAM or save state.
                  //    if both bits 0 and 1 are "1", the movie file is invalid.)
                  //    other: reserved, set to 0
            0x15, // 1-byte flags (controller flags):
                  //    bit 0: controller 1 in use
                  //    bit 1: controller 2 in use (SGB games can be 2-player multiplayer)
                  //    bit 2: controller 3 in use (SGB games can be 3- or 4-player multiplayer with multitap)
                  //    bit 3: controller 4 in use (SGB games can be 3- or 4-player multiplayer with multitap)
                  //    other: reserved
            0x16, // 1-byte flags:  (system flags - note that no matter what these are, the game always runs at 60 frames/sec)
                  //    bit 0: if "1", movie is for the GBA system; if "0", it isn't.
                  //    bit 1: if "1", movie is for the GBC system; if "0", it isn't.
                  //    bit 2: if "1", movie is for the SGB system; if "0", it isn't. (If all 3 of these bits are "0", it's for regular GB.)
                  //    (NOTE:  The above 3 bits are mutually exclusive, although a given ROM can be playable on multiple systems.)
                  //    other: reserved, set to 0
            0x17, // 1-byte flags:  (values of some boolean emulator options)
                  //    bit 0: (useBiosFile)
                  //    if "1" and the movie is of a GBA game, the movie was made using a GBA BIOS file, otherwise it wasn't.
                  //    bit 1: (skipBiosFile)
                  //    if "0" and the movie was made with a GBA BIOS file, the BIOS intro is included in the movie, otherwise it isn't.
                  //    bit 2: (rtcEnable)
                  //    if "1", the emulator's "real time clock" feature was enabled; if "0", it wasn't.
                  //    bit 3: (unsupported)
                  //    must be "0" or the movie file is considered invalid (legacy)
                  //    bit 4: (lagReduction)
                  //    if "0" and the movie is of a GBA game, the movie was made using the old excessively laggy GBA timing, otherwise it wasn't.
                  //    bit 5: (gbcHdma5Fix)
                  //    if "0" and the movie is of a GBC game, the movie was made using the old buggy HDMA5 timing, otherwise it wasn't.
                  //    bit 6: (echoRAMFix)
                  //    if "1" and the movie is of a GB, GBC, or SGB game, the movie was made with Echo RAM Fix on, otherwise it wasn't.
                  //    other: reserved, set to 0
            0x18, // 4-byte little-endian unsigned int: theApp.winSaveType (value of that emulator option)
            0x1C, // 4-byte little-endian unsigned int: theApp.winFlashSize (value of that emulator option)
            0x20, // 4-byte little-endian unsigned int: gbEmulatorType (value of that emulator option)
            0x24, // 12-byte character array: the internal game title of the ROM
                  //    used while recording, not necessarily null-terminated (ASCII?)
            0x30, // 1-byte: reserved, set to 0
            0x31, // 1-byte unsigned char: the internal CRC of the ROM used while recording
            0x32, // 2-byte little-endian unsigned short: the internal Checksum of the ROM used
                  //    while recording, or a calculated CRC16 of the BIOS if GBA
            0x34, // 4-byte little-endian unsigned int: the Game Code of the ROM used
                  //    while recording, or the Unit Code if not GBA
            0x38, // 4-byte little-endian unsigned int: offset to the savestate
                  //    or SRAM inside file, set to 0 if unused
            0x3C  // 4-byte little-endian unsigned int: offset to the controller data inside file
        };

        public VisualBoyAdvance(string VBMFile)
        {
            Filename = VBMFile;
            FillByteArrayFromFile(VBMFile, ref FileContents);

            SaveStateOffset      = Read32(ref FileContents, Offsets[17]);
            ControllerDataOffset = Read32(ref FileContents, Offsets[18]);
            
            Header = new TASHeader();
            Header.Signature     = ReadHEX(ref FileContents, Offsets[0], 4);
            Header.Version       = Read32(ref FileContents, Offsets[1]);
            Header.UID           = ConvertUNIXTime(Read32(ref FileContents, Offsets[2]));
            Header.FrameCount    = Read32(ref FileContents, Offsets[3]);    // FIXME: unreliable
            Header.RerecordCount = Read32(ref FileContents, Offsets[4]);
            
            Options = new TASOptions(true);
            Options.MovieStartFlag[0] = ((1 & (FileContents[Offsets[5]] >> 0)) == 1) ? true : false;
            Options.MovieStartFlag[1] = ((1 & (FileContents[Offsets[5]] >> 1)) == 1) ? true : false;
            Options.MovieStartFlag[2] = (!Options.MovieStartFlag[0] && !Options.MovieStartFlag[1]) ? true : false;

            int activeControllers = 0;
            Input = new TASInput(4, false);
            for (int i = 0; i < 4; i++)
            {
                if ((1 & (FileContents[Offsets[6]] >> i)) == 1)
                {
                    Input.Controllers[i] = true;
                    activeControllers++;
                }
                else
                {
                    Input.Controllers[i] = false;
                }
            }
            // deduce the actual frame count from the file content
            Header.FrameCount = (FileContents.Length - ControllerDataOffset) / (activeControllers * 2);

            Extra = new TASExtra();
            Extra.Author      = ReadChars(ref FileContents, HEADER_SIZE, 64);
            Extra.Description = ReadChars(ref FileContents, HEADER_SIZE + 64, 128);
            Extra.ROM         = ReadChars(ref FileContents, Offsets[12], 12);
            Extra.CRC         = Convert.ToString((int)Offsets[14]);

            VBMSpecific = new FormatSpecific(FileContents[Offsets[7]], FileContents[Offsets[8]]);

            getFrameInput(ref FileContents);
        }

        private void getFrameInput(ref byte[] byteArray)
        {
            Input.FrameData = new TASMovieInput[Header.FrameCount];            
            int position = 0;
            int i = 0;

            while (position < Header.FrameCount)
            {                
                Input.FrameData[position] = new TASMovieInput();
                for (int j = 0; j < 4; j++)
                {
                    if (Input.Controllers[j])
                    {
                        byte[] frame = ReadBytes(ref byteArray, ControllerDataOffset + i + j, 2);
                        Input.FrameData[position].Controller[j] = parseControllerData(frame);
                        i++;
                    }                    
                }
                position++; i++;
            }                        
        }

        /// <summary>
        /// Parse the 2-byte controller state value to a string
        /// </summary>        
        private string parseControllerData(byte[] bytes)
        {
            string input = "";

            for (int i = 0; i < 8; i++)
            {
                if ((1 & (bytes[0] >> i)) == 1) input += InputValues[i];
                if ((1 & (bytes[1] >> i)) == 1) input += InputValues[i + 8];
            }

            return input;
        }

        /// <summary>
        /// Convert the string input to a 2-byte controller state value 
        /// </summary>      
        private byte[] parseControllerData(string frameInput)
        {
            byte[] input = { 0x00, 0x00 };
            
            if (frameInput == null || frameInput == "") return input;

            for (int i = 0; i < 8; i++)
            {
                if (frameInput.Contains(InputValues[i])) input[0] |= (byte)(1 << i);
                // input at InputValues[10] not used
                if (frameInput.Contains(InputValues[i + 8]) && i != 3) input[1] |= (byte)(1 << i);            
            }                            

            return input;
        }

        /// <summary>
        /// Save the contenst of the frame data back out to a VBM file
        /// 
        /// TODO::This isn't the most elegant solution, but there's a major performance hit
        /// if I try to repeatedly resize the array from within the loop.
        /// </summary>        
        public override void Save(string filename, ref TASMovieInput[] input)
        {
            byte[] head = ReadBytes(ref FileContents, 0, ControllerDataOffset);
            int size = 0;
            int controllers = Input.ControllerCount;

            // get the size of the file byte[] (minus the header)
            for (int i = 0; i < input.Length; i++)
                for (int j = 0; j < controllers; j++)                                   
                        size += 2;

            // create the output array and copy in the contents
            byte[] outputFile = new byte[head.Length + size];
            head.CopyTo(outputFile, 0);

            // add the controller data
            int position = 0;
            for (int i = 0; i < input.Length; i++)
            {               
                for (int j = 0; j < controllers; j++)
                {
                    // check if the controller we're about to process is used
                    if (Input.Controllers[j])
                    {
                        byte[] parsed = parseControllerData(input[i].Controller[j]);
                        outputFile[head.Length + position++] = parsed[0];
                        outputFile[head.Length + position++] = parsed[1];                        
                    }                    
                }
            }
            // update the movie description and author
            for (int i = 0; i < 64; i++)
                if (i < Extra.Author.Length)
                    outputFile[HEADER_SIZE + i] = Convert.ToByte(Extra.Author[i]);
                else
                    outputFile[HEADER_SIZE + i] = 0;
            for (int j = 0; j < 128; j++)
                if (j < Extra.Description.Length)
                    outputFile[HEADER_SIZE + 64 + j] = Convert.ToByte(Extra.Description[j]);
                else
                    outputFile[HEADER_SIZE + 64 + j] = 0;


            //// DEBUGGING //
            //MovieSplicer.UI.frmDebug frm = new MovieSplicer.UI.frmDebug();
            //for (int i = 0; i < FileContents.Length; i++)
            //{
            //    System.Windows.Forms.ListViewItem lvi = new System.Windows.Forms.ListViewItem();
            //    lvi.Text = i.ToString();
            //    lvi.SubItems.Add(FileContents[i].ToString());
            //    string item = (i < outputFile.Length) ? outputFile[i].ToString() : "out of range";
            //    lvi.SubItems.Add(item);
            //    frm.Add(lvi);
            //}
            //frm.Show();
            /////////////////

            WriteByteArrayToFile(ref outputFile, filename, input.Length - 1, Offsets[3]);            
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
