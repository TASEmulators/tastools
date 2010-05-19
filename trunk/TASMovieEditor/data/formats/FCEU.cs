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
    /// <summary>
    /// Process an FCE Ultra movie file.
    /// 
    /// TODO::Control inputs aren't tracked, so a reset during input would be lost
    /// </summary>
    public class FCEU : TASMovie
    {        
        private int      ControllerDataLength;
        private string[] InputValues = { "A", "B", "s", "S", "^", "v", "<", ">" };
        private int[] Offsets = {
            0x00, // 4-byte signature: 46 43 4D 1A "FCM\x1A"
            0x04, // 4-byte little-endian unsigned int: version number, must be 2
            0x08, // 1-byte flags:
                  //    bit 0: reserved, set to 0
                  //    bit 1:
                  //        if "0", movie begins from an embedded "quicksave" snapshot
                  //        if "1", movie begins from reset 
                  //    bit 2:
                  //        if "0", NTSC timing
                  //        if "1", PAL timing           
                  //    other: reserved, set to 0
            0x09, // 1-byte flags: reserved, set to 0
            0x0A, // 1-byte flags: reserved, set to 0
            0x0B, // 1-byte flags: reserved, set to 0
            0x0C, // 4-byte little-endian unsigned int: number of frames
            0x10, // 4-byte little-endian unsigned int: rerecord count
            0x14, // 4-byte little-endian unsigned int: length of controller 
            0x18, // 4-byte little-endian unsigned int: offset to the savestate inside file
            0x1C, // 4-byte little-endian unsigned int: offset to the controller data inside file
            0x20, // 16-byte md5sum of the ROM used
            0x30, // 4-byte little-endian unsigned int: version of the emulator used
            0x34, // name of the ROM used - UTF8 encoded nul-terminated string.
        };

        /// <summary>
        /// Populate an FCEU object from the source movie
        /// </summary>        
        public FCEU(string FCMFile)
        {
            Filename = FCMFile;
            FillByteArrayFromFile(FCMFile, ref FileContents);

            byte options         = FileContents[Offsets[2]];
            SaveStateOffset      = Read32(ref FileContents, Offsets[9]);
            ControllerDataOffset = Read32(ref FileContents, Offsets[10]);
            ControllerDataLength = Read32(ref FileContents, Offsets[8]);
            
            Header.Signature     = ReadHEX(ref FileContents, Offsets[0], 4);
            Header.Version       = Read32(ref FileContents, Offsets[1]);
            Header.FrameCount    = Read32(ref FileContents, Offsets[6]);
            Header.RerecordCount = Read32(ref FileContents, Offsets[7]);
            Header.EmulatorID    = Read32(ref FileContents, Offsets[12]).ToString();            

            Extra = new TASExtra();
            Extra.CRC = ReadHEX(ref FileContents, Offsets[11], 16);
            Extra.ROM = ReadCharsNullTerminated(ref FileContents, Offsets[13]);

            int startPos = Offsets[13] + Extra.ROM.Length + 1;
            Extra.Author = ReadChars(ref FileContents, startPos, SaveStateOffset - startPos);           

            Options = new TASOptions(true);
            Options.MovieStartFlag[0]  = ((1 & options >> 1) == 0) ? true : false;
            Options.MovieStartFlag[1]  = ((1 & FileContents[ControllerDataOffset] >> 0) == 1) ? true : false;
            Options.MovieStartFlag[2]  = ((1 & FileContents[ControllerDataOffset] >> 1) == 1) ? true : false;
            Options.MovieTimingFlag[0] = ((1 & options >> 2) == 0) ? true : false;
            Options.MovieTimingFlag[1] = ((1 & options >> 2) == 1) ? true : false;

            Input = new TASInput(4, false);
            getFrameInput(ref FileContents);
        }

        /// <summary>
        /// Process the input values in the file to a TASMovieInput collection
        /// </summary>        
        private void getFrameInput(ref byte[] byteArray)
        {
            Input.FrameData = new TASMovieInput[Header.FrameCount];

            int   position   = ControllerDataOffset;
            int   frameCount = 0;
            int[] joop       = { 0, 0, 0, 0 };

            while (ControllerDataLength > 0)
            {
                int updateType = byteArray[position] >> 7;
                int NDelta     = (byteArray[position] >> 5) & 3;
                int delta      = 0;
                int data       = byteArray[position] & 0x1F;

                ++position; --ControllerDataLength;

                switch (NDelta)
                {
                    case 0:
                        break;
                    case 1:
                        delta |= byteArray[position++];
                        break;
                    case 2:
                        delta |= byteArray[position++];
                        delta |= byteArray[position++] << 8;
                        break;
                    case 3:
                        delta |= byteArray[position++];
                        delta |= byteArray[position++] << 8;
                        delta |= byteArray[position++] << 16;
                        break;
                }

                // populate parsed data once (since it doesn't change until delta == 0,
                // there's no need to process each time the loop iterates)
                TASMovieInput parsedControllerData = parseControllerData(joop);

                while (delta > 0)
                {
                    // Save the controlled data                                                                                                                                  
                    Input.FrameData[frameCount] = new TASMovieInput();
                    Input.FrameData[frameCount] = parsedControllerData;
                    ++frameCount; --delta;
                }

                if (ControllerDataLength > NDelta)
                    ControllerDataLength -= NDelta;
                else
                    ControllerDataLength = 0;

                if (updateType == 0) // Controller data
                {
                    int ctrlno = (data >> 3);
                    joop[ctrlno] ^= (1 << (data & 7));
                    if (ctrlno == 0) { Input.Controllers[0] = true; }
                    if (ctrlno == 1) { Input.Controllers[1] = true; }
                    if (ctrlno == 2) { Input.Controllers[2] = true; }
                    if (ctrlno == 3) { Input.Controllers[3] = true; }
                }
                // Exerpt from Nesmock 1.6.0 source.
                //else // Control data
                //    switch (data)
                //    {
                //        case 0: break; // nothing
                //        case 1: Save = false; break; // reset
                //        case 2: Save = false; break; // power cycle
                //        case 7: break; // VS coin
                //        case 8: break; // VS dip0
                //        case 24: FDS = true; Cdata[frame].FDS |= 1; break; /* FDS insert, FIXME */
                //        case 25: FDS = true; Cdata[frame].FDS |= 2; break; /* FDS eject, FIXME */
                //        case 26: FDS = true; Cdata[frame].FDS |= 4; break; /* FDS swap, FIXME */
                //    }

            }
        }
        /// <summary>
        /// Convert frame input from binary values to their string representation
        /// </summary>
        private TASMovieInput parseControllerData(int[] joop)
        {
            TASMovieInput frameData = new TASMovieInput();

            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 8; j++)
                    if ((1 & (joop[i] >> j)) == 1) frameData.Controller[i] += InputValues[j];

            return frameData;
        }

        /// <summary>
        /// Convert frame input from string values to their binary representation
        /// </summary>
        private byte parseControllerData(string frameValue)
        {
            byte joop = 0x00;

            if (frameValue != null && frameValue != "")
                for (int i = 0; i < 8; i++)
                    if (frameValue.Contains(InputValues[i])) joop |= (byte)(1 << i);

            return joop;
        }

        /// <summary>
        /// Save changes to the currently buffered FCM file        
        /// </summary>        
        public override void Save(string filename, ref TASMovieInput[] input)
        {
            byte[] head        = ReadBytes(ref FileContents, 0, ControllerDataOffset);
            int    cdataLength = getRLELength(ref input);
            int    controllers = Input.ControllerCount;

            int   buffer   = 0;
            int[] joop     = { 0, 0, 0, 0 };
            int   position = ControllerDataOffset;

            byte[] outputFile = new byte[head.Length + cdataLength];
            head.CopyTo(outputFile, 0);

            // start from either power-on or reset
            if (Options.MovieStartFlag[2])
                outputFile[position++] = (0x82 & 0x9F) | (0 << 5);
            else // if (Options.MovieStartFlag[1])
                outputFile[position++] = (0x81 & 0x9F) | (0 << 5); // FIXME

            int frame = 0;
            while (frame < input.Length)
            {
                //// cycle through the controllers
                for (int j = 0; j < controllers; j++)
                {
                    int current = parseControllerData(input[frame].Controller[j]);
                    if (current != joop[j])
                    {
                        // cycle through each bit to see if it's changed
                        for (int k = 0; k < 8; k++)
                        {
                            // if there is a difference, write it out as a command
                            if (((current ^ joop[j]) & (1 << k)) > 0)
                                doEncode(j, k, ref buffer, ref outputFile, ref position);
                        }
                    }
                    joop[j] = current;
                }
                buffer++; frame++;
            }                                    

            doEncode(0, 0x80, ref buffer, ref outputFile, ref position);
            //outputFile[position++] = ((0x80) & 0x9F) | (0 << 5); // null command

            // write the new controllerDataLength            
            Write32(ref outputFile, Offsets[8], cdataLength);
            
            updateMetaData(ref outputFile);
            WriteByteArrayToFile(ref outputFile, filename, input.Length, Offsets[6]);  
        }

        /// <summary>
        /// Update the movie's author info and shift the controllerdata/savestate offsets
        /// accordingly
        /// </summary>        
        private void updateMetaData(ref byte[] byteArray)
        {
            int    startPos = Offsets[13] + Extra.ROM.Length + 1;
            byte[] author   = WriteChars(Extra.Author + "\0");

            while (!((startPos + author.Length) % 4 == 0))
            {
                byte[] padded = new byte[author.Length + 1];
                author.CopyTo(padded, 0);
                padded[padded.Length - 1] = 0;
                author = padded;
            }
            byte[] temp = new byte[startPos + author.Length + byteArray.Length - SaveStateOffset];
            int    newSaveStateOffset = startPos + author.Length;
            int    newCDataOffset     = ControllerDataOffset + (newSaveStateOffset - SaveStateOffset);

            for (int i = 0; i < startPos; i++)
                temp[i] = byteArray[i];
            for (int j = 0; j < author.Length; j++)
                temp[j + startPos] = author[j];
            for (int k = 0; k < byteArray.Length - SaveStateOffset; k++)
                temp[k + newSaveStateOffset] = byteArray[k + SaveStateOffset];

            Write32(ref temp, Offsets[10], newCDataOffset);
            Write32(ref temp, Offsets[9], newSaveStateOffset);

            byteArray = temp;
        }

        /// <summary>
        /// Encode FCM controller data
        /// 
        /// NOTE::This is almost verbatum from the FCEU source
        /// </summary>
        private void doEncode(int joy, int button, ref int buffer, ref byte[] CompressedData, ref int position)
        {
            int d = 0;

            if (buffer >= ((1 << 24) - 1)) // if the buffer's gonna overflow, write a dummy command
                d = 0x80;
            else if (buffer >= 65536)
                d = 3 << 5;
            else if (buffer >= 256)
                d = 2 << 5;
            else if (buffer > 0)
                d = 1 << 5;

            d |= joy << 3; d |= button;

            CompressedData[position++] = (byte)d;            
            while (buffer > 0)
            {
                CompressedData[position++] = (byte)(buffer & 0xff);
                buffer >>= 8;
            }
        }

        /// <summary>
        /// Get the RLE length of the input so we can size the output array accordingly
        /// 
        /// HACK::this whole routine is a bad idea, but i need to know the final length of the array,
        /// and dynamic allocation just doesn't seem to want to work :P
        /// </summary>   
        public int getRLELength(ref TASMovieInput[] input)
        {
            int buffer = 0;
            int[] joop = { 0, 0, 0, 0 };     
            int length = 1;
            for (int i = 0; i < input.Length; i++)
            {                
                // cycle through the controllers
                for (int j = 0; j < 4; j++)
                {                    
                    int current = parseControllerData(input[i].Controller[j]);
                    if (current != joop[j])
                    {
                        // cycle through each bit to see if it's changed
                        for (int k = 0; k < 8; k++)
                        {
                            // if there is a difference, write it out as a command
                            if (((current ^ joop[j]) & (1 << k)) > 0)                                
                            {
                                length++;
                                while (buffer > 0)
                                {
                                    length++;
                                    buffer >>= 8;
                                }
                            }
                        }
                    }
                    joop[j] = current;
                }
                buffer++;
            }
            // handle additional buffered values that may have escaped the initial loop
            // TODO::Functional, but inefficent
            length++;
            while (buffer > 0)
            {
                length++;
                buffer >>= 8;
            }
            return length;
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
