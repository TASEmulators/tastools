/******************************************************************************** 
 * TAS Movie Editor                                                             *
 *                                                                              *
 * Copyright notice for this file:                                              *
 *  Copyright (C) 2006-2009 Maximus                                             *
 *                                                                              *
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
 *                                                                              *
 * Conventions:                                                                 *
 * - Class Level Variables, Methods and Properties are denoted with each word   *   
 *   appearing with the first letter capitalized: ie. FrameData, Save()         *
 * - private Class Variables and methods are denoted with the first word being  *
 *   lowercase, then the subsequent words having their first letter             *
 *   capitalized: ie. openDlg, parseControllerData()                            *
 *                                                                              *
 * Inline Comments:                                                             *
 * - NOTE  -> contains an explanation for a block of code                       *
 * - HACK  -> same as a note, but generally describes sub-optimal code          *
 * - TODO  -> placeholder for a task                                            *
 * - DEBUG -> identifies a problematic block of code                            *
 *******************************************************************************/
using System;
using System.Resources;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using MovieSplicer.Data;
using MovieSplicer.Data.Formats;
using MovieSplicer.Components;

namespace MovieSplicer.UI
{    
    public partial class frmMain : TASForm
    {
        public static frmMain frm;

        private TASMovie                Movie;
        private TASMovieInputCollection FrameData;
        private TASMovieInputCollection FrameBuffer;
        private UndoBuffer              UndoHistory;
        
        // will contain the message history for this session
        frmMessages Msg = new frmMessages();
        frmEditing  Editor;
        FindReplaceDialog frd;
        
        /// <summary>
        /// Class constructor. Sets up the common components and controls
        /// </summary>
        public frmMain()
        {
            InitializeComponent();
            frd = new FindReplaceDialog(this);
            UndoHistory = new UndoBuffer();
            
            this.Text = APP_TITLE + " v" + VERSION;            
            populateRecentFiles();
        }                               
        
        // pass menu options to editor on change
        // NOTE::effective, but optimal?
        private void mnuAutoFireOption_CheckStateChanged(object sender, EventArgs e)
        {
            if (Editor != null) Editor.AutoFire = mnuAutoFireOption.Checked;
        }

        private void mnuEditingPrompt_CheckStateChanged(object sender, EventArgs e)
        {
            if (Editor != null) Editor.EditingPrompts = mnuEditingPrompt.Checked;
        }

    #region Frame Data Handlers

        /// <summary>
        /// Populate edit controls with frame data        
        /// </summary>
        private void lvInput_Clicked(object sender, EventArgs e)
        {
            if (lvInput.SelectedIndices.Count > 0 && FrameData.Format != MovieType.None)
            {
                ListViewItem lvi = lvInput.Items[lvInput.SelectedIndices[0]];
                Editor.PopulateEditFields(lvi);                
            }
        }

        /// <summary>
        /// Populate edit controls if navigating via arrow keys
        /// </summary>        
        private void lvInput_KeyUp(object sender, KeyEventArgs e)
        {
            if (lvInput.SelectedIndices.Count > 0 && FrameData.Format != MovieType.None)
            {
                ListViewItem lvi = lvInput.Items[lvInput.SelectedIndices[0]];
                Editor.PopulateEditFields(lvi);
            }
        }

        /// <summary>
        /// Edit the movie when th euser submits new input
        /// </summary>        
        private void lvInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (lvInput.SelectedIndices.Count == 0) return;
            return;
            /*
            char[] input = new char[0];
            input[0] = "";
            if (FrameData.Format != MovieType.None)
            {
                switch (e.KeyChar)
                {
                    //WSAD
                    case 'w':
                    case 'W':
                        input = "^";
                        break;
                    case 'a':
                    case 'A':
                        input = "<";
                        break;
                    case 's':
                    case 'S':
                        input = "v";
                        break;
                    case 'd':
                    case 'D':
                        input = ">";
                        break;
                    //Num-pad snes buttons
                    case '1':
                        input = "Y";
                        break;
                    case '0':
                        input = "B";
                        break;
                    case '.':
                        input = "A";
                        break;
                    case '3':
                        input = "X";
                        break;
                    case '4':
                        input = "L";
                        break;
                    case '5':
                        input = "R";
                        break;
                    //Bah...
                    case 'x':
                    case 'X':
                        input = "S";
                        break;
                    case 'z':
                    case 'Z':
                        input = "s";
                        break;
                    case 'r':
                    case 'R':
                        input = "(reset)";
                        break;
                    //Clear
                    case '\b':
                        input = "\b";
                        break;
                }
            }
            if (input[0] == "") return;

            int frameIndex = lvInput.SelectedIndices[0];
            int framePosition = Convert.ToInt32(lvInput.Items[frameIndex].Text);
            int totalFrames = lvInput.SelectedIndices[lvInput.SelectedIndices.Count - 1] - frameIndex + 1;
            bool[] updateFlag = { false, false, false, false };

            TASMovieInput updated = new TASMovieInput();
            for (int i = 0; i < updated.Controller.Length; i++)
                updated.Controller[i] = FrameData[frameIndex].Controller[i];


            // if no controllers were set, return
            if (!Editor.IsControllerChecked(0)
                && !Editor.IsControllerChecked(1)
                && !Editor.IsControllerChecked(2)
                && !Editor.IsControllerChecked(3)
                )
                return;

            UndoBuffer.Add(ref UndoHistory, ref FrameData);

            for( int i = 0; i < 4; i++ )
            {
                if (!Editor.IsControllerChecked(i)) continue;

                // append or overwrite check
                if (chkAppendInput.Checked)
                    TASMovieInput.ToggleUpdate(ref FrameData, updated, updateFlag, AutoFire, frameIndex, totalFrames);
                else
                    TASMovieInput.Insert(ref FrameData, updated, updateFlag, AutoFire, frameIndex, totalFrames);

                updated.Controller[0] = txtFrameDataC1.Text;
            }

            lvInput.ClearVirtualCache();
            lvInput.Refresh();
            Msg.AddMsg("Updated " + totalFrames + " frame(s) at frame " + framePosition);
             * */
        }

        /// <summary>
        /// Change the drag cursor
        /// </summary>        
        private void lvInput_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Move;
        }

        /// <summary>
        /// Load the move that's dropped
        /// </summary>        
        private void lvInput_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

            // we can only load 1 file at a time, so take the first in the collection
            // and ignore the rest
            loadMovie(files[0], false);
        }
    
    #endregion
   
    #region Menu Actions

        /// <summary>
        /// Loads a TAS into the editor
        /// </summary>        
        private void loadMovie(string filename, bool fromRecent)
        {            
            resetApplication();
            
            // initialize the editor
            Editor = new frmEditing();
            mnuEditing.Enabled = true;

            // copy the clean frame data to the undo buffer as the first item
            mnuUndoChange.Enabled = true;

            // make sure the file isn't locked before we do anything else
            System.IO.FileStream fs = null;
            try
            {
                fs = System.IO.File.OpenRead(filename);

                FrameData.Format = IsValid(filename);
                ResourceManager rm = new ResourceManager("MovieSplicer.Properties.Resources", GetType().Assembly);

                // load the movie object up with the correct format and display a thumbnail
                switch (FrameData.Format)
                {
                    case MovieType.SMV:
                        Movie = new SNES9x(filename);
                        Methods.PopulateMovieInfo.SMV(ref tvInfo, ref Movie);
                        pbFormat.Image = ((System.Drawing.Icon)(rm.GetObject("icon_smv"))).ToBitmap();
                        break;
                    case MovieType.FCM:
                        Movie = new FCEU(filename);
                        Methods.PopulateMovieInfo.FCM(ref tvInfo, ref Movie);
                        pbFormat.Image = ((System.Drawing.Icon)(rm.GetObject("icon_fcm"))).ToBitmap();
                        break;
                    case MovieType.GMV:
                        Movie = new Gens(filename);
                        Methods.PopulateMovieInfo.GMV(ref tvInfo, ref Movie);
                        pbFormat.Image = ((System.Drawing.Icon)(rm.GetObject("icon_gmv"))).ToBitmap();
                        break;
                    case MovieType.FMV:
                        Movie = new Famtasia(filename);
                        Methods.PopulateMovieInfo.FMV(ref tvInfo, ref Movie);
                        pbFormat.Image = ((System.Drawing.Icon)(rm.GetObject("icon_fmv"))).ToBitmap();
                        break;
                    case MovieType.VBM:
                        Movie = new VisualBoyAdvance(filename);
                        Methods.PopulateMovieInfo.VBM(ref tvInfo, ref Movie);
                        pbFormat.Image = ((System.Drawing.Icon)(rm.GetObject("icon_vbm"))).ToBitmap();
                        break;
                    case MovieType.M64:
                        Movie = new Mupen64(filename);
                        Methods.PopulateMovieInfo.M64(ref tvInfo, ref Movie);
                        pbFormat.Image = ((System.Drawing.Icon)(rm.GetObject("icon_m64"))).ToBitmap();
                        break;
                    case MovieType.MMV:
                        Movie = new Dega(filename);
                        Methods.PopulateMovieInfo.MMV(ref tvInfo, ref Movie);
                        pbFormat.Image = ((System.Drawing.Icon)(rm.GetObject("icon_mmv"))).ToBitmap();
                        break;
                    case MovieType.PXM:
                        Movie = new PCSX(filename);
                        Methods.PopulateMovieInfo.PXM(ref tvInfo, ref Movie);
                        pbFormat.Image = ((System.Drawing.Icon)(rm.GetObject("icon_pxm"))).ToBitmap();
                        break;
                    case MovieType.None:
                        resetApplication();
                        return;
                }

                // destroy the resource manager instance
                rm = null;

                // assign the shared input collection to the current movie's            
                FrameData.Input = Movie.Input.FrameData;
                FrameData.Controllers = Movie.Input.ControllerCount;

                // set the number of controller columns
                lvInput.SetColumns(Movie.Input.ControllerCount);
                lvInput.VirtualMovieType = FrameData.Format;

                // initialize editing fields            
                bool[] activeControllers = { false, false, false, false, false };
                for (int i = 0; i < Movie.Input.ControllerCount; i++)
                    activeControllers[i] = Movie.Input.Controllers[i];
                Editor.ToggleInputBoxes(activeControllers);

                // trim the filename and throw it into a text field
                txtMovieFilename.Text = FilenameFromPath(filename);

                // enable grayed menu options
                mnuSave.Enabled = true;
                mnuSaveAs.Enabled = true;
                mnuClose.Enabled = true;

                // populate the virtual listview 
                lvInput.ClearVirtualCache();
                lvInput.VirtualListSource = FrameData.Input;
                lvInput.VirtualListSize = FrameData.Input.Length;

                // add frame count to statusbar
                sbarFrameCount.Text = FrameData.Input.Length.ToString();
                frd.GotoFrameNumberLabel = getFrameNumberRange();

                Editor.LoadSharedObjects(ref lvInput, ref FrameData.Input, ref UndoHistory, ref Msg);
                Msg.AddMsg("Successfully loaded " + FilenameFromPath(filename));

                // show subtitle export option
                mnuExportSRT.Enabled = true;

                Methods.AppSettings.Save(filename);
                populateRecentFiles();

                runMovieGeneratorToolStripMenuItem.Enabled = true;
            }
            catch
            {
                MessageBox.Show(this,
                    filename + " cannot be accessed at the moment.\nEither the file is locked or it doesn't exist.",
                    "File Access Error",
                    MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
                return;
            }
            finally
            {
                if (fs != null) fs.Close();
            }
        }

        /// <summary>
        /// If the INI file exists and there are movies populated, populate some menuitems
        /// </summary>
        private void populateRecentFiles()
        {
            string[] files = Methods.AppSettings.Load();
            mnuOpenRecent.DropDown.Items.Clear();
            foreach (string file in files)
            {
                if (file == null) return;
                mnuOpenRecent.DropDown.Items.Add(file, null, new System.EventHandler(this.recentFileClicked));    
            }
        }

        /// <summary>
        /// Recent file(s) list click handler
        /// </summary>        
        private void recentFileClicked(object sender, EventArgs e)
        {
            loadMovie(sender.ToString(), true);
        }

        /// <summary>
        /// Reset all objects and controls to their default state
        /// </summary>
        private void resetApplication()
        {
            // disable menu commands that require a movie to be loaded in order to operate
            mnuSave.Enabled   = false;
            mnuSaveAs.Enabled = false;
            mnuClose.Enabled  = false;
            mnuExportSRT.Enabled = true;

            // reset the input list
            lvInput.SetColumns(0);
            lvInput.VirtualListSize = 0;

            // clear the movie treeview
            tvInfo.Nodes.Clear();

            // nullify the input data reference
            FrameData.Input  = null;
            FrameData.Format = MovieType.None;

            // reset the editor
            if (Editor != null)
                Editor.Close();
            Editor = null;
            mnuEditing.Enabled = false;

            // clear the undo history
            UndoHistory.Changes = new TASMovieInput[0][];
            mnuUndoChange.Enabled = false;

            Movie = null;

            // clear the icon
            pbFormat.Image = null;

            runMovieGeneratorToolStripMenuItem.Enabled = false;

            // reset filename
            txtMovieFilename.Text = "";
            sbarFrameCount.Text = "0";
            frd.GotoFrameNumberLabel = getFrameNumberRange();
        }

        /// <summary>
        /// Show an OpenFileDialog to allow a file to be selected and loaded
        /// </summary>        
        private void mnuOpen_Click(object sender, EventArgs e)
        {
            openDlg        = new OpenFileDialog();
            openDlg.Filter = TAS_FILTER;
            openDlg.ShowDialog();

            if (openDlg.FileName.Length > 0)
                loadMovie(openDlg.FileName, false);
            
            openDlg.Dispose();            
        }

        /// <summary>
        /// Save the current movie, overwriting the original
        /// </summary>
        private void mnuSave_Click(object sender, EventArgs e)
        {
            DialogResult verifyOverwrite = MessageBox.Show(this,
                    "Are you sure you want to overwrite the existing file?",
                    "Confirm Overwrite",
                    MessageBoxButtons.OKCancel, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button1);
 
            if (verifyOverwrite != DialogResult.OK)
                return;

            Movie.Save("", ref FrameData.Input);
        }

        /// <summary>
        /// Save the current movie out to a new file
        /// </summary>        
        private void mnuSaveAs_Click(object sender, EventArgs e)
        {
            if (FrameData.Format != MovieType.None)
            {                               
                frmSaveAs frm = new frmSaveAs(ref Movie, ref FrameData, "new-");
                frm.ShowDialog(); frm.Dispose();
            }
        }

        /// <summary>
        /// Close the currently open file
        /// </summary>        
        private void mnuClose_Click(object sender, EventArgs e)
        {
            resetApplication();
        }

        /// <summary>
        /// Show the message history window
        /// </summary>        
        private void mnuMessageHistory_Click(object sender, EventArgs e)
        {
            Msg.ShowDialog(this);
        }

        /// <summary>
        /// Terminate the application
        /// </summary>        
        private void mnuExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        /// <summary>
        /// Show the splicing form
        /// </summary>        
        private void mnuSplice_Click(object sender, EventArgs e)
        {
            frmSplice frm = new frmSplice();
            frm.ShowDialog();
        }

        /// <summary>
        ///  Display the editor windows
        /// </summary>        
        private void mnuEditing_Click(object sender, EventArgs e)
        {
            // NOTE::refresh the object pointers
            Editor.LoadSharedObjects(ref lvInput, ref FrameData.Input, ref UndoHistory, ref Msg);   

            if (!Editor.Visible)
                Editor.Show(this);
            else
                Editor.Focus();
        }

        /// <summary>
        /// Load the subtitle export form
        /// </summary>        
        private void mnuExportSRT_Click(object sender, EventArgs e)
        {
            frmSubtitles frm = new frmSubtitles(ref FrameData);
            frm.ShowDialog(); frm.Dispose();
        }

        /// <summary>
        /// Show the movie compare form
        /// </summary>    
        private void mnuCompareMovieInputRange_Click(object sender, EventArgs e)
        {
            frmCompare frm = new frmCompare();
            frm.ShowDialog(); frm.Dispose();
        }

    #endregion   
   
    #region Editing
        
        /////////////////////////////////////////////////////////////////////////////////
        // TODO::Move all editing code out of the main class and into the editor class //
        /////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Update the listview virtualListSize and the frame count in the statusbar
        /// </summary>
        private void updateControlsAfterEdit()
        {
            lvInput.ClearVirtualCache();
            lvInput.VirtualListSource = FrameData.Input;
            lvInput.VirtualListSize   = FrameData.Input.Length;
            lvInput.Refresh();

            sbarFrameCount.Text = FrameData.Input.Length.ToString();
            frd.GotoFrameNumberLabel = getFrameNumberRange();

            // NOTE::refresh the object pointers
            // DEBUG::Not the most elegant solution
            Editor.LoadSharedObjects(ref lvInput, ref FrameData.Input, ref UndoHistory, ref Msg); 
            
        }

        /// <summary>
        /// Insert a blank row into the listview at the selectedIndex point and
        /// update the inputArray, or prompt for insertion of multiple frames based
        /// on how many frames were selected.
        /// </summary>
        private void insertFrames()
        {
            int framePosition = FrameData.Input.Length - 1;
            int totalFrames = 1;

            // make sure something is selected
            if (lvInput.SelectedIndices.Count > 0)
            {
                framePosition = lvInput.SelectedIndices[0];
                totalFrames = lvInput.SelectedIndices.Count;
            }

            if (FrameData.Input.Length == 0)
            {
                framePosition = 0;
            }

            // prompt for multiple frame insertion
            if (lvInput.SelectedIndices.Count > 1 && mnuEditingPrompt.Checked)
            {
                DialogResult confirmAdd = MessageBox.Show(this,
                        "Are you sure you want to insert " + totalFrames + " frames at frame " + framePosition,
                        "Confirm Multiple Frame Insertion",
                        MessageBoxButtons.OKCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);

                if (confirmAdd != DialogResult.OK) return;
            }

            UndoBuffer.Add(ref UndoHistory, ref FrameData.Input);
            TASMovieInput.Insert(ref FrameData.Input, new TASMovieInput(), framePosition, totalFrames);

            updateControlsAfterEdit();
            Msg.AddMsg("Inserted " + totalFrames + " frame(s) at position " + framePosition);
        }
   
        /// <summary>
        /// Append a blank row into the listview after the selectedIndex point and
        /// update the inputArray, or prompt for insertion of multiple frames based
        /// on how many frames were selected.
        /// </summary>
        private void appendFrames()
        {
            int framePosition = FrameData.Input.Length;
            int totalFrames = 1;

            // make sure something is selected
            if (lvInput.SelectedIndices.Count > 0)
            {
                framePosition = lvInput.SelectedIndices[lvInput.SelectedIndices.Count - 1] + 1;
                totalFrames = lvInput.SelectedIndices.Count;
            }

            if (FrameData.Input.Length == 0)
            {
                framePosition = 0;
            }

            // prompt for multiple frame insertion
            if (lvInput.SelectedIndices.Count > 1 && mnuEditingPrompt.Checked)
            {
                DialogResult confirmAdd = MessageBox.Show(this,
                        "Are you sure you want to append " + totalFrames + " frames after frame " + framePosition,
                        "Confirm Multiple Frame Insertion",
                        MessageBoxButtons.OKCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);

                if (confirmAdd != DialogResult.OK) return;
            }

            UndoBuffer.Add(ref UndoHistory, ref FrameData.Input);
            TASMovieInput.Insert(ref FrameData.Input, new TASMovieInput(), framePosition, totalFrames);

            updateControlsAfterEdit();
            Msg.AddMsg("Appended " + totalFrames + " frame(s) after position " + framePosition);
        }

        /// <summary>
        /// Remove the row from the listview at the selectedIndex point and
        /// update the inputArray, or if multiple rows have been selected, prompt for deletion
        /// </summary>
        private void removeFrames()
        {
            // make sure something is selected
            if (lvInput.SelectedIndices.Count == 0) return;

            int[] selectedIndices = new int[lvInput.SelectedIndices.Count];
            lvInput.SelectedIndices.CopyTo(selectedIndices, 0);

            //int framePosition = Convert.ToInt32(lvInput.Items[frameIndex].Text);

            // prompt for multiple frame insertion
            if (lvInput.SelectedIndices.Count > 1 && mnuEditingPrompt.Checked)
            {
                DialogResult confirmDelete = MessageBox.Show(this,
                        "Are you sure you want to remove the selected " + lvInput.SelectedIndices.Count + " frames", 
                        "Confirm Multiple Frame Removal",
                        MessageBoxButtons.OKCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);
 
                if (confirmDelete != DialogResult.OK) return;
            }

            UndoBuffer.Add(ref UndoHistory, ref FrameData.Input);
            TASMovieInput.Remove(ref FrameData.Input, selectedIndices);

            //*
            // HACK::ensures that the virtual list doesn't try to access an element that 
            // no longer exists after a block of frames is deleted
            int focusPos = selectedIndices[selectedIndices.Length - 1] - selectedIndices.Length;
            if (FrameData.Input.Length > 0)
            {
                // if we haven't removed all frames...
                lvInput.Items[focusPos].Focused = true;
                lvInput.Focus();
                lvInput.EnsureVisible(focusPos);
            }
            lvInput.SelectedIndices.Clear();
            //*/

            updateControlsAfterEdit();
            Msg.AddMsg("Removed " + lvInput.SelectedIndices.Count + " frame(s)");
        }

        /// <summary>
        /// Insert frame (context menu)
        /// </summary>
        private void cmnuitemInsertFrame_Click(object sender, EventArgs e)
        {
            insertFrames();
        }

        /// <summary>
        /// Append frame (context menu)
        /// </summary>
        private void cmnuitemAppendFrame_Click(object sender, EventArgs e)
        {
            appendFrames();
        }

        /// <summary>
        /// Remove frame (context menu)
        /// </summary>
        private void cmnuitemRemoveFrames_Click(object sender, EventArgs e)
        {
            removeFrames();
        }

    #endregion
    
    #region Copy-Pasting
        
        /// <summary>
        /// Reset the paste-specific controls to their default values
        /// </summary>
        private void resetPasteControls()
        {            
            sbarCopyBufferSize.Text     = "0";
            FrameBuffer.Format          = MovieType.None;
            sbarCopyBufferType.Text     = Enum.GetName(typeof(MovieType), FrameBuffer.Format);
            mnuPasteBefore.Enabled = false;
            mnuPasteAfter.Enabled = false;
            cmnuitemPasteFramesBefore.Enabled = false;
            cmnuitemPasteFramesAfter.Enabled = false;
        }

        /// <summary>
        /// Set various controls when the copy buffer is filled
        /// </summary>
        private void enablePasteControls()
        {
            sbarCopyBufferType.Text     = Enum.GetName(typeof(MovieType), FrameBuffer.Format);
            sbarCopyBufferSize.Text     = FrameBuffer.Input.Length.ToString();
            mnuPasteBefore.Enabled = true;
            mnuPasteAfter.Enabled = true;
            cmnuitemPasteFramesBefore.Enabled = true;
            cmnuitemPasteFramesAfter.Enabled = true;
        }

        /// <summary>
        /// Copy the selected frames to the buffer arraylist
        /// </summary>
        private void copyFrames()
        {
            // make sure something is selected
            if (lvInput.SelectedIndices.Count == 0) return;
                                    
            int[] selectedIndices = new int[lvInput.SelectedIndices.Count];            
            lvInput.SelectedIndices.CopyTo(selectedIndices, 0);

            FrameBuffer.Controllers = FrameData.Controllers;
            FrameBuffer.Format      = FrameData.Format;
            FrameBuffer.Input       = TASMovieInput.Copy(ref FrameData.Input, selectedIndices);            
            
            enablePasteControls();   
        }

        /// <summary>
        /// Cut the selected frames to the buffer arraylist
        /// </summary>
        private void cutFrames()
        {
            copyFrames();
            removeFrames();
        }
        
        /// <summary>
        /// Insert the buffered frame input at/after the selected position.
        /// </summary>
        private void pasteFrames(bool after)
        {
            bool notEmpty = (lvInput.Items.Count > 0);

            if (notEmpty)
            {
                // check for a valid paste position
                if (lvInput.SelectedIndices.Count == 0) return;
            }

            int framePos = notEmpty ? (after ? lvInput.SelectedIndices[lvInput.SelectedIndices.Count - 1] + 1: lvInput.SelectedIndices[0]) : 0;

            // confirm whether to paste frames
            if (mnuEditingPrompt.Checked)
            {
                DialogResult confirmPaste = MessageBox.Show(this,
                        "Paste " + FrameBuffer.Input.Length + " frame(s) "
                        + (notEmpty && after ? "after frame " + (framePos - 1) : "at frame " + framePos) + " ?", 
                        "Confirm Paste",
                        MessageBoxButtons.OKCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);

                if (confirmPaste != DialogResult.OK) return;
            }

            UndoBuffer.Add(ref UndoHistory, ref FrameData.Input);
            TASMovieInput.Insert(ref FrameData.Input, ref FrameBuffer.Input, framePos);

           updateControlsAfterEdit();

           lvInput.SelectedIndices.Clear();
            for (int i = 0; i < FrameBuffer.Input.Length; i++)
            {
                lvInput.Items[framePos + i].Selected = true;
            }

            int focusPos = lvInput.SelectedIndices[lvInput.SelectedIndices.Count - 1];
            lvInput.Items[focusPos].Focused = true;
            lvInput.Focus();
            lvInput.EnsureVisible(focusPos);
            lvInput.Refresh();

            Msg.AddMsg("Pasted " + FrameBuffer.Input.Length + " frame(s)");
        }

        /// <summary>
        /// Insert the buffered frame input at/after each selected position.
        /// </summary>
        private void pasteFramesMultiple(bool after)
        {
            // check for a valid paste position
            if (lvInput.SelectedIndices.Count <= 1) return;

            int[] selectedIndices = new int[lvInput.SelectedIndices.Count];
            if (after)
                for (int i = 0; i < selectedIndices.Length; i++)
                    selectedIndices[i] = lvInput.SelectedIndices[i] + 1;
            else
                lvInput.SelectedIndices.CopyTo(selectedIndices, 0);

            bool multiplePaste = false;
            // ask whether to paste multiple times
            if (mnuEditingPrompt.Checked)
            {
                DialogResult confirmPaste = MessageBox.Show(this,
                        "Paste " + selectedIndices.Length + " chunks of " + FrameBuffer.Input.Length + " frame(s) "
                        + (after ? "after" : "before") + " each selected frame?\nWarning: this might be extremely slow!",
                        "Confirm Paste",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);

                if (confirmPaste == DialogResult.Yes) multiplePaste = true;
                else if (confirmPaste == DialogResult.No) multiplePaste = false;
                else return;
            }

            UndoBuffer.Add(ref UndoHistory, ref FrameData.Input);
            if (multiplePaste)
                TASMovieInput.InsertMultiple(ref FrameData.Input, ref FrameBuffer.Input, selectedIndices);
            else
                TASMovieInput.Insert(ref FrameData.Input, ref FrameBuffer.Input,
                    (after ? selectedIndices[selectedIndices.Length - 1] : selectedIndices[0]));

            lvInput.ClearVirtualCache();
            lvInput.VirtualListSource = FrameData.Input;
            lvInput.VirtualListSize = FrameData.Input.Length;
            lvInput.SelectedIndices.Clear();
            lvInput.Refresh();

            if (multiplePaste)
            {
                for (int i = 0; i < selectedIndices.Length; i++)
                {
                    for (int j = 0; j < FrameBuffer.Input.Length; j++)
                    {
                        lvInput.Items[selectedIndices[i] + i * FrameBuffer.Input.Length + j].Selected = true;
                    }
                }
            }
            else
            {
                int startPos = (after ? selectedIndices[selectedIndices.Length - 1] : selectedIndices[0]);
                for (int i = 0; i < FrameBuffer.Input.Length; i++)
                {
                    lvInput.Items[startPos + i].Selected = true;
                }
            }
            int focusPos = lvInput.SelectedIndices[lvInput.SelectedIndices.Count - 1];
            lvInput.Items[focusPos].Focused = true;
            lvInput.Focus();
            lvInput.EnsureVisible(focusPos);
            updateControlsAfterEdit();

            if (multiplePaste)
                Msg.AddMsg("Pasted " + lvInput.SelectedIndices.Count + " chunks of " + FrameBuffer.Input.Length + " frame(s)");
            else
                Msg.AddMsg("Pasted " + FrameBuffer.Input.Length + " frame(s)");
        }

        /// <summary>
        /// Show the buffer form (pass in the buffer array and the buffer's MovieType)
        /// </summary>        
        private void mnuViewBuffer_Click(object sender, EventArgs e)
        {
            frmBuffer frm = new frmBuffer(ref FrameBuffer);
            frm.ShowDialog(this);
            frm.Dispose(ref FrameBuffer);
            
            // if the buffer array comes back empty, reset all the copy/paste options
            if (FrameBuffer.Input == null)
                resetPasteControls();
            else
                enablePasteControls();
        }

        /// <summary>
        /// Copy Frames (main menu)
        /// </summary>        
        private void mnuCopy_Click(object sender, EventArgs e)
        {
            copyFrames();
        }

        /// <summary>
        /// Cut Frames (main menu)
        /// </summary>        
        private void mnuCut_Click(object sender, EventArgs e)
        {
            cutFrames();
        }

        /// <summary>
        /// Paste frames before (main menu)
        /// </summary>        
        private void mnuPasteBefore_Click(object sender, EventArgs e)
        {
            pasteFrames(false);
        }

        /// <summary>
        /// Paste frames after (main menu)
        /// </summary>        
        private void mnuPasteAfter_Click(object sender, EventArgs e)
        {
            pasteFrames(true);
        }

        /// <summary>
        /// Copy Frames (context menu)
        /// </summary>        
        private void cmnuitemCopyFrames_Click(object sender, EventArgs e)
        {
            copyFrames();
        }

        /// <summary>
        /// Cut Frames (context menu)
        /// </summary>        
        private void cmnuitemCutFrames_Click(object sender, EventArgs e)
        {
            cutFrames();
        }

        /// <summary>
        /// Paste frames before (context menu)
        /// </summary>
        private void cmnuitemPasteFramesBefore_Click(object sender, EventArgs e)
        {
            pasteFrames(false);
        }

        /// <summary>
        /// Paste frames after (context menu)
        /// </summary>
        private void cmnuitemPasteFramesAfter_Click(object sender, EventArgs e)
        {
            pasteFrames(true);
        }        

    #endregion                                                              

    #region Select/Find/Replace/GoTo
        public bool findNext( string find, bool downward )
        {
            if (Movie == null ) return false;

            int start = 0;
            int end   = lvInput.Items.Count;
            if (downward)
            {
                start = lvInput.SelectedIndices.Count > 0 ? lvInput.SelectedIndices[0] + 1 : 0;
            }
            else
            {
                start = (lvInput.SelectedIndices.Count > 0 ? lvInput.SelectedIndices[lvInput.SelectedIndices.Count - 1] : end) - 1;
                end   = -1;
            }

            int position = TASMovieInput.Search(ref FrameData.Input, find, start, end);
            if (position < 0)
            {
                if (lvInput.SelectedIndices.Count > 0)
                {
                    DialogResult answer = MessageBox.Show(MovieSplicer.UI.frmMain.frm,
                        "Input pattern not found between selected position and end of movie.\nContinue searching from the "
                        + (downward ? "beginning" : "end") + " of the movie?", "Continue",
                        MessageBoxButtons.OKCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);
                    if (answer == DialogResult.OK)
                    {
                        end      = downward ? 0 : lvInput.Items.Count - 1;
                        position = TASMovieInput.Search(ref FrameData.Input, find, end, start);
                    }
                }
            }

            if (position >= 0 && position < FrameData.Input.Length)
            {
                // clear the selection collection
                // NOTE::This is since we will be creating a new selected items collection 
                // with only 1 item (the resulting frame), but we don't want to clear if no results are
                // found (since it'll just jump back to the top and repeat if we do)
                lvInput.SelectedIndices.Clear();

                lvInput.Items[position].Selected = true;
                lvInput.Items[position].Focused = true;
                lvInput.Focus();
                lvInput.EnsureVisible(position);

                return true;
            }
            else
            {
                MessageBox.Show(this,
                        "Input pattern not found",
                        "Sorry",
                        MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
 
                return false;
            }
         }

        public void replaceCurrentSelection(string replaceWith)
        {
            if (Movie == null) return;
            if (lvInput.SelectedIndices.Count < 1) return;


            UndoBuffer.Add(ref UndoHistory, ref FrameData.Input);

            for (int i = 0; i < lvInput.SelectedIndices.Count; i++)
            {
                FrameData.Input[lvInput.SelectedIndices[i]].Controller[0] = replaceWith;
            }

            updateControlsAfterEdit();
            Msg.AddMsg("Replaced frame(s) on position with " + lvInput.SelectedIndices[0]);
        }

        public void replaceAllItems(string replaceWhat, string replaceWith, bool direction)
        {
            if (Movie == null) return;

            int start = 0;
            int end = FrameData.Input.Length;
            bool search = !(lvInput.SelectedIndices.Count > 1);
            
            UndoBuffer.Add(ref UndoHistory, ref FrameData.Input);

            int totalReplacements = 0;
            if (!search)
            {
                for (int i = 0; i < lvInput.SelectedIndices.Count; i++)
                {
                    for (int j = 0; j < FrameData.Input[i].Controller.Length; j++)
                    {
                        if (FrameData.Input[lvInput.SelectedIndices[i]].Controller[j] != replaceWhat) continue;
                        FrameData.Input[lvInput.SelectedIndices[i]].Controller[j] = replaceWith;
                        ++totalReplacements;
                    }
                }
            }
            else
            {
                for (int i = start; i < end; i++)
                {
                    for (int j = 0; i < FrameData.Input[i].Controller.Length; i++)
                    {
                        if (FrameData.Input[i].Controller[j] != replaceWhat) continue;
                        FrameData.Input[i].Controller[j] = replaceWith;
                        ++totalReplacements;
                    }
                }
            }
            updateControlsAfterEdit();
            MessageBox.Show(this,
                "" + totalReplacements + " cases replaced.",
                "Replace",
                MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
 
            Msg.AddMsg("Replaced frame(s) on position with " + lvInput.SelectedIndices[0]);
        }

        public string getFrameNumberRange()
        {
            return Movie == null ? "no movie" : (FrameData.Input.Length > 0 ? "0-" + (FrameData.Input.Length - 1) : "empty movie");
        }

        public void gotoFrameNumber(string text)
        {
            // if not numeric or no movie loaded
            if (IsNumeric(text) == false || Movie == null) return;

            int targetFrame = Convert.ToInt32(text);

            // check for valid range
            if (targetFrame < FrameData.Input.Length && targetFrame >= 0)
            {
                lvInput.SelectedIndices.Clear();
                lvInput.Items[targetFrame].Selected = true;
                lvInput.Items[targetFrame].Focused = true;
                lvInput.Focus();
                lvInput.EnsureVisible(targetFrame);
            }
        }

        private void findToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frd.SelectedTabIn(0);
            if (frd.Visible)
                frd.Focus();
            else
                frd.Show(this);
            frd.SelectedTabIn(0);
        }

        private void findNextToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (frd.FindOut.Length > 0)
                findNext(frd.FindOut, frd.FindUpOrDownOut);
            else
                findToolStripMenuItem_Click(sender, e);
        }

        private void replaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frd.SelectedTabIn(1);
            if (frd.Visible)
                frd.Focus();
            else
                frd.Show(this);
            frd.SelectedTabIn(1);
        }

        private void goToToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frd.SelectedTabIn(2);
            if (frd.Visible)
                frd.Focus();
            else
                frd.Show(this);
            frd.SelectedTabIn(2);
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < lvInput.Items.Count; i++ )
            {
                lvInput.Items[i].Selected = true;
            }

            lvInput.Select();
        }
        #endregion

        private void runMovieGeneratorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MovieSplicer.Data.moviegenerator.MovieGenerator mg = new MovieSplicer.Data.moviegenerator.MovieGenerator();
            mg.movie = Movie;
            if (!mg.RestoreSavedSession()) mg.LoadConfig();
            if (!mg.Preprocess()) return;
            mg.Generate(this);

            /*
                        MessageBox.Show(this,
                                "",
                                "",
                                MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1);
 
             */
        }

        #region Undo
        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Undo();
        }

        /// <summary>
        /// Undo a change
        /// </summary>        
        private void mnuUndoChange_Click(object sender, EventArgs e)
        {
            Undo();
        }

        private void Undo()
        {
            if (UndoHistory.Changes.Length > 0)
            {
                FrameData.Input = UndoHistory.Changes[UndoHistory.Changes.Length - 1];
                UndoBuffer.Undo(ref UndoHistory);

                lvInput.SelectedIndices.Clear();
                updateControlsAfterEdit();
 
                Msg.AddMsg("Undid last change");
           }
        }
        #endregion

        private void frmMain_Load(object sender, EventArgs e)
        {

        }


        /// <summary>
        /// Show the About dialog
        /// </summary>
        private void mnuInputDescriptions_Click(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Launch the external CHM (Help file)
        /// </summary>        
        private void mnuContent_Click(object sender, EventArgs e)
        {
            if (System.IO.File.Exists("tas-movie-editor.chm"))
                System.Diagnostics.Process.Start("tas-movie-editor.chm");
        }

        /// <summary>
        /// Show the About dialog
        /// </summary>
        private void mnuAbout_Click(object sender, EventArgs e)
        {
            frmAbout frm = new frmAbout();
            frm.ShowDialog();
        }
    }
}
