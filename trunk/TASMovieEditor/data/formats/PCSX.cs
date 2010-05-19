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
    public class PCSX : TASMovie
    {
        private static int[] BYTES_PER_FRAME = { 1, 4, 0, 0, 2, 6, 0, 6 };
        
        /// <summary>
        /// Contains Format Specific items
        /// </summary>
        public struct FormatSpecific
        {
            public byte[] ControllerType;
            public int[]  ControllerSize;
            public bool HasEmbeddedMemoryCards;
            public bool HasEmbeddedCheatList;
            public bool UsesHacks;
            public int EmulatorVersion;
            public int MemoryCard1Offset;
            public int MemoryCard2Offset;
            public int CheatListOffset;
            public int CDRomIDOffset;
            public int CDRomCount;

            public FormatSpecific(byte ControllerPort1Type, byte ControllerPort2Type)
            {
                HasEmbeddedMemoryCards = false;
                HasEmbeddedCheatList   = false;
                UsesHacks              = false;
                EmulatorVersion   = 0;
                MemoryCard1Offset = 0;
                MemoryCard2Offset = 0;
                CheatListOffset   = 0;
                CDRomIDOffset     = 0;
                CDRomCount        = 0;

                ControllerType = new byte[2];
                ControllerSize = new int[2];
                ControllerType[0] = ControllerPort1Type;
                ControllerType[1] = ControllerPort2Type;
                ControllerSize[0] = BYTES_PER_FRAME[ControllerPort1Type];
                ControllerSize[1] = BYTES_PER_FRAME[ControllerPort2Type];
            }

            public string GetControllerTypeString(int id)
            {
                string type = "Unknown";

                switch (ControllerType[id])
                {
                    case 1:
                        type = "Mouse";
                        break;
                    case 4:
                        type = "Standard";
                        break;
                    case 5:
                        type = "Analog Joystick";
                        break;
                    case 7:
                        type = "Analog Controller";
                        break;
                    default:
                        break;
                }
                return type;
            }
        } 

        public FormatSpecific PXMSpecific;

        private string[] InputValues = { "Se", "_1", "_2", "St", "^", ">", "v", "<", "L2", "R2", "L1", "R1", "Tr", "Ci", "Cr", "Sq" };
        private string[] ControlValues = { "_0", "!", "CDcase", "SIOhack", "SPUhack", "Cheats", "REhack", "PEhack" };
        private string[] AnalogValues = { "X", "Y", "Xl", "Yl", "Xr", "Yr", "(", ")", "[", "]", "{", "}", ":" };
        private int[] Offsets = {
            0x00, // 4-byte signature: "PXM "
            0x04, // 4-byte unsigned long: version number (for now it is always 2)
            0x08, // 4-byte unsigned long: version of the emulator used
            0x0C, // 1-byte flags:
                  //    bit 0: reserved, set to 0
                  //    bit 1:
                  //    if "0", movie begins from power-on
                  //    if "1", movie begins from an embedded savestate
                  //    bit 2:
                  //    if "0", NTSC timing
                  //    if "1", PAL timing
                  //    bit 3:
                  //    if "0", movie does not contain embedded memory cards
                  //    if "1", movie does contain embedded memory cards
                  //    bit 4:
                  //    if "0", movie does not contain embedded cheat list
                  //    if "1", movie does contain embedded cheat list
                  //    bit 5:
                  //    if "0", movie does not use hacks
                  //    if "1", movie does use hacks such as "SPU/SIO IRQ always enabled"
                  //    other: reserved, set to 0
            0x0D, // 1-byte flags: reserved, set to 0
            0x0E, // 1-byte unsigned char: controller port 1 type
            0x0F, // 1-byte unsigned char: controller port 2 type
            0x10, // 4-byte unsigned long: number of frames
            0x14, // 4-byte unsigned long: rerecord count
            0x18, // 4-byte unsigned long: offset to the savestate inside file
            0x1C, // 4-byte unsigned long: offset to the memory card 1 inside file
            0x20, // 4-byte unsigned long: offset to the memory card 2 inside file
            0x24, // 4-byte unsigned long: offset to the cheat list inside file
            0x28, // 4-byte unsigned long: offset to the CD-ROM IDs inside file
            0x2C, // 4-byte unsigned long: offset to the controller data inside file
            0x30, // 4-byte unsigned long: string length of author's name
            0x34  // string: name of the author
        };

        public PCSX(string PXMFile)
        {
            Filename = PXMFile;
            FillByteArrayFromFile(PXMFile, ref FileContents);

            ControllerDataOffset = Read32(ref FileContents, Offsets[14]);
            SaveStateOffset      = Read32(ref FileContents, Offsets[9]);
           
            Header = new TASHeader();
            Header.Signature     = ReadHEX(ref FileContents, Offsets[0], 4);
            Header.Version       = Read32(ref FileContents, Offsets[1]);
            Header.FrameCount    = Read32(ref FileContents, Offsets[7]);
            Header.RerecordCount = Read32(ref FileContents, Offsets[8]);
                      
            Options = new TASOptions(true);
            Options.MovieStartFlag[0]  = (1 & (FileContents[Offsets[3]] >> 1)) == 1 ? true : false;
            Options.MovieStartFlag[1]  = (1 & (FileContents[Offsets[3]] >> 1)) == 0 ? true : false;
            Options.MovieTimingFlag[0] = (1 & (FileContents[Offsets[3]] >> 2)) == 0 ? true : false;
            Options.MovieTimingFlag[1] = (1 & (FileContents[Offsets[3]] >> 2)) == 1 ? true : false;

            PXMSpecific = new FormatSpecific(FileContents[Offsets[5]], FileContents[Offsets[6]]);
            PXMSpecific.HasEmbeddedMemoryCards = (1 & (FileContents[Offsets[3]] >> 3)) == 1 ? true : false;
            PXMSpecific.HasEmbeddedCheatList   = (1 & (FileContents[Offsets[3]] >> 4)) == 1 ? true : false;
            PXMSpecific.UsesHacks              = (1 & (FileContents[Offsets[3]] >> 5)) == 1 ? true : false;
            PXMSpecific.EmulatorVersion   = Read32(ref FileContents, Offsets[2]);
            PXMSpecific.MemoryCard1Offset = Read32(ref FileContents, Offsets[10]);
            PXMSpecific.MemoryCard2Offset = Read32(ref FileContents, Offsets[11]);
            PXMSpecific.CheatListOffset   = Read32(ref FileContents, Offsets[12]);
            PXMSpecific.CDRomIDOffset     = Read32(ref FileContents, Offsets[13]);
            
            PXMSpecific.CDRomCount       = FileContents[PXMSpecific.CDRomIDOffset];

            Extra = new TASExtra();
            Extra.ROM = ReadChars(ref FileContents, PXMSpecific.CDRomIDOffset + 1, PXMSpecific.CDRomCount * 9);
            Extra.Author = ReadChars(ref FileContents, Offsets[16], Read32(ref FileContents, Offsets[15]));
           
            Input = new TASInput(3, true);

            getFrameInput(ref FileContents);                                     
        }

        private void getFrameInput(ref byte[] byteArray)
        {
            int Controller1Size = PXMSpecific.ControllerSize[0];
            int Controller2Size = PXMSpecific.ControllerSize[1];
            int Controller1Type = PXMSpecific.ControllerType[0];
            int Controller2Type = PXMSpecific.ControllerType[1];
            int ControlSize = BYTES_PER_FRAME[0];

            Input.FrameData = new TASMovieInput[Header.FrameCount];

            for (int i = 0, pos = ControllerDataOffset; i < Header.FrameCount; ++i)
            {
                Input.FrameData[i] = new TASMovieInput();

                // there are always 2 controllers to deal with, so no need to 
                // run the array through a loop to be populated
                Input.FrameData[i].Controller[0] = parseControllerData(byteArray, pos, Controller1Type);
                pos += Controller1Size;
                Input.FrameData[i].Controller[1] = parseControllerData(byteArray, pos, Controller2Type);
                pos += Controller2Size;
                Input.FrameData[i].Controller[2] = parseControlData(byteArray, pos);
                pos += ControlSize;
            }
        }

        /// <summary>
        /// Convert the binary representation of input to meaningful values 
        /// </summary>
        private string parseControllerData(byte[] byteArray, int pos, int controllerType)
        {
            string input = "";

            switch (controllerType)
            {
                case 1:
                    // mouse
                    input += parseStandardControllerData(byteArray, pos);
                    input += parseAnalogData(byteArray, pos, 0);
                    break;
                case 4:
                    // standard
                    input += parseStandardControllerData(byteArray, pos);
                    break;
                case 5:
                case 7:
                    // analog
                    input += parseStandardControllerData(byteArray, pos);
                    input += parseAnalogData(byteArray, pos, 1);
                    input += parseAnalogData(byteArray, pos, 2);
                    break;
                default:
                    // not supported yet
                    break;
            }

            return input;
        }

        /// <summary>
        /// Convert the binary representation of input to meaningful values 
        /// </summary>
        private byte[] parseControllerData(string frameInput, int controllerType)
        {
            byte[] input = { };

            switch (controllerType)
            {
                case 1:
                {
                    // mouse
                    byte[] stdPad = parseStandardControllerData(frameInput);
                    byte[] axes = parseAnalogData(frameInput, 0);
                    input = new byte[stdPad.Length + axes.Length];
                    stdPad.CopyTo(input, 0);
                    axes.CopyTo(input, stdPad.Length);
                    break;
                }
                case 4:
                {
                    // standard
                    byte[] stdPad = parseStandardControllerData(frameInput);
                    input = new byte[stdPad.Length];
                    stdPad.CopyTo(input, 0);
                    break;
                }
                case 5:
                case 7:
                {
                    // analog
                    byte[] stdPad = parseStandardControllerData(frameInput);
                    byte[] axesLeft = parseAnalogData(frameInput, 1);
                    byte[] axesRight = parseAnalogData(frameInput, 2);
                    input = new byte[stdPad.Length + axesLeft.Length + axesRight.Length];
                    stdPad.CopyTo(input, 0);
                    axesLeft.CopyTo(input, stdPad.Length);
                    axesRight.CopyTo(input, stdPad.Length + axesLeft.Length);
                    break;
                }
                default:
                    // not supported yet
                    break;
            }

            return input;
        }

        /// <summary>
        /// Convert the binary representation of input to meaningful values 
        /// </summary>
        private string parseStandardControllerData(byte[] byteArray, int pos)
        {
            string input = "";
            
            // check the first byte of input
            for (int i = 0; i < 8; i++)
            {
                if ((1 & (byteArray[pos] >> i)) == 1)
                    input += InputValues[i];
            }
            // check the second byte of input
            for (int j = 0; j < 8; j++)
            {
                if ((1 & (byteArray[pos + 1] >> j)) == 1)
                    input += InputValues[j + 8];
            }
    
            return input;
        }

        /// <summary>
        /// Convert the string representation of input back to binary values
        /// </summary>
        private byte[] parseStandardControllerData(string frameInput)
        {
            byte[] input = { 0x00, 0x00 };

            if (frameInput == null || frameInput == "") return input;
            
            for (int i = 0; i < 8; i++)            
                if (frameInput.Contains(InputValues[i])) input[0] |= (byte)(1 << i);  

            for (int j = 0; j < 8; j++)
                if (frameInput.Contains(InputValues[j + 8])) input[1] |= (byte)(1 << j);                                        
            
            return input;
        }

        /// <summary>
        /// Convert the binary representation of input to meaningful values
        /// labelType: 0 = mouse, 1 = left analog stick, 2 = right analog stick
        /// </summary>
        private string parseAnalogData(byte[] byteArray, int pos, int labelType)
        {
            string input = "";

            for (int i = pos; i < pos + 2; i++)
            {
                if (byteArray[i] != 0x80)
                    input += AnalogValues[2] + AnalogValues[i + labelType * 2] + AnalogValues[12] + byteArray[i] + AnalogValues[7];
            }

            return input;
        }

        /// <summary>
        /// Convert the string representation of input back to binary values
        /// </summary>
        private byte[] parseAnalogData(string frameInput, int controllerType)
        {
            byte[] input = { 0x80, 0x80 };

            if (frameInput == null || frameInput == "") return input;

            for (int i = 0; i < 2; i++)
            {
                int start = frameInput.IndexOf(AnalogValues[controllerType]);
                if (start != -1)
                {
                    int nStart = frameInput.IndexOf(AnalogValues[12], start) + AnalogValues[12].Length;
                    int nEnd = frameInput.IndexOf(AnalogValues[7], nStart);
                    string num = frameInput.Substring(nStart, nEnd - nStart);
                    input[i] = Convert.ToByte(num);
                }
            }

            return input;
        }


        /// <summary>
        /// Convert the binary representation of input to meaningful values 
        /// </summary>
        private string parseControlData(byte[] byteArray, int pos)
        {
            string input = "";

            for (int i = 0; i < 8; i++)
            {
                if ((1 & (byteArray[pos] >> i)) == 1)
                    input += ControlValues[i];
            }

            return input;
        }

        /// <summary>
        /// Convert the string representation of input back to binary values
        /// </summary>
        private byte[] parseControlData(string frameInput)
        {
            byte[] input = { 0x00 };

            if (frameInput == null || frameInput == "") return input;

            for (int i = 0; i < 8; i++)
                if (frameInput.Contains(ControlValues[i])) input[0] |= (byte)(1 << i);

            return input;
        }

        /// <summary>
        /// Save an PXM file back out to disk
        /// </summary>
        public override void Save(string filename, ref TASMovieInput[] input)
        {
            int Controller1Size = PXMSpecific.ControllerSize[0];
            int Controller2Size = PXMSpecific.ControllerSize[1];
            int Controller1Type = PXMSpecific.ControllerType[0];
            int Controller2Type = PXMSpecific.ControllerType[1];
            int ControlSize     = BYTES_PER_FRAME[0];

            int oldAuthorNameLength = Read32(ref FileContents, Offsets[15]);
            byte[] author = WriteChars(Extra.Author);
            int deltaAuthorNameLength = author.Length - oldAuthorNameLength;

            // head excludes the author info but includes its length
            byte[] head = ReadBytes(ref FileContents, 0, Offsets[16]);

            // assuming that controller data are always at the end
            byte[] extra = ReadBytes(ref FileContents, Offsets[16] + oldAuthorNameLength, ControllerDataOffset - Offsets[16] - oldAuthorNameLength);

            // get the size of the file byte[] (minus the header & extra data)
            int size = input.Length * (Controller1Size + Controller2Size + ControlSize);

            // create the output array and copy in the contents
            byte[] outputFile = new byte[head.Length + author.Length + extra.Length + size];
            head.CopyTo(outputFile, 0);

            int newSaveStateOffset      = SaveStateOffset               + deltaAuthorNameLength;
            int newMemoryCard1Offset    = PXMSpecific.MemoryCard1Offset + deltaAuthorNameLength;
            int newMemoryCard2Offset    = PXMSpecific.MemoryCard2Offset + deltaAuthorNameLength;
            int newCheatListOffset      = PXMSpecific.CheatListOffset   + deltaAuthorNameLength;
            int newCDRomIDOffset        = PXMSpecific.CDRomIDOffset     + deltaAuthorNameLength;
            int newControllerDataOffset = ControllerDataOffset          + deltaAuthorNameLength;

            Write32(ref outputFile, Offsets[7],  input.Length);

            Write32(ref outputFile, Offsets[9],  newSaveStateOffset);
            Write32(ref outputFile, Offsets[10], newMemoryCard1Offset);
            Write32(ref outputFile, Offsets[11], newMemoryCard2Offset);
            Write32(ref outputFile, Offsets[12], newCheatListOffset);
            Write32(ref outputFile, Offsets[13], newCDRomIDOffset);
            Write32(ref outputFile, Offsets[14], newControllerDataOffset);

            Write32(ref outputFile, Offsets[15], author.Length);
            for (int j = 0; j < author.Length; ++j)
                outputFile[Offsets[16] + j] = Convert.ToByte(author[j]);

            extra.CopyTo(outputFile, head.Length + author.Length);

            // add the controller data
            int position = newControllerDataOffset;
            for (int i = 0; i < input.Length; i++)           
            {
                parseControllerData(input[i].Controller[0], Controller1Type).CopyTo(outputFile, position);
                position += Controller1Size;
                parseControllerData(input[i].Controller[1], Controller2Type).CopyTo(outputFile, position);
                position += Controller2Size;
                parseControlData(input[i].Controller[2]).CopyTo(outputFile, position);
                position += ControlSize;
            }
                    
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

            WriteByteArrayToFile(ref outputFile, filename, input.Length, Offsets[7]);  
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
