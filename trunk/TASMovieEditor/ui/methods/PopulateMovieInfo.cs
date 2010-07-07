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
using System.Windows.Forms;

using MovieSplicer.Data;
using MovieSplicer.Data.Formats;

namespace MovieSplicer.UI.Methods
{
    /// <summary>
    /// Populate a TreeView control with parsed data from a movie object
    /// </summary>
    class PopulateMovieInfo
    {       
        /// <summary>
        /// Populate an SNES9x movie file's header information
        /// </summary>        
        public static void SMV(ref TreeView tv, ref TASMovie smv)
        {
            SNES9x movie = (SNES9x)smv;
            tv.Nodes.Clear();            

            tv.Nodes.Add("Header");            
            tv.Nodes[0].Nodes.Add("Signature:      " + movie.Header.Signature);
            tv.Nodes[0].Nodes.Add("Version:        " + movie.Header.Version.ToString());
            tv.Nodes[0].Nodes.Add("UID:            " + movie.Header.UID);
            tv.Nodes[0].Nodes.Add("Frame Count:    " + String.Format("{0:0,0}", movie.Header.FrameCount));
            tv.Nodes[0].Nodes.Add("Rerecord Count: " + String.Format("{0:0,0}", movie.Header.RerecordCount));

            tv.Nodes.Add("Options");
            tv.Nodes[1].Nodes.Add("Movie Start:  " + movie.Options.MovieStart);
            tv.Nodes[1].Nodes.Add("Movie Timing: " + movie.Options.MovieTiming);

            tv.Nodes[1].Nodes.Add("Sync Options");
            tv.Nodes[1].Nodes[2].Nodes.Add("FAKEMUTE:   " + movie.SMVSpecific.FAKEMUTE.ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("LEFTRIGHT:  " + movie.SMVSpecific.LEFTRIGHT.ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("SYNCSOUND:  " + movie.SMVSpecific.SYNCSOUND.ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("VOLUMEENVX: " + movie.SMVSpecific.VOLUMEENVX.ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("WIP1TIMING: " + movie.SMVSpecific.WIP1TIMING.ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("SPEEDHACKS: " + (!movie.SMVSpecific.NOCPUSHUTDOWN).ToString());

            tv.Nodes.Add("Metadata");
            tv.Nodes[2].Nodes.Add("Author: " + movie.Extra.Author);

            tv.Nodes.Add("ROM Information");
            tv.Nodes[3].Nodes.Add("ROM Title: " + movie.Extra.ROM);
            tv.Nodes[3].Nodes.Add("ROM CRC:   " + movie.Extra.CRC);

            tv.Nodes.Add("Controllers");
            tv.Nodes[4].Nodes.Add("Controller 1 Present: " + movie.Input.Controllers[0].ToString());
            tv.Nodes[4].Nodes.Add("Controller 2 Present: " + movie.Input.Controllers[1].ToString());
            tv.Nodes[4].Nodes.Add("Controller 3 Present: " + movie.Input.Controllers[2].ToString());
            tv.Nodes[4].Nodes.Add("Controller 4 Present: " + movie.Input.Controllers[3].ToString());
            tv.Nodes[4].Nodes.Add("Controller 5 Present: " + movie.Input.Controllers[4].ToString());
            
            movie = null; tv.ExpandAll(); tv.Nodes[0].EnsureVisible();
        }        

        /// <summary>
        /// Populate an FCE Ultra movie file's header information
        /// </summary>
        public static void FCM(ref TreeView tv, ref TASMovie fcm)
        {
            FCEU movie = (FCEU)fcm;
            tv.Nodes.Clear();

            tv.Nodes.Add("Header");
            tv.Nodes[0].Nodes.Add("Signature:        " + movie.Header.Signature);
            tv.Nodes[0].Nodes.Add("Version:          " + movie.Header.Version.ToString());
            tv.Nodes[0].Nodes.Add("Frame Count:      " + String.Format("{0:0,0}", movie.Header.FrameCount));
            tv.Nodes[0].Nodes.Add("Rerecord Count:   " + String.Format("{0:0,0}", movie.Header.RerecordCount));
            tv.Nodes[0].Nodes.Add("Emulator Version: " + movie.Header.EmulatorID);
            tv.Nodes[0].Nodes.Add("Movie Start:      " + movie.Options.MovieStart);
            tv.Nodes[0].Nodes.Add("Movie Timing:     " + movie.Options.MovieTiming);

            tv.Nodes.Add("Metadata");
            tv.Nodes[1].Nodes.Add("Author: " + movie.Extra.Author);

            tv.Nodes.Add("ROM Information");
            tv.Nodes[2].Nodes.Add("ROM Title: " + movie.Extra.ROM);
            tv.Nodes[2].Nodes.Add("ROM CRC:   " + movie.Extra.CRC);

            tv.Nodes.Add("Controllers");
            tv.Nodes[3].Nodes.Add("Controller 1: " + movie.Input.Controllers[0].ToString());
            tv.Nodes[3].Nodes.Add("Controller 2: " + movie.Input.Controllers[1].ToString());
            tv.Nodes[3].Nodes.Add("Controller 3: " + movie.Input.Controllers[2].ToString());
            tv.Nodes[3].Nodes.Add("Controller 4: " + movie.Input.Controllers[3].ToString());

            movie = null; tv.ExpandAll(); tv.Nodes[0].EnsureVisible();
        }
       
        /// <summary>
        /// Populate a VisualBoyAdvance movie file's header information
        /// </summary>
        public static void VBM(ref TreeView tv, ref TASMovie vbm)
        {
            VisualBoyAdvance movie = (VisualBoyAdvance)vbm;
            tv.Nodes.Clear();

            tv.Nodes.Add("Header");
            tv.Nodes[0].Nodes.Add("Signature:      " + movie.Header.Signature);
            tv.Nodes[0].Nodes.Add("Version:        " + movie.Header.Version.ToString());
            tv.Nodes[0].Nodes.Add("UID:            " + movie.Header.UID);
            tv.Nodes[0].Nodes.Add("Frame Count:    " + String.Format("{0:0,0}", movie.Header.FrameCount));
            tv.Nodes[0].Nodes.Add("Rerecord Count: " + String.Format("{0:0,0}", movie.Header.RerecordCount));

            tv.Nodes.Add("Options");
            tv.Nodes[1].Nodes.Add("Movie Start: " + movie.Options.MovieStart);
            tv.Nodes[1].Nodes.Add("System Type");
            tv.Nodes[1].Nodes[1].Nodes.Add("Game Boy Advance: " + movie.VBMSpecific.SystemType[0].ToString());
            tv.Nodes[1].Nodes[1].Nodes.Add("Game Boy Colour:  " + movie.VBMSpecific.SystemType[1].ToString());
            tv.Nodes[1].Nodes[1].Nodes.Add("Super Game Boy:   " + movie.VBMSpecific.SystemType[2].ToString());
            tv.Nodes[1].Nodes[1].Nodes.Add("Game Boy:         " + movie.VBMSpecific.SystemType[3].ToString());
            tv.Nodes[1].Nodes.Add("BIOS Flags");
            tv.Nodes[1].Nodes[2].Nodes.Add("useBiosFile:  " + movie.VBMSpecific.BIOSFlags[0].ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("skipBiosFile: " + movie.VBMSpecific.BIOSFlags[1].ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("rtcEnable:    " + movie.VBMSpecific.BIOSFlags[2].ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("lagReduction: " + movie.VBMSpecific.BIOSFlags[4].ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("gbcHdma5Fix:  " + movie.VBMSpecific.BIOSFlags[5].ToString());

            tv.Nodes.Add("Metadata");
            tv.Nodes[2].Nodes.Add("Author:      " + movie.Extra.Author);
            tv.Nodes[2].Nodes.Add("Description: " + movie.Extra.Description);

            tv.Nodes.Add("ROM Information");
            tv.Nodes[3].Nodes.Add("ROM Name: " + movie.Extra.ROM);
            tv.Nodes[3].Nodes.Add("ROM CRC:  " + movie.Extra.CRC);
            //tv.Nodes[3].Nodes.Add("Check:    " + movie.RomInfo.Checksum.ToString());

            tv.Nodes.Add("Controllers");
            tv.Nodes[4].Nodes.Add("Controller 1 Present: " + movie.Input.Controllers[0].ToString());
            tv.Nodes[4].Nodes.Add("Controller 2 Present: " + movie.Input.Controllers[1].ToString());
            tv.Nodes[4].Nodes.Add("Controller 3 Present: " + movie.Input.Controllers[2].ToString());
            tv.Nodes[4].Nodes.Add("Controller 4 Present: " + movie.Input.Controllers[3].ToString());

            movie = null; tv.ExpandAll(); tv.Nodes[0].EnsureVisible();
        }       

        /// <summary>
        /// Populate a Famtasia movie file's header information
        /// </summary>
        public static void FMV(ref TreeView tv, ref TASMovie fmv)
        {
            Famtasia movie = (Famtasia)fmv;
            tv.Nodes.Clear();

            tv.Nodes.Add("Header");
            tv.Nodes[0].Nodes.Add("Signature:      " + movie.Header.Signature);
            tv.Nodes[0].Nodes.Add("Movie Title:    " + movie.Extra.Description);
            tv.Nodes[0].Nodes.Add("Frame Count:    " + String.Format("{0:0,0}", movie.Header.FrameCount));
            tv.Nodes[0].Nodes.Add("Rerecord Count: " + String.Format("{0:0,0}", movie.Header.RerecordCount));
            tv.Nodes[0].Nodes.Add("Emulator ID:    " + movie.Header.EmulatorID);
            tv.Nodes[0].Nodes.Add("Movie Start:    " + movie.Options.MovieStart);

            tv.Nodes.Add("Controllers");
            tv.Nodes[1].Nodes.Add("Controller 1: " + movie.Input.Controllers[0].ToString());
            tv.Nodes[1].Nodes.Add("Controller 2: " + movie.Input.Controllers[1].ToString());

            movie = null; tv.ExpandAll(); tv.Nodes[0].EnsureVisible();            
        }        

        /// <summary>
        /// Populate a Gens movie file's header information
        /// </summary>
        public static void GMV(ref TreeView tv, ref TASMovie gmv)
        {
            Gens movie = (Gens)gmv;
            tv.Nodes.Clear();

            tv.Nodes.Add("Header");
            tv.Nodes[0].Nodes.Add("Signature:      " + movie.Header.Signature);
            tv.Nodes[0].Nodes.Add("Version:        " + movie.Header.Version.ToString());
            tv.Nodes[0].Nodes.Add("Movie Name:     " + movie.Extra.Description);
            tv.Nodes[0].Nodes.Add("Frame Count:    " + String.Format("{0:0,0}", movie.Header.FrameCount));
            tv.Nodes[0].Nodes.Add("Rerecord Count: " + String.Format("{0:0,0}", movie.Header.RerecordCount));

            if (movie.Header.Version > 0x09)
            {
                tv.Nodes[0].Nodes.Add("FPS:            " + movie.Options.FPS);
                tv.Nodes[0].Nodes.Add("Movie Start:    " + movie.Options.MovieStart);
            }

            tv.Nodes.Add("Controllers");
            tv.Nodes[1].Nodes.Add("Controller 1: true");
            tv.Nodes[1].Nodes[0].Nodes.Add("Config: " + movie.GMVSpecific.Player1Config + " button");
            tv.Nodes[1].Nodes.Add("Controller 2: true");
            tv.Nodes[1].Nodes[1].Nodes.Add("Config: " + movie.GMVSpecific.Player2Config + " button");
            tv.Nodes[1].Nodes.Add("Controller 3: false");

            if (movie.Header.Version > 0x09)
                if (movie.Input.ControllerCount == 3)
                    tv.Nodes[1].Nodes[2].Text = "Controller 3: true";

            movie = null; tv.ExpandAll(); tv.Nodes[0].EnsureVisible();
        }

        /// <summary>
        /// Populate a Mupen64 movie file's header information
        /// </summary>
        public static void M64(ref TreeView tv, ref TASMovie m64)
        {
            Mupen64 movie = (Mupen64)m64;
            tv.Nodes.Clear();

            tv.Nodes.Add("Header");
            tv.Nodes[0].Nodes.Add("Signature:      " + movie.Header.Signature);
            tv.Nodes[0].Nodes.Add("Version:        " + movie.Header.Version.ToString());
            tv.Nodes[0].Nodes.Add("UID:            " + movie.Header.UID);
            tv.Nodes[0].Nodes.Add("Frame Count:    " + String.Format("{0:0,0}", movie.Header.FrameCount));
            tv.Nodes[0].Nodes.Add("Rerecord Count: " + String.Format("{0:0,0}", movie.Header.RerecordCount));

            tv.Nodes.Add("Options");
            tv.Nodes[1].Nodes.Add("FPS:         " + movie.Options.FPS);
            tv.Nodes[1].Nodes.Add("Movie Start: " + movie.Options.MovieStart);

            tv.Nodes.Add("ROM Information");
            tv.Nodes[2].Nodes.Add("Name:    " + movie.Extra.ROM);
            tv.Nodes[2].Nodes.Add("CRC:     " + movie.Extra.CRC);
            tv.Nodes[2].Nodes.Add("Country: " + movie.Extra.Country);

            tv.Nodes.Add("Extra Information");
            tv.Nodes[3].Nodes.Add("Author:       " + movie.Extra.Author);
            tv.Nodes[3].Nodes.Add("Description:  " + movie.Extra.Description);
            tv.Nodes[3].Nodes.Add("Video Plugin: " + movie.M64Specific.VideoPlugin);
            tv.Nodes[3].Nodes.Add("Audio Plugin: " + movie.M64Specific.AudioPlugin);
            tv.Nodes[3].Nodes.Add("Input Plugin: " + movie.M64Specific.InputPlugin);
            tv.Nodes[3].Nodes.Add("RSP Plugin:   " + movie.M64Specific.RSPPlugin);

            tv.Nodes.Add("Controller Information");
            for (int i = 0; i < 4; i++)
            {
                tv.Nodes[4].Nodes.Add("Controller " + (i + 1));
                tv.Nodes[4].Nodes[i].Nodes.Add("Controller Present: " + movie.M64Specific.Controller[i].Option[0]);
                tv.Nodes[4].Nodes[i].Nodes.Add("Mempak Present:     " + movie.M64Specific.Controller[i].Option[1]);
                tv.Nodes[4].Nodes[i].Nodes.Add("Rumblepak Present:  " + movie.M64Specific.Controller[i].Option[2]);
            }

            movie = null; tv.ExpandAll(); tv.Nodes[0].EnsureVisible();
        }

        /// <summary>
        /// Populate a Dega movie file's header information
        /// </summary>
        public static void MMV(ref TreeView tv, ref TASMovie mmv)
        {
            Dega movie = (Dega)mmv;
            tv.Nodes.Clear();

            tv.Nodes.Add("Header");
            tv.Nodes[0].Nodes.Add("Signature:      " + movie.Header.Signature);
            tv.Nodes[0].Nodes.Add("Version:        " + movie.Header.Version.ToString());
            tv.Nodes[0].Nodes.Add("Frame Count:    " + String.Format("{0:0,0}", movie.Header.FrameCount));
            tv.Nodes[0].Nodes.Add("Rerecord Count: " + String.Format("{0:0,0}", movie.Header.RerecordCount));

            tv.Nodes.Add("Options");
            tv.Nodes[1].Nodes.Add("Movie Start:  " + movie.Options.MovieStart);
            tv.Nodes[1].Nodes.Add("Movie Timing: " + movie.Options.MovieTiming);
            tv.Nodes[1].Nodes.Add("System Type");
            tv.Nodes[1].Nodes[2].Nodes.Add("Japan:     " + movie.MMVSpecific.Japan.ToString());
            tv.Nodes[1].Nodes[2].Nodes.Add("Game Gear: " + movie.MMVSpecific.GameGear.ToString());

            tv.Nodes.Add("Metadata");
            tv.Nodes[2].Nodes.Add("Author: " + movie.Extra.Author);

            tv.Nodes.Add("ROM Information");
            tv.Nodes[3].Nodes.Add("ROM Name: " + movie.Extra.ROM);
            tv.Nodes[3].Nodes.Add("ROM MD5:  " + movie.Extra.CRC);

            tv.Nodes.Add("Controllers");
            tv.Nodes[4].Nodes.Add("Controller 1 Present: " + movie.Input.Controllers[0].ToString());
            tv.Nodes[4].Nodes.Add("Controller 2 Present: " + movie.Input.Controllers[1].ToString());

            movie = null; tv.ExpandAll(); tv.Nodes[0].EnsureVisible();
        }

        /// <summary>
        /// Populate a PCSX movie file's header information
        /// </summary>
        public static void PXM(ref TreeView tv, ref TASMovie mmv)
        {
            PCSX movie = (PCSX)mmv;
            tv.Nodes.Clear();

            tv.Nodes.Add("Header");
            tv.Nodes[0].Nodes.Add("Signature:        " + movie.Header.Signature);
            tv.Nodes[0].Nodes.Add("Movie Version:    " + movie.Header.Version.ToString());
            tv.Nodes[0].Nodes.Add("Emulator Version: " + movie.PXMSpecific.EmulatorVersion.ToString());
            tv.Nodes[0].Nodes.Add("Frame Count:      " + String.Format("{0:0,0}", movie.Header.FrameCount));
            tv.Nodes[0].Nodes.Add("Rerecord Count:   " + String.Format("{0:0,0}", movie.Header.RerecordCount));

            tv.Nodes.Add("Options");
            tv.Nodes[1].Nodes.Add("Movie Start:  " + movie.Options.MovieStart);
            tv.Nodes[1].Nodes.Add("Movie Timing: " + movie.Options.MovieTiming);
            tv.Nodes[1].Nodes.Add("Uses Hacks:   " + movie.PXMSpecific.UsesHacks.ToString());
            tv.Nodes[1].Nodes.Add("Contains Embedded Memory Cards: " + movie.PXMSpecific.HasEmbeddedMemoryCards.ToString());
            tv.Nodes[1].Nodes.Add("Contains Embedded Cheat List:   " + movie.PXMSpecific.HasEmbeddedCheatList.ToString());
            tv.Nodes.Add("Metadata");
            tv.Nodes[2].Nodes.Add("Author: " + movie.Extra.Author);

            tv.Nodes.Add("CD-ROM Information");
            tv.Nodes[3].Nodes.Add("CD-ROM Count: " + movie.PXMSpecific.CDRomCount);
            for (int i = 0; i < movie.PXMSpecific.CDRomCount; i++)
            {
                tv.Nodes[3].Nodes.Add("CD-ROM ID " + (i + 1) + ":  " + movie.Extra.ROM.Substring(i * 9, 9));
            }

            tv.Nodes.Add("Controllers");
            tv.Nodes[4].Nodes.Add("Controller 1 Type: " + movie.PXMSpecific.GetControllerTypeString(0));
            tv.Nodes[4].Nodes.Add("Controller 2 Type: " + movie.PXMSpecific.GetControllerTypeString(1));

            movie = null; tv.ExpandAll(); tv.Nodes[0].EnsureVisible();
        }       
    }
}
