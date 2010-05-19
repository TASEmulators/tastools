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
    public class TASCompareListView: ListView
    {     
        private const int WM_PAINT = 0xF;

        public TASMovieInputCollection Source;
        public TASMovieInputCollection Target;

        public int SourceOffset;
        public int TargetOffset;

        // Cache items
        private ListViewItem[] cache;
        private int            firstItem;        
        
        /// <summary>
        /// Create a new TASListView object with an event handler on RetriveVirtualItem
        /// </summary>
        public TASCompareListView()
        {
            this.DoubleBuffered = true;
            this.RetrieveVirtualItem += new RetrieveVirtualItemEventHandler(GetVirtualItem);
            this.CacheVirtualItems   += new CacheVirtualItemsEventHandler(CacheVirtualItemsList);
            this.VirtualMode = true;           
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
        /// <param name="columns"></param>
        public void SetColumns()
        {
            if (this.Columns.Count > 0) return;
            
            this.Columns.Clear();
            
            this.Columns.Add("Movie 1", 100);            
            for (int i = 0; i < Source.Controllers; i++)
                this.Columns.Add("Controller " + (i + 1), 200);
            
            this.Columns.Add("Movie 2", 100);            
            for (int j = 0; j < Target.Controllers; j++)
                this.Columns.Add("Controller " + (j + 1), 200);
        }
        
        /// <summary>
        /// Get the index of the specified item
        /// </summary>        
        private void GetVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
            // If we have the item cached, return it. Otherwise, recreate it.
            if (cache != null && e.ItemIndex >= firstItem && e.ItemIndex < firstItem + cache.Length)
                e.Item = cache[e.ItemIndex - firstItem];
            else
                e.Item = GetListItem(e.ItemIndex);
        }

        /// <summary>
        /// Get the item at a specified index
        /// </summary>        
        private ListViewItem GetListItem(int listIndex)
        {
            bool diff = false;
            ListViewItem lv = new ListViewItem();
            //if(VirtualMovieType == TASForm.MovieType.VBM || 
            //   VirtualMovieType == TASForm.MovieType.SMV)
            //        lv = new ListViewItem((listIndex).ToString());
            //else                                
            
            // if we're not out of (source) range, add the frame number
            lv.Text = (listIndex > Source.Input.Length - 1) ? "" : ((listIndex + SourceOffset).ToString());
            
            for (int i = 0; i < Source.Controllers; i++)
            {
                if (listIndex > Source.Input.Length - 1)                             
                    lv.SubItems.Add("");                
                else                             
                    lv.SubItems.Add(Source.Input[listIndex].Controller[i]);                
            }
            
            // if we're not out of (target) range, add the frame number
            if (listIndex > Target.Input.Length - 1)
                lv.SubItems.Add("");
            else
                lv.SubItems.Add((listIndex + TargetOffset).ToString());
            
            for (int j = 0; j < Target.Controllers ; j++)
            {                
                if (listIndex > Target.Input.Length - 1)
                    lv.SubItems.Add("");
                else                
                    lv.SubItems.Add(Target.Input[listIndex].Controller[j]);

                // check if the frames differ
                if (listIndex < Source.Input.Length && listIndex < Target.Input.Length)                                    
                    if (Source.Input[listIndex].Controller[j] != Target.Input[listIndex].Controller[j])
                        diff = true;
            }
                           
            if (listIndex % 2 == 0) lv.BackColor = System.Drawing.Color.BlanchedAlmond;
            if (diff) lv.BackColor = System.Drawing.Color.AliceBlue;

            return lv;
        }

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
