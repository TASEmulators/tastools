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
using System.Windows.Forms;
using System.Diagnostics;

using AMS.Profile;

namespace MovieSplicer.UI.Methods
{
    class AppSettings
    {
        const string SETTINGS_FILE = "tas-editor.ini";
        const string SECTION_FILES = "Recent Files";
        const byte MAX_FILES = 9;

        /// <summary>
        /// Save a filename as the first recent file no matter it is old or new
        /// Bring up the new file if it is already on the list
        /// Only MAX_FILES are stored, so bump the last entry off the list
        /// </summary>        
        public static void Save(string filename)
        {

            Ini settingsFile = new Ini();
            string[] files = Load();

            settingsFile.SetValue(SECTION_FILES, "File 1", filename);

            int n = 1;
            foreach (string file in files)
            {
                if (n >= MAX_FILES) break;
                if (file != null && file != filename)
                {
                    n++;
                    settingsFile.SetValue(SECTION_FILES, "File " + n, file);
                }
            }
        }

        /// <summary>
        /// Load the recent files from the ini file
        /// </summary>        
        public static string[] Load()
        {
            Ini settingsFile = new Ini();
            string[] files = new string[MAX_FILES];

            for (int i = 0; i < MAX_FILES; i++)
            {
                if (settingsFile.HasEntry(SECTION_FILES, "File " + (i + 1)))
                    files[i] = settingsFile.GetValue(SECTION_FILES, "File " + (i + 1)).ToString();
            }

            return files;
        }
    }
}
