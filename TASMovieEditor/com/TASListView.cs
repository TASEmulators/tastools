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
using System.Resources;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using MovieSplicer.Data;

namespace MovieSplicer.Components
{
    /// <summary>
    /// This is essentially a subclass of System.Windows.Forms.ListView that provides
    /// an auto-expanding last column and virtualization.   
    /// </summary>
    public class TASListView: ListView
    {             
        private const int WM_PAINT = 0xF;        

        public TASMovieInput[]   VirtualListSource;
        public TASForm.MovieType VirtualMovieType;        

        // Cache items
        private ListViewItem[] cache;
        private int            firstItem;

        //private ImageList input;

        /// <summary>
        /// Create a new TASListView object with an event handler on RetriveVirtualItem
        /// </summary>
        public TASListView()
        {
            this.DoubleBuffered = true;
            this.RetrieveVirtualItem += new RetrieveVirtualItemEventHandler(GetVirtualItem);
            this.CacheVirtualItems   += new CacheVirtualItemsEventHandler(CacheVirtualItemsList);
            
            //ResourceManager rm = new ResourceManager("MovieSplicer.Properties.Resources", GetType().Assembly);
            
            //// add our resource images to an imagelist (graphical representations of keys)
            //input = new ImageList();
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("up")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("down")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("left")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("right")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("B")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("A")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("X")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("Y")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("se")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("st")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("L")));
            //input.Images.Add((System.Drawing.Image)(rm.GetObject("R")));

            //rm = null;
        }               
       
        /// <summary>
        // Various message handlers for this control
        /// </summary>
        protected override void WndProc(ref Message message)
        {
            switch (message.Msg)
            {
                // if the control is in details view mode and columns
                // have been added, then intercept the WM_PAINT message
                // and reset the last column width to fill the list view
                case WM_PAINT:
                    if (this.View == View.Details && this.Columns.Count > 0)
                        this.Columns[this.Columns.Count - 1].Width = -2;
                    break;
            }

            // pass messages on to the base control for processing
            base.WndProc(ref message);
        }   
        
    #region Methods

        /// <summary>
        /// Set the number of controller columns to display
        /// 
        /// TODO::Now that the TASMovieInputCollection contains the controller count,
        /// is this necessary anymore?
        /// </summary>       
        public void SetColumns(int columns)
        {
            this.Columns.Clear();
            this.Columns.Add("Frame");

            if (columns == 0) return;

            for (int i = 0; i < columns; i++)
                this.Columns.Add("Controller " + (i + 1), 75);
        }
        
        /// <summary>
        /// Get the index of the specified item
        /// </summary>        
        private void GetVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
//            /*this.BeginUpdate();*/ // EPIC BUG FIX: THIS WAS MAKING MY CPU/GRAPHIC CARD FAN *WHINING*!
            // If we have the item cached, return it. Otherwise, recreate it.
            if (cache != null && e.ItemIndex >= firstItem && e.ItemIndex < firstItem + cache.Length)
                e.Item = cache[e.ItemIndex - firstItem];
            else
                e.Item = GetListItem(e.ItemIndex);
//            /*this.EndUpdate();*/
        }

        /// <summary>
        /// Get the item at a specified index
        /// </summary>        
        private ListViewItem GetListItem(int listIndex)
        {            
            ListViewItem lv;
            if(VirtualMovieType == TASForm.MovieType.VBM || 
               VirtualMovieType == TASForm.MovieType.SMV)
                lv = new ListViewItem((listIndex).ToString());
            else
                lv = new ListViewItem((listIndex + 1).ToString());

            // get each controller used and its input for the frame            
            for (int i = 0; i < this.Columns.Count - 1; i++)
            {
                // DEBUG::handles movie input with null entries in the
                // the main array
                if (VirtualListSource[listIndex] != null)                    
                    lv.SubItems.Add(VirtualListSource[listIndex].Controller[i]);                                                       
                //lv.SubItems.Add(new EXMultipleImagesListViewSubItem(ConvertStringToImage(VirtualListSource[listIndex].Controller[i]))); 
                //else
                //    lv.SubItems.Add("");
            }
               
            if (listIndex % 2 == 0) lv.BackColor = System.Drawing.Color.BlanchedAlmond;
            
            return lv;
        }

        /// <summary>
        /// Lookup key image in imagelist
        /// 
        /// NOTE::I don't really like arraylists, but I didn't write the ExtendedListView class,
        /// and seeing as the author did a really good job, there's no need to re-write it ... for now :P
        /// </summary>        
        //private ArrayList ConvertStringToImage(string controller)
        //{            
        //    ArrayList arr = new ArrayList();
        //    if (controller == null) return arr;
        //    if (controller.Contains("^")) arr.Add(input.Images[0]);
        //    if (controller.Contains("v")) arr.Add(input.Images[1]);
        //    if (controller.Contains("<")) arr.Add(input.Images[2]);
        //    if (controller.Contains(">")) arr.Add(input.Images[3]);
        //    if (controller.Contains("B")) arr.Add(input.Images[4]);
        //    if (controller.Contains("A")) arr.Add(input.Images[5]);
        //    if (controller.Contains("X")) arr.Add(input.Images[6]);
        //    if (controller.Contains("Y")) arr.Add(input.Images[7]);
        //    if (controller.Contains("s")) arr.Add(input.Images[8]);
        //    if (controller.Contains("S")) arr.Add(input.Images[9]);
        //    if (controller.Contains("L")) arr.Add(input.Images[10]);
        //    if (controller.Contains("R")) arr.Add(input.Images[11]);

        //    return arr;
        //}

        /// <summary>
        /// Clear the virtual cache (duh :P)
        /// </summary>
        public void ClearVirtualCache()
        {
            cache = null;
        }

        /// <summary>
        /// Cache current view
        /// </summary>        
        private void CacheVirtualItemsList(object sender, CacheVirtualItemsEventArgs e)
        {
            // Only recreate the cache if we need to.
            if (cache != null && e.StartIndex >= firstItem && e.EndIndex <= firstItem + cache.Length)
                return;

            firstItem  = e.StartIndex;
            int length = e.EndIndex - e.StartIndex + 1;
            cache = new ListViewItem[length];

            for (int i = 0; i < cache.Length; i++) 
                cache[i] = GetListItem(firstItem + i);
        }

    #endregion

    }
}
