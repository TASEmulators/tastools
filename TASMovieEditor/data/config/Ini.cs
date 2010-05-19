/*
 * AMS.Profile Class Library
 * 
 * Written by Alvaro Mendez
 * Copyright (c) 2005. All Rights Reserved.
 * 
 * The AMS.Profile namespace contains interfaces and classes that 
 * allow reading and writing of user-profile data.
 * This file contains the Ini class.
 * 
 * The code is thoroughly documented, however, if you have any questions, 
 * feel free to email me at alvaromendez@consultant.com.  Also, if you 
 * decide to this in a commercial application I would appreciate an email 
 * message letting me know.
 *
 * This code may be used in compiled form in any way you desire. This
 * file may be redistributed unmodified by any means providing it is 
 * not sold for profit without the authors written consent, and 
 * providing that this notice and the authors name and all copyright 
 * notices remains intact. This file and the accompanying source code 
 * may not be hosted on a website or bulletin board without the author's 
 * written permission.
 * 
 * This file is provided "as is" with no expressed or implied warranty.
 * The author accepts no liability for any damage/loss of business that
 * this product may cause.
 *
 * Last Updated: Feb. 17, 2005
 */


using System;
using System.Text;
using System.Collections;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Reflection;
using System.IO;

namespace AMS.Profile
{
    /// <summary>
    ///   Profile class that utilizes an INI-formatted file to retrieve and save its data. </summary>
    /// <remarks>
    ///   This class works with INI files, which use a simple two-dimensional text format of sections
    ///   and entries to organize their data.  Here's a sample of the format:
    ///   <code>
    ///   [A Section]
    ///   An Entry=Some Value
    ///   Another Entry=Another Value
    ///     
    ///   [Another Section]
    ///   This is cool=True
    ///   </code>
    ///   This class utilizes the <see cref="GetPrivateProfileString" /> and <see cref="WritePrivateProfileString" /> Win32 APIs
    ///   to read and write the INI file.</remarks>
    public class Ini : Profile
    {
        /// <summary>
        ///   Initializes a new instance of the Ini class by setting the <see cref="Profile.Name" /> to <see cref="Profile.DefaultName" />. </summary>
        public Ini()
        {
        }

        /// <summary>
        ///   Initializes a new instance of the Ini class by setting the <see cref="Profile.Name" /> to the given file name. </summary>
        /// <param name="fileName">
        ///   The name of the INI file to initialize the <see cref="Profile.Name" /> property with. </param>
        public Ini(string fileName)
            :
            base(fileName)
        {
        }

        /// <summary>
        ///   Initializes a new instance of the Ini class based on another Ini object. </summary>
        /// <param name="ini">
        ///   The Ini object whose properties and events are used to initialize the object being constructed. </param>
        public Ini(Ini ini)
            :
            base(ini)
        {
        }

        /// <summary>
        ///   Gets the default name for the INI file. </summary>
        /// <remarks>
        ///   For Windows apps, this property returns the name of the executable plus .ini ("program.exe.ini").
        ///   For Web apps, this property returns the full path of <i>web.ini</i> based on the root folder.
        ///   This property is used to set the <see cref="Profile.Name" /> property inside the default constructor.</remarks>
        public override string DefaultName
        {
            get
            {
                return DefaultNameWithoutExtension + ".ini";
            }
        }

        /// <summary>
        ///   Retrieves a copy of itself. </summary>
        /// <returns>
        ///   The return value is a copy of itself as an object. </returns>
        /// <seealso cref="Profile.CloneReadOnly" />
        public override object Clone()
        {
            return new Ini(this);
        }

        // The Win32 API methods
        [DllImport("kernel32", SetLastError = true)]
        static extern int WritePrivateProfileString(string section, string key, string value, string fileName);
        [DllImport("kernel32", SetLastError = true)]
        static extern int WritePrivateProfileString(string section, string key, int value, string fileName);
        [DllImport("kernel32", SetLastError = true)]
        static extern int WritePrivateProfileString(string section, int key, string value, string fileName);
        [DllImport("kernel32")]
        static extern int GetPrivateProfileString(string section, string key, string defaultValue, StringBuilder result, int size, string fileName);
        [DllImport("kernel32")]
        static extern int GetPrivateProfileString(string section, int key, string defaultValue, [MarshalAs(UnmanagedType.LPArray)] byte[] result, int size, string fileName);
        [DllImport("kernel32")]
        static extern int GetPrivateProfileString(int section, string key, string defaultValue, [MarshalAs(UnmanagedType.LPArray)] byte[] result, int size, string fileName);

        /// <summary>
        ///   Sets the value for an entry inside a section. </summary>
        /// <param name="section">
        ///   The name of the section that holds the entry. </param>
        /// <param name="entry">
        ///   The name of the entry where the value will be set. </param>
        /// <param name="value">
        ///   The value to set. If it's null, the entry is removed. </param>
        /// <exception cref="InvalidOperationException">
        ///   <see cref="Profile.ReadOnly" /> is true or
        ///   <see cref="Profile.Name" /> is null or empty. </exception>
        /// <exception cref="ArgumentNullException">
        ///   Either section or entry is null. </exception>
        /// <exception cref="Win32Exception">
        ///   The <see cref="WritePrivateProfileString" /> API failed. </exception>
        /// <remarks>
        ///   If the INI file does not exist, it is created.
        ///   The <see cref="Profile.Changing" /> event is raised before setting the value.  
        ///   If its <see cref="ProfileChangingArgs.Cancel" /> property is set to true, this method 
        ///   returns immediately without setting the value.  After the value has been set, 
        ///   the <see cref="Profile.Changed" /> event is raised. </remarks>
        /// <seealso cref="GetValue" />
        public override void SetValue(string section, string entry, object value)
        {
            // If the value is null, remove the entry
            if (value == null)
            {
                RemoveEntry(section, entry);
                return;
            }

            VerifyNotReadOnly();
            VerifyName();
            VerifyAndAdjustSection(ref section);
            VerifyAndAdjustEntry(ref entry);

            if (!RaiseChangeEvent(true, ProfileChangeType.SetValue, section, entry, value))
                return;

            if (WritePrivateProfileString(section, entry, value.ToString(), Name) == 0)
                throw new Win32Exception();

            RaiseChangeEvent(false, ProfileChangeType.SetValue, section, entry, value);
        }

        /// <summary>
        ///   Retrieves the value of an entry inside a section. </summary>
        /// <param name="section">
        ///   The name of the section that holds the entry with the value. </param>
        /// <param name="entry">
        ///   The name of the entry where the value is stored. </param>
        /// <returns>
        ///   The return value is the entry's value, or null if the entry does not exist. </returns>
        /// <exception cref="InvalidOperationException">
        ///	  <see cref="Profile.Name" /> is null or empty. </exception>
        /// <exception cref="ArgumentNullException">
        ///   Either section or entry is null. </exception>
        /// <seealso cref="SetValue" />
        /// <seealso cref="Profile.HasEntry" />
        public override object GetValue(string section, string entry)
        {
            VerifyName();
            VerifyAndAdjustSection(ref section);
            VerifyAndAdjustEntry(ref entry);

            // Loop until the buffer has grown enough to fit the value
            for (int maxSize = 250; true; maxSize *= 2)
            {
                StringBuilder result = new StringBuilder(maxSize);
                int size = GetPrivateProfileString(section, entry, "", result, maxSize, Name);

                if (size < maxSize - 1)
                {
                    if (size == 0 && !HasEntry(section, entry))
                        return null;
                    return result.ToString();
                }
            }
        }

        /// <summary>
        ///   Removes an entry from a section. </summary>
        /// <param name="section">
        ///   The name of the section that holds the entry. </param>
        /// <param name="entry">
        ///   The name of the entry to remove. </param>
        /// <exception cref="InvalidOperationException">
        ///	  <see cref="Profile.Name" /> is null or empty or
        ///   <see cref="Profile.ReadOnly" /> is true. </exception>
        /// <exception cref="ArgumentNullException">
        ///   Either section or entry is null. </exception>
        /// <exception cref="Win32Exception">
        ///   The <see cref="WritePrivateProfileString" /> API failed. </exception>
        /// <remarks>
        ///   The <see cref="Profile.Changing" /> event is raised before removing the entry.  
        ///   If its <see cref="ProfileChangingArgs.Cancel" /> property is set to true, this method 
        ///   returns immediately without removing the entry.  After the entry has been removed, 
        ///   the <see cref="Profile.Changed" /> event is raised. </remarks>
        /// <seealso cref="RemoveSection" />
        public override void RemoveEntry(string section, string entry)
        {
            // Verify the entry exists
            if (!HasEntry(section, entry))
                return;

            VerifyNotReadOnly();
            VerifyName();
            VerifyAndAdjustSection(ref section);
            VerifyAndAdjustEntry(ref entry);

            if (!RaiseChangeEvent(true, ProfileChangeType.RemoveEntry, section, entry, null))
                return;

            if (WritePrivateProfileString(section, entry, 0, Name) == 0)
                throw new Win32Exception();

            RaiseChangeEvent(false, ProfileChangeType.RemoveEntry, section, entry, null);
        }

        /// <summary>
        ///   Removes a section. </summary>
        /// <param name="section">
        ///   The name of the section to remove. </param>
        /// <exception cref="InvalidOperationException">
        ///	  <see cref="Profile.Name" /> is null or empty or
        ///   <see cref="Profile.ReadOnly" /> is true. </exception>
        /// <exception cref="ArgumentNullException">
        ///   section is null. </exception>
        /// <exception cref="Win32Exception">
        ///   The <see cref="WritePrivateProfileString" /> API failed. </exception>
        /// <remarks>
        ///   The <see cref="Profile.Changing" /> event is raised before removing the section.  
        ///   If its <see cref="ProfileChangingArgs.Cancel" /> property is set to true, this method 
        ///   returns immediately without removing the section.  After the section has been removed, 
        ///   the <see cref="Profile.Changed" /> event is raised. </remarks>
        /// <seealso cref="RemoveEntry" />
        public override void RemoveSection(string section)
        {
            // Verify the section exists
            if (!HasSection(section))
                return;

            VerifyNotReadOnly();
            VerifyName();
            VerifyAndAdjustSection(ref section);

            if (!RaiseChangeEvent(true, ProfileChangeType.RemoveSection, section, null, null))
                return;

            if (WritePrivateProfileString(section, 0, "", Name) == 0)
                throw new Win32Exception();

            RaiseChangeEvent(false, ProfileChangeType.RemoveSection, section, null, null);
        }

        /// <summary>
        ///   Retrieves the names of all the entries inside a section. </summary>
        /// <param name="section">
        ///   The name of the section holding the entries. </param>
        /// <returns>
        ///   If the section exists, the return value is an array with the names of its entries; 
        ///   otherwise it's null. </returns>
        /// <exception cref="InvalidOperationException">
        ///	  <see cref="Profile.Name" /> is null or empty. </exception>
        /// <seealso cref="Profile.HasEntry" />
        /// <seealso cref="GetSectionNames" />
        public override string[] GetEntryNames(string section)
        {
            // Verify the section exists
            if (!HasSection(section))
                return null;

            VerifyAndAdjustSection(ref section);

            // Loop until the buffer has grown enough to fit the value
            for (int maxSize = 500; true; maxSize *= 2)
            {
                byte[] bytes = new byte[maxSize];
                int size = GetPrivateProfileString(section, 0, "", bytes, maxSize, Name);

                if (size < maxSize - 2)
                {
                    // Convert the buffer to a string and split it
                    string entries = Encoding.ASCII.GetString(bytes, 0, size - (size > 0 ? 1 : 0));
                    if (entries == "")
                        return new string[0];
                    return entries.Split(new char[] { '\0' });
                }
            }
        }

        /// <summary>
        ///   Retrieves the names of all the sections. </summary>
        /// <returns>
        ///   If the INI file exists, the return value is an array with the names of all the sections;
        ///   otherwise it's null. </returns>
        /// <seealso cref="Profile.HasSection" />
        /// <seealso cref="GetEntryNames" />
        public override string[] GetSectionNames()
        {
            // Verify the file exists
            if (!File.Exists(Name))
                return null;

            // Loop until the buffer has grown enough to fit the value
            for (int maxSize = 500; true; maxSize *= 2)
            {
                byte[] bytes = new byte[maxSize];
                int size = GetPrivateProfileString(0, "", "", bytes, maxSize, Name);

                if (size < maxSize - 2)
                {
                    // Convert the buffer to a string and split it
                    string sections = Encoding.ASCII.GetString(bytes, 0, size - (size > 0 ? 1 : 0));
                    if (sections == "")
                        return new string[0];
                    return sections.Split(new char[] { '\0' });
                }
            }
        }
    }
}
