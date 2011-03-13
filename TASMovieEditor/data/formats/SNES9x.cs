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
    public class SNES9x : TASMovie
    {
        /// <summary>
        /// Contains Format Specific items
        /// </summary>
        public struct FormatSpecific
        {
            public bool DATA_EXISTS;
            public bool WIP1TIMING;
            public bool LEFTRIGHT;
            public bool VOLUMEENVX;
            public bool FAKEMUTE;
            public bool SYNCSOUND;
            public bool HASROMINFO;
            public bool NOCPUSHUTDOWN;

            public FormatSpecific(byte options)
            {
                DATA_EXISTS   = (1 & (options >> 0)) == 1 ? true : false;
                if (DATA_EXISTS)
                {
                    WIP1TIMING = (1 & (options >> 1)) == 1 ? true : false;
                    LEFTRIGHT = (1 & (options >> 2)) == 1 ? true : false;
                    VOLUMEENVX = (1 & (options >> 3)) == 1 ? true : false;
                    FAKEMUTE = (1 & (options >> 4)) == 1 ? true : false;
                    SYNCSOUND = (1 & (options >> 5)) == 1 ? true : false;
                    HASROMINFO = (1 & (options >> 6)) == 1 ? true : false;
                    NOCPUSHUTDOWN = (1 & (options >> 7)) == 1 ? true : false;
                }
                else
                {
                    WIP1TIMING    = false;
                    LEFTRIGHT     = false;
                    VOLUMEENVX    = false;
                    FAKEMUTE      = false;
                    SYNCSOUND     = false;
                    HASROMINFO    = false;
                    NOCPUSHUTDOWN = false;
                }
            }
        } 

        const byte BYTES_PER_FRAME   = 2;
        const byte EXTRAROMINFO_SIZE = 30;        

        public FormatSpecific SMVSpecific;

        private string[] InputValues = { ">", "<", "v", "^", "S", "s", "Y", "B", "!", "0", "1", "2", "R", "L", "X", "A" };
        private int[]    Offsets = {
            0x00, // 4-byte signature: 53 4D 56 1A "SMV\x1A"
            0x04, // 4-byte little-endian unsigned int: version number, must be either 1 (Snes9x v1.43) or 4 (Snes9x v1.51)
            0x08, // 4-byte little-endian integer: movie "uid" - recording time in Unix epoch format
            0x0C, // 4-byte little-endian unsigned int: rerecord count
            0x10, // 4-byte little-endian unsigned int: number of frames
            0x14, // 1-byte flags "controller mask":
                  //    bit 0: controller 1 in use
                  //    bit 1: controller 2 in use
                  //    bit 2: controller 3 in use
                  //    bit 3: controller 4 in use
                  //    bit 4: controller 5 in use
                  //    other: reserved, set to 0
            0x15, // 1-byte flags "movie options":
                  //    bit 0:
                  //       if "0", movie begins from an embedded "quicksave" snapshot
                  //       if "1", a SRAM is included instead of a quicksave; movie begins from reset
                  //    bit 1: if "0", movie is NTSC (60 fps); if "1", movie is PAL (50 fps)
                  //    other: reserved, set to 0
            0x16, // 1-byte flags "sync options":
                  //    bit 0: MOVIE_SYNC2_INIT_FASTROM
                  //       this bit is only used for Snes9x 1.43 and should be "0" for v1.51
                  //    other: reserved, set to 0
            0x17, // 1-byte flags "sync options":
                  //    bit 0: MOVIE_SYNC_DATA_EXISTS
                  //       if "1", the following bits are defined.
                  //       if "0", the following bits have no meaning.
                  //       this bit is only used for Snes9x v1.43 and should be "0" for v1.51.
                  //    bit 1: MOVIE_SYNC_WIP1TIMING
                  //    bit 2: MOVIE_SYNC_LEFTRIGHT
                  //    bit 3: MOVIE_SYNC_VOLUMEENVX
                  //    bit 4: MOVIE_SYNC_FAKEMUTE
                  //    bit 5: MOVIE_SYNC_SYNCSOUND
                  //    bit 6: MOVIE_SYNC_HASROMINFO
                  //       if "1", there is extra ROM info located right in between of the metadata and the savestate.
                  //    bit 7: MOVIE_SYNC_NOCPUSHUTDOWN
                  //       if "0", the "SpeedHacks" config option is on.
                  //       if "1", the "SpeedHacks" is off.
                  //       this bit is only used for Snes9x v1.51 and should be "0" for v1.43.
            0x18, // 4-byte little-endian unsigned int: offset to the savestate inside file
            0x1C, // 4-byte little-endian unsigned int: offset to the controller data inside file
            0x20, // Snes9x v1.43: UTF16-coded movie title string (author info)
                  // Snes9x v1.51: 4-byte little-endian unsigned int: number of input samples, primarily for peripheral-using games

                  // Snes9x v1.51 has additional bytes at the end of the header.
            0x24, // 2-byte unsigned ints: what type of controller is plugged into ports 1 and 2 respectively: 0=NONE, 1=JOYPAD, 2=MOUSE, 3=SUPERSCOPE, 4=JUSTIFIER, 5=MULTITAP
            0x26, // 4-byte signed ints: controller IDs of port 1, or -1 for unplugged
            0x2A, // 4-byte signed ints: controller IDs of port 2, or -1 for unplugged
            0x2E, // 18 bytes: reserved for future use
            0x40, // UTF16-coded movie title string (author info)
        };

        public SNES9x(string SMVFile)
        {
            Filename = SMVFile;
            FillByteArrayFromFile(SMVFile, ref FileContents);

            ControllerDataOffset = Read32(ref FileContents, Offsets[10]);
            SaveStateOffset      = Read32(ref FileContents, Offsets[9]);
           
            Header = new TASHeader();
            Header.Signature     = ReadHEX(ref FileContents, Offsets[0], 4);
            Header.Version       = Read32(ref FileContents, Offsets[1]);
            Header.UID           = ConvertUNIXTime(Read32(ref FileContents, Offsets[2]));
            Header.FrameCount    = Read32(ref FileContents, Offsets[4]) + 1;    // including the 0th frame
            Header.RerecordCount = Read32(ref FileContents, Offsets[3]);
                      
            Options = new TASOptions(true);
            Options.MovieStartFlag[0]  = (1 & (FileContents[Offsets[6]] >> 0)) == 0 ? true : false;
            Options.MovieStartFlag[1]  = (1 & (FileContents[Offsets[6]] >> 0)) == 1 ? true : false;
            Options.MovieTimingFlag[0] = (1 & (FileContents[Offsets[6]] >> 1)) == 0 ? true : false;
            Options.MovieTimingFlag[1] = (1 & (FileContents[Offsets[6]] >> 1)) == 1 ? true : false;

            SMVSpecific = new FormatSpecific(FileContents[Offsets[8]]);
            
            Extra = new TASExtra();            
            if (SMVSpecific.HASROMINFO)
            {
                Extra.ROM = ReadChars(ref FileContents, 0x07 + SaveStateOffset - EXTRAROMINFO_SIZE, 23);
                Extra.CRC = ReadHEXUnicode(ref FileContents, 0x03 + SaveStateOffset - EXTRAROMINFO_SIZE, 4);
            }

            int MetaDataOffset = Header.Version == 1 ? Offsets[11] : Offsets[16];
            int MetaDataLength = (SaveStateOffset - MetaDataOffset) - (SMVSpecific.HASROMINFO ? (int)EXTRAROMINFO_SIZE : 0);
            Extra.Author = ReadChars16(ref FileContents, MetaDataOffset, MetaDataLength);
           
            Input = new TASInput(5, false);
            for (int c = 0; c < 5; c++)
                Input.Controllers[c] = ((1 & (FileContents[Offsets[5]] >> c)) == 1) ? true : false;

            getFrameInput(ref FileContents);                                     
        }

        private void getFrameInput(ref byte[] byteArray)
        {
            Input.FrameData = new TASMovieInput[Header.FrameCount];            
            int controllers = Input.ControllerCount;

            // parse frame data
            for (int i = 0; i < Header.FrameCount; i++)
            {              
                Input.FrameData[i] = new TASMovieInput();
                // cycle through the controller data for the current frame
                for (int j = 0; j < controllers; j++)
                {                                            
                    byte[] frame = ReadBytes(ref byteArray,
                        ControllerDataOffset + ((i * controllers * BYTES_PER_FRAME) + (j * BYTES_PER_FRAME)),
                        BYTES_PER_FRAME);

                    Input.FrameData[i].Controller[j] = parseControllerData(frame);                                  
                }                               
            }
            
            // DEBUG::Not sure why all of a sudden this routine stopped working,
            // but this adds a blank frame to the end of the movie (if necessary)
            //if (Input.FrameData[Input.FrameData.Length - 1] == null)
            //    Input.FrameData[Input.FrameData.Length - 1] = new TASMovieInput();
        }

        /// <summary>
        /// Convert the binary representation of input to meaningful values 
        /// </summary>
        private string parseControllerData(byte[] byteArray)
        {
            string   input = "";
            
            // check the reset combo
            if ((byteArray[0] & byteArray[1]) == 0xFF)
            {
                return "!";
            }

            // check the first byte of input
            for (int i = 0; i < 8; i++)
            {
                if ((1 & (byteArray[1] >> i)) == 1)
                    input += InputValues[i];
            }
            // check the second byte of input
            for (int j = 0; j < 8; j++)
            {
                if ((1 & (byteArray[0] >> j)) == 1)
                    input += InputValues[j + 8];
            }
            return input;
        }

        /// <summary>
        /// Convert the string representation of input back to binary values
        /// </summary>
        private byte[] parseControllerData(string frameInput)
        {
            byte[] input = { 0x00, 0x00 };

            if (frameInput == null || frameInput == "") return input;
            if (frameInput.Contains("!"))
            {
                input[0] = 0xFF;
                input[1] = 0xFF;
                return input;
            }

            for (int i = 0; i < 8; i++)            
                if (frameInput.Contains(InputValues[i])) input[1] |= (byte)(1 << i);                                        
            for (int j = 0; j < 8; j++)
                if (frameInput.Contains(InputValues[j + 8])) input[0] |= (byte)(1 << j);                                        
            
            return input;
        }

        /// <summary>
        /// Save an SMV file back out to disk
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
            byte[] outputFile = new byte[head.Length + size];
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
                    }
                            
            updateMetadata(ref outputFile);
            
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

            WriteByteArrayToFile(ref outputFile, filename, input.Length - 1, Offsets[4]);  
        }

        /// <summary>
        /// Update the metadata information in the SMV
        /// </summary>        
        private void updateMetadata(ref byte[] byteArray)
        {
            int startPos = (Header.Version == 1) ? Offsets[11] : Offsets[16];
            int extraROMLength = (SMVSpecific.HASROMINFO) ? Convert.ToInt32(EXTRAROMINFO_SIZE) : 0;
            byte[] author = WriteChars16(Extra.Author);
            byte[] temp = new byte[startPos + author.Length + (byteArray.Length - SaveStateOffset + extraROMLength)];

            int newSaveStateOffset = startPos + author.Length + extraROMLength;
            int newCDataOffset = ControllerDataOffset + (newSaveStateOffset - SaveStateOffset);

            for (int i = 0; i < startPos; i++)
                temp[i] = byteArray[i];
            for (int j = 0; j < author.Length; j++)
                temp[j + startPos] = author[j];
            for (int k = 0; k < byteArray.Length - SaveStateOffset + extraROMLength; k++)
                temp[k + newSaveStateOffset - extraROMLength] = byteArray[k + SaveStateOffset - extraROMLength];

            Write32(ref temp, Offsets[9], newSaveStateOffset);
            Write32(ref temp, Offsets[10], newCDataOffset);            

            byteArray = temp;            
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
