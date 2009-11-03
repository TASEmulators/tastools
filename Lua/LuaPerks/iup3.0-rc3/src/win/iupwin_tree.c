/** \file
 * \brief Tree Control
 *
 * See Copyright Notice in iup.h
 */

#undef NOTREEVIEW
#include <windows.h>
#include <commctrl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_drv.h"
#include "iup_stdcontrols.h"
#include "iup_tree.h"
#include "iup_image.h"
#include "iup_array.h"

#include "iupwin_drv.h"
#include "iupwin_handle.h"
#include "iupwin_draw.h"
#include "iupwin_info.h"

typedef struct _winTreeItemData
{
  COLORREF color;
  unsigned char kind;
  void* userdata;
  HFONT hFont;
  short image;
  short image_expanded;
} winTreeItemData;

#ifndef TVN_ITEMCHANGING                          /* Vista Only */
typedef struct tagNMTVITEMCHANGE {
    NMHDR hdr;
    UINT uChanged;
    HTREEITEM hItem;
    UINT uStateNew;
    UINT uStateOld;
    LPARAM lParam;
} NMTVITEMCHANGE;
#define TVN_ITEMCHANGINGA        (TVN_FIRST-16)    
#define TVN_ITEMCHANGINGW        (TVN_FIRST-17)    
#endif

static void winTreeSetFocusNode(Ihandle* ih, HTREEITEM hItem);
typedef int (*winTreeNodeFunc)(Ihandle* ih, HTREEITEM hItem, void* userdata);

static int winTreeForEach(Ihandle* ih, HTREEITEM hItem, winTreeNodeFunc func, void* userdata)
{
  HTREEITEM hItemChild;

  if (!hItem)
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);

  while(hItem != NULL)
  {
    if (!func(ih, hItem, userdata))
      return 0;

    hItemChild = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
    if (hItemChild)
    {
      /* Recursively traverse child items */
      if (!winTreeForEach(ih, hItemChild, func, userdata))
        return 0;
    }

    /* Go to next sibling item */
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
  }

  return 1;
}

/*****************************************************************************/
/* FINDING ITEMS                                                             */
/*****************************************************************************/
static HTREEITEM winTreeFindNodeID(Ihandle* ih, HTREEITEM hItem, HTREEITEM hNode)
{
  TVITEM item;
  winTreeItemData* itemData;

  while(hItem != NULL)
  {
    /* ID control to traverse items */
    ih->data->id_control++;

    /* StateID founded! */
    if(hItem == hNode)
      return hItem;

    /* Get hItem attributes */
    item.hItem = hItem;
    item.mask = TVIF_HANDLE|TVIF_PARAM;
    SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
    itemData = (winTreeItemData*)item.lParam;

    /* Check whether we have child items */
    if (itemData->kind == ITREE_BRANCH)
    {
      /* Recursively traverse child items */
      HTREEITEM hItemChild = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
      hItemChild = winTreeFindNodeID(ih, hItemChild, hNode);

      /* StateID founded! */
      if(hItemChild)
        return hItemChild;
    }
    /* Go to next sibling item */
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
  }

  return NULL;
}

static int winTreeGetNodeId(Ihandle* ih, HTREEITEM hItem)
{
  HTREEITEM hItemRoot = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
  ih->data->id_control = -1;
  if (winTreeFindNodeID(ih, hItemRoot, hItem))
    return ih->data->id_control;
  else
    return -1;
}

static HTREEITEM winTreeFindUserDataID(Ihandle* ih, HTREEITEM hItem, void* userdata)
{
  TVITEM item;
  winTreeItemData* itemData;

  while(hItem != NULL)
  {
    /* ID control to traverse items */
    ih->data->id_control++;

    /* Get hItem attributes */
    item.hItem = hItem;
    item.mask = TVIF_HANDLE|TVIF_PARAM;
    SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
    itemData = (winTreeItemData*)item.lParam;

    /* userdata founded! */
    if(itemData->userdata == userdata)
      return hItem;

    /* Check whether we have child items */
    if (itemData->kind == ITREE_BRANCH)
    {
      /* Recursively traverse child items */
      HTREEITEM hItemChild = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
      hItemChild = winTreeFindUserDataID(ih, hItemChild, userdata);

      /* userdata founded! */
      if (hItemChild)
        return hItemChild;
    }

    /* Go to next sibling item */
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
  }

  return NULL;
}

static int winTreeGetUserDataId(Ihandle* ih, void* userdata)
{
  HTREEITEM hItemRoot = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
  ih->data->id_control = -1;
  if (winTreeFindUserDataID(ih, hItemRoot, userdata))
    return ih->data->id_control;
  else
    return -1;
}

static HTREEITEM winTreeFindNodeFromID(Ihandle* ih, HTREEITEM hItem)
{
  TVITEM item;
  winTreeItemData* itemData;

  while(hItem != NULL)
  {
    /* ID control to traverse items */
    ih->data->id_control--;

    /* StateID founded! */
    if(ih->data->id_control < 0)
      return hItem;

    /* Get hItem attributes */
    item.hItem = hItem;
    item.mask = TVIF_HANDLE|TVIF_PARAM;
    SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
    itemData = (winTreeItemData*)item.lParam;

    /* Check whether we have child items */
    if (itemData->kind == ITREE_BRANCH)
    {
      /* Recursively traverse child items */
      HTREEITEM hItemChild = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
      hItemChild = winTreeFindNodeFromID(ih, hItemChild);

      /* StateID founded! */
      if(ih->data->id_control < 0)
        return hItemChild;
    }

    /* Go to next sibling item */
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
  }

  return hItem;
}

static HTREEITEM winTreeFindNodeFromString(Ihandle* ih, const char* name_id)
{
  if (name_id[0])
  {
    HTREEITEM hRoot = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
    iupStrToInt(name_id, &ih->data->id_control);
    return winTreeFindNodeFromID(ih, hRoot);
  }
  else
    return (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CARET, 0);
}

/* Recursively, find a new brother for the item
   that will have its depth changed. Returns the new brother. */
static HTREEITEM winTreeFindNewBrother(Ihandle* ih, HTREEITEM hBrotherItem)
{
  TVITEM item;
  winTreeItemData* itemData;

  while(hBrotherItem != NULL)
  {
    if(ih->data->id_control < 0)
      return hBrotherItem;

    item.hItem = hBrotherItem;
    item.mask = TVIF_HANDLE|TVIF_PARAM;
    SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
    itemData = (winTreeItemData*)item.lParam;

    if (itemData->kind == ITREE_BRANCH)
    {
      HTREEITEM hItemChild;

      ih->data->id_control--;
      hItemChild = winTreeFindNewBrother(ih, (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hBrotherItem));

      if(ih->data->id_control < 0)
        return hItemChild;
    }

    hBrotherItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hBrotherItem);
  }

  return hBrotherItem;
}

static HTREEITEM winTreeFindNodePointed(Ihandle* ih)
{
  TVHITTESTINFO info;
  DWORD pos = GetMessagePos();
  info.pt.x = LOWORD(pos);
  info.pt.y = HIWORD(pos);

  ScreenToClient(ih->handle, &info.pt);
  
  return (HTREEITEM)SendMessage(ih->handle, TVM_HITTEST, 0, (LPARAM)(LPTVHITTESTINFO)&info);
}

int iupwinGetColor(const char* value, COLORREF *color)
{
  unsigned char r, g, b;
  if (iupStrToRGB(value, &r, &g, &b))
  {
    *color = RGB(r,g,b);
    return 1;
  }
  return 0;
}

/*****************************************************************************/
/* ADDING ITEMS                                                              */
/*****************************************************************************/
void iupdrvTreeAddNode(Ihandle* ih, const char* name_id, int kind, const char* title, int add)
{
  TVITEM item, tviPrevItem;
  TVINSERTSTRUCT tvins;
  HTREEITEM hPrevItem = winTreeFindNodeFromString(ih, name_id);
  int kindPrev;
  winTreeItemData* itemData;

  if (!hPrevItem)
    return;

  itemData = calloc(1, sizeof(winTreeItemData));
  itemData->image = -1;
  itemData->image_expanded = -1;
  itemData->kind = (unsigned char)kind;

  item.mask = TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT; 
  item.pszText = (char*)title;
  item.lParam = (LPARAM)itemData;

  iupwinGetColor(iupAttribGetStr(ih, "FGCOLOR"), &itemData->color);

  if (kind == ITREE_BRANCH)
  {
    item.iSelectedImage = item.iImage = (int)ih->data->def_image_collapsed;

    if (ih->data->add_expanded)
    {
      item.mask |= TVIF_STATE;
      item.state = item.stateMask = TVIS_EXPANDED;
      item.iSelectedImage = item.iImage = (int)ih->data->def_image_expanded;
    }
  }
  else
    item.iSelectedImage = item.iImage = (int)ih->data->def_image_leaf;

  /* Save the heading level in the node's application-defined data area */
  tvins.item = item;

  /* get the KIND attribute of node selected */ 
  tviPrevItem.hItem = hPrevItem;
  tviPrevItem.mask = TVIF_PARAM|TVIF_CHILDREN; 
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&tviPrevItem);
  kindPrev = ((winTreeItemData*)tviPrevItem.lParam)->kind;

  /* Define the parent and the position to the new node inside
     the list, using the KIND attribute of node selected */
  if (kindPrev == ITREE_BRANCH && add)
  {
    tvins.hParent = hPrevItem;
    tvins.hInsertAfter = TVI_FIRST;   /* insert the new node after item selected, as first child */
  }
  else
  {
    tvins.hParent = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hPrevItem);
    tvins.hInsertAfter = hPrevItem;   /* insert the new node after item selected */
  }

  /* Add the node to the tree-view control */
  SendMessage(ih->handle, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

  if (kindPrev == ITREE_BRANCH && tviPrevItem.cChildren==0)
  {
    /* this is the first child, redraw to update the '+'/'-' buttons */
    iupdrvDisplayRedraw(ih);
  }
}

static void winTreeAddRootNode(Ihandle* ih)
{
  TVITEM item; 
  TVINSERTSTRUCT tvins;
  HTREEITEM hNewItem;
  winTreeItemData* itemData;

  itemData = calloc(1, sizeof(winTreeItemData));
  itemData->image = -1;
  itemData->image_expanded = -1;
  itemData->kind = ITREE_BRANCH;

  item.mask = TVIF_PARAM | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
  item.state = item.stateMask = TVIS_EXPANDED;
  item.iSelectedImage = item.iImage = (int)ih->data->def_image_expanded;
  item.lParam = (LPARAM)itemData;

  iupwinGetColor(iupAttribGetStr(ih, "FGCOLOR"), &itemData->color);

  /* Save the heading level in the node's application-defined data area */
  tvins.item = item; 
  tvins.hInsertAfter = TVI_FIRST;
  tvins.hParent = TVI_ROOT;

  /* Add the node to the tree-view control */
  hNewItem = (HTREEITEM)SendMessage(ih->handle, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

  /* MarkStart node */
  iupAttribSetStr(ih, "_IUPTREE_MARKSTART_NODE", (char*)hNewItem);

  /* Set the default VALUE */
  winTreeSetFocusNode(ih, hNewItem);
}

static int winTreeIsItemExpanded(Ihandle* ih, HTREEITEM hItem)
{
  TVITEM item;
  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_STATE;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  return (item.state & TVIS_EXPANDED) != 0;
}

static void winTreeExpandItem(Ihandle* ih, HTREEITEM hItem, int expand)
{
  if (expand == -1)
    expand = !winTreeIsItemExpanded(ih, hItem); /* toggle */

  if (expand)
    SendMessage(ih->handle, TVM_EXPAND, TVE_EXPAND, (LPARAM)hItem);
  else
    SendMessage(ih->handle, TVM_EXPAND, TVE_COLLAPSE, (LPARAM)hItem);
}

/*****************************************************************************/
/* EXPANDING AND STORING ITEMS                                               */
/*****************************************************************************/
static void winTreeExpandTree(Ihandle* ih, HTREEITEM hItem, int expand)
{
  HTREEITEM hItemChild;
  while(hItem != NULL)
  {
    hItemChild = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);

    /* Check whether we have child items */
    if (hItemChild)
    {
      winTreeExpandItem(ih, hItem, expand);

      /* Recursively traverse child items */
      winTreeExpandTree(ih, hItemChild, expand);
    }

    /* Go to next sibling item */
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
  }
}

/*****************************************************************************/
/* SELECTING ITEMS                                                           */
/*****************************************************************************/

static int winTreeIsItemSelected(Ihandle* ih, HTREEITEM hItem)
{
  return ((SendMessage(ih->handle, TVM_GETITEMSTATE, (WPARAM)hItem, TVIS_SELECTED)) & TVIS_SELECTED)!=0;
}

static void winTreeSelectItem(Ihandle* ih, HTREEITEM hItem, int select)
{
  TV_ITEM item;
  item.mask = TVIF_STATE | TVIF_HANDLE;
  item.stateMask = TVIS_SELECTED;
  item.hItem = hItem;

  if (select == -1)
    select = !winTreeIsItemSelected(ih, hItem);

  item.state = select ? TVIS_SELECTED : 0;

  SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)&item);
}

static HTREEITEM winTreeGetFocusNode(Ihandle* ih)
{
  return (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CARET, 0);
}

/* ------------Comment from wxWidgets--------------------
   Helper function which tricks the standard control into changing the focused
   item without changing anything else. */
static void winTreeSetFocusNode(Ihandle* ih, HTREEITEM hItem)
{
  HTREEITEM hItemFocus = winTreeGetFocusNode(ih);
  if (hItem != hItemFocus)
  {
    /* remember the selection state of the item */
    int wasSelected = winTreeIsItemSelected(ih, hItem);
    int wasFocusSelected = 0;

    if (iupwinIsVista())
      iupAttribSetStr(ih, "_IUPTREE_ALLOW_CHANGE", (char*)hItem);  /* Vista Only */
    else
      wasFocusSelected = hItemFocus && winTreeIsItemSelected(ih, hItemFocus);

    iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", "1");

    if (wasFocusSelected)
    {
      /* prevent the tree from unselecting the old focus which it would do by default */
      SendMessage(ih->handle, TVM_SELECTITEM, TVGN_CARET, (LPARAM)NULL); /* remove the focus */
      winTreeSelectItem(ih, hItemFocus, 1); /* select again */
    }

    SendMessage(ih->handle, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem); /* set focus, selection, and unselect the previous focus */

    if (!wasSelected)
      winTreeSelectItem(ih, hItem, 0); /* need to clear the selection if was not selected */

    iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", NULL);
    iupAttribSetStr(ih, "_IUPTREE_ALLOW_CHANGE", NULL);
  }
}

typedef struct _winTreeRange{
  HTREEITEM hItem1, hItem2;
  char inside, clear;
}winTreeRange;

static int winTreeSelectRangeFunc(Ihandle* ih, HTREEITEM hItem, winTreeRange* range)
{
  int end_range = 0;

  if (range->inside == 0) /* detect the range start */
  {
    if (range->hItem1 == hItem) range->inside=1;
    else if (range->hItem2 == hItem) range->inside=1;
  }
  else if (range->inside == 1) /* detect the range end */
  {
    if (range->hItem1 == hItem) end_range=1;
    else if (range->hItem2 == hItem) end_range=1;
  }

  if (range->inside == 1) /* if inside, select */
    winTreeSelectItem(ih, hItem, 1);
  else if (range->clear)  /* if outside and clear, unselect */
    winTreeSelectItem(ih, hItem, 0);

  if (end_range || (range->inside && range->hItem1==range->hItem2))
    range->inside=-1;  /* update after selecting the node */

  return 1;
}

static void winTreeSelectRange(Ihandle* ih, HTREEITEM hItemFrom, HTREEITEM hItemTo, int clear)
{
  winTreeRange range;
  range.hItem1 = hItemFrom;
  range.hItem2 = hItemTo;
  range.inside = 0;
  range.clear = (char)clear;
  winTreeForEach(ih, NULL, (winTreeNodeFunc)winTreeSelectRangeFunc, &range);
}

static void winTreeSelectAll(Ihandle* ih)
{
  HTREEITEM hItemRoot = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
  winTreeSelectRange(ih, hItemRoot, NULL, 0);
}

static void winTreeClearSelection(Ihandle* ih, HTREEITEM hItemExcept)
{
  winTreeSelectRange(ih, hItemExcept, hItemExcept, 1);
}

static int winTreeInvertSelectFunc(Ihandle* ih, HTREEITEM hItem, void* userdata)
{
  winTreeSelectItem(ih, hItem, -1);
  (void)userdata;
  return 1;
}

typedef struct _winTreeSelArray{
  Iarray* markedArray;
  char is_handle;
  int id_control;
}winTreeSelArray;

static int winTreeSelectedArrayFunc(Ihandle* ih, HTREEITEM hItem, winTreeSelArray* selarray)
{                                                                                  
  selarray->id_control++;

  if (winTreeIsItemSelected(ih, hItem))
  {
    if (selarray->is_handle)
    {
      HTREEITEM* hItemArrayData = (HTREEITEM*)iupArrayInc(selarray->markedArray);
      int i = iupArrayCount(selarray->markedArray);
      hItemArrayData[i-1] = hItem;
    }
    else
    {
      int* intArrayData = (int*)iupArrayInc(selarray->markedArray);
      int i = iupArrayCount(selarray->markedArray);
      intArrayData[i-1] = selarray->id_control;
    }
  }

  return 1;
}

static Iarray* winTreeGetSelectedArray(Ihandle* ih)
{
  Iarray* markedArray = iupArrayCreate(1, sizeof(HTREEITEM));
  winTreeSelArray selarray;
  selarray.markedArray = markedArray;
  selarray.id_control = -1;
  selarray.is_handle = 1;

  winTreeForEach(ih, NULL, (winTreeNodeFunc)winTreeSelectedArrayFunc, &selarray);

  return markedArray;
}

static Iarray* winTreeGetSelectedArrayId(Ihandle* ih)
{
  Iarray* markedArray = iupArrayCreate(1, sizeof(int));
  winTreeSelArray selarray;
  selarray.markedArray = markedArray;
  selarray.id_control = -1;
  selarray.is_handle = 0;

  winTreeForEach(ih, NULL, (winTreeNodeFunc)winTreeSelectedArrayFunc, &selarray);

  return markedArray;
}


/*****************************************************************************/
/* COPYING ITEMS (Branches and its children)                                 */
/*****************************************************************************/
/* Insert the copied item in a new location. Returns the new item. */
static HTREEITEM winTreeCopyItem(Ihandle* ih, HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hPosition, int full_copy)
{
  TVITEM item; 
  TVINSERTSTRUCT tvins;
  char* title = iupStrGetMemory(255);

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_STATE | TVIF_PARAM;
  item.pszText = title;
  item.cchTextMax = 255;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);

  if (full_copy) /* during a full copy the userdata reference is not copied */
  {
    /* create a new one */
    winTreeItemData* itemDataNew = malloc(sizeof(winTreeItemData));
    memcpy(itemDataNew, (void*)item.lParam, sizeof(winTreeItemData));
    itemDataNew->userdata = NULL;
    item.lParam = (LPARAM)itemDataNew;
  }

  /* Copy everything including user data reference */
  tvins.item = item; 
  tvins.hInsertAfter = hPosition;
  tvins.hParent = hParent;

  /* Add the node to the tree-view control */
  return (HTREEITEM)SendMessage(ih->handle, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
}

static void winTreeCopyChildren(Ihandle* ih, HTREEITEM hItemSrc, HTREEITEM hItemDst, int full_copy)
{
  HTREEITEM hChildSrc = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItemSrc);
  HTREEITEM hNewItem = TVI_FIRST;
  while (hChildSrc != NULL)
  {
    hNewItem = winTreeCopyItem(ih, hChildSrc, hItemDst, hNewItem, full_copy); /* Use the same order they where enumerated */

    /* Recursively transfer all the items */
    winTreeCopyChildren(ih, hChildSrc, hNewItem, full_copy);  

    /* Go to next sibling item */
    hChildSrc = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hChildSrc);
  }
}

/* Copies all items in a branch to a new location. Returns the new branch node. */
static HTREEITEM winTreeCopyNode(Ihandle* ih, HTREEITEM hItemSrc, HTREEITEM hItemDst, int full_copy)
{
  HTREEITEM hNewItem, hParent;
  TVITEM item;
  winTreeItemData* itemDataDst;

  /* Get DST node attributes */
  item.hItem = hItemDst;
  item.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_STATE;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemDataDst = (winTreeItemData*)item.lParam;

  if (itemDataDst->kind == ITREE_BRANCH && (item.state & TVIS_EXPANDED))
  {
    /* copy as first child of expanded branch */
    hParent = hItemDst;
    hItemDst = TVI_FIRST;
  }
  else
  {                                                    
    /* copy as next brother of item or collapsed branch */
    hParent = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItemDst);
  }

  hNewItem = winTreeCopyItem(ih, hItemSrc, hParent, hItemDst, full_copy);

  winTreeCopyChildren(ih, hItemSrc, hNewItem, full_copy);

  return hNewItem;
}

/*****************************************************************************/
/* MANIPULATING IMAGES                                                       */
/*****************************************************************************/
static void winTreeUpdateImages(Ihandle* ih, HTREEITEM hItem, int mode)
{
  HTREEITEM hItemChild;
  TVITEM item;
  winTreeItemData* itemData;

  /* called when one of the default images is changed */

  while(hItem != NULL)
  {
    /* Get node attributes */
    item.hItem = hItem;
    item.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE;
    SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
    itemData = (winTreeItemData*)item.lParam;

    if (itemData->kind == ITREE_BRANCH)
    {
      if (item.state & TVIS_EXPANDED)
      {
        if (mode == ITREE_UPDATEIMAGE_EXPANDED)
        {
          item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
          item.iSelectedImage = item.iImage = (itemData->image_expanded!=-1)? itemData->image_expanded: (int)ih->data->def_image_expanded;
          SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(LPTVITEM)&item);
        }
      }
      else
      {
        if (mode == ITREE_UPDATEIMAGE_COLLAPSED)
        {
          item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
          item.iSelectedImage = item.iImage = (itemData->image!=-1)? itemData->image: (int)ih->data->def_image_collapsed;
          SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(LPTVITEM)&item);
        }
      }

      /* Recursively traverse child items */
      hItemChild = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
      winTreeUpdateImages(ih, hItemChild, mode);
    }
    else
    {
      if (mode == ITREE_UPDATEIMAGE_LEAF)
      {
        item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        item.iSelectedImage = item.iImage = (itemData->image!=-1)? itemData->image: (int)ih->data->def_image_leaf;
        SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(LPTVITEM)&item);
      }
    }

    /* Go to next sibling node */
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
  }
}

static int winTreeGetImageIndex(Ihandle* ih, const char* name)
{
  HIMAGELIST image_list;
  int count, i;
  Iarray* bmpArray;
  HBITMAP *bmpArrayData;
  HBITMAP bmp = iupImageGetImage(name, ih, 0);
  if (!bmp)
    return -1;

  /* the array is used to avoi adding the same bitmap twice */
  bmpArray = (Iarray*)iupAttribGet(ih, "_IUPWIN_BMPARRAY");
  if (!bmpArray)
  {
    /* create the array if does not exist */
    bmpArray = iupArrayCreate(50, sizeof(HBITMAP));
    iupAttribSetStr(ih, "_IUPWIN_BMPARRAY", (char*)bmpArray);
  }

  bmpArrayData = iupArrayGetData(bmpArray);

  image_list = (HIMAGELIST)SendMessage(ih->handle, TVM_GETIMAGELIST, TVSIL_NORMAL, 0);
  if (!image_list)
  {
    int width, height;

    /* must use this info, since image can be a driver image loaded from resources */
    iupdrvImageGetInfo(bmp, &width, &height, NULL);

    /* create the image list if does not exist */
    image_list = ImageList_Create(width, height, ILC_COLOR32, 0, 50);
    SendMessage(ih->handle, TVM_SETIMAGELIST, 0, (LPARAM)image_list);
  }

  /* check if that bitmap is already added to the list,
     but we can not compare with the actual bitmap at the list since it is a copy */
  count = ImageList_GetImageCount(image_list);
  for (i = 0; i < count; i++)
  {
    if (bmpArrayData[i] == bmp)
      return i;
  }

  bmpArrayData = iupArrayInc(bmpArray);
  bmpArrayData[i] = bmp;
  return ImageList_Add(image_list, bmp, NULL);  /* the bmp is duplicated at the list */
}


/*****************************************************************************/
/* CALLBACKS                                                                 */
/*****************************************************************************/

static int winTreeCallBranchLeafCb(Ihandle* ih, HTREEITEM hItem)
{
  TVITEM item;
  winTreeItemData* itemData;

  /* Get Children: branch or leaf */
  item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_STATE; 
  item.hItem = hItem;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  if (itemData->kind == ITREE_BRANCH)
  {
    if (item.state & TVIS_EXPANDED)
    {
      IFni cbBranchClose = (IFni)IupGetCallback(ih, "BRANCHCLOSE_CB");
      if (cbBranchClose)
        return cbBranchClose(ih, winTreeGetNodeId(ih, hItem));
    }
    else
    {
      IFni cbBranchOpen  = (IFni)IupGetCallback(ih, "BRANCHOPEN_CB");
      if (cbBranchOpen)
        return cbBranchOpen(ih, winTreeGetNodeId(ih, hItem));
    }
  }
  else
  {
    IFni cbExecuteLeaf = (IFni)IupGetCallback(ih, "EXECUTELEAF_CB");
    if (cbExecuteLeaf)
      return cbExecuteLeaf(ih, winTreeGetNodeId(ih, hItem));
  }

  return IUP_DEFAULT;
}

static void winTreeCallMultiSelectionCb(Ihandle* ih)
{
  IFnIi cbMulti = (IFnIi)IupGetCallback(ih, "MULTISELECTION_CB");
  if(cbMulti)
  {
    Iarray* markedArray = winTreeGetSelectedArrayId(ih);
    int* id_hitem = (int*)iupArrayGetData(markedArray);

    cbMulti(ih, id_hitem, iupArrayCount(markedArray));

    iupArrayDestroy(markedArray);
  }
  else
  {
    IFnii cbSelec = (IFnii)IupGetCallback(ih, "SELECTION_CB");
    if (cbSelec)
    {
      Iarray* markedArray = winTreeGetSelectedArrayId(ih);
      int* id_hitem = (int*)iupArrayGetData(markedArray);
      int i, count = iupArrayCount(markedArray);

      for (i=0; i<count; i++)
        cbSelec(ih, id_hitem[i], 1);

      iupArrayDestroy(markedArray);
    }
  }
}

static void winTreeCallSelectionCb(Ihandle* ih, int status, HTREEITEM hItem)
{
  IFnii cbSelec = (IFnii)IupGetCallback(ih, "SELECTION_CB");
  if (cbSelec)
  {
    if (ih->data->mark_mode == ITREE_MARK_MULTIPLE && IupGetCallback(ih,"MULTISELECTION_CB"))
    {
      if ((GetKeyState(VK_SHIFT) & 0x8000))
        return;
    }

    if (iupAttribGet(ih, "_IUPTREE_IGNORE_SELECTION_CB"))
      return;
    
    cbSelec(ih, winTreeGetNodeId(ih, hItem), status);
  }
}

static int winTreeCallDragDropCb(Ihandle* ih, HTREEITEM	hItemDrag, HTREEITEM hItemDrop, int *is_ctrl)
{
  IFniiii cbDragDrop = (IFniiii)IupGetCallback(ih, "DRAGDROP_CB");
  int is_shift = 0;
  if ((GetKeyState(VK_SHIFT) & 0x8000))
    is_shift = 1;
  if ((GetKeyState(VK_CONTROL) & 0x8000))
    *is_ctrl = 1;
  else
    *is_ctrl = 0;

  if (cbDragDrop)
  {
    int drag_id = winTreeGetNodeId(ih, hItemDrag);
    int drop_id = winTreeGetNodeId(ih, hItemDrop);
    return cbDragDrop(ih, drag_id, drop_id, is_shift, *is_ctrl);
  }

  return IUP_CONTINUE; /* allow to move by default if callback not defined */
}


/*****************************************************************************/
/* GET AND SET ATTRIBUTES                                                    */
/*****************************************************************************/


static int winTreeSetImageBranchExpandedAttrib(Ihandle* ih, const char* value)
{
  ih->data->def_image_expanded = (void*)winTreeGetImageIndex(ih, value);

  /* Update all images, starting at root node */
  winTreeUpdateImages(ih, (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0), ITREE_UPDATEIMAGE_EXPANDED);

  return 1;
}

static int winTreeSetImageBranchCollapsedAttrib(Ihandle* ih, const char* value)
{
  ih->data->def_image_collapsed = (void*)winTreeGetImageIndex(ih, value);

  /* Update all images, starting at root node */
  winTreeUpdateImages(ih, (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0), ITREE_UPDATEIMAGE_COLLAPSED);

  return 1;
}

static int winTreeSetImageLeafAttrib(Ihandle* ih, const char* value)
{
  ih->data->def_image_leaf = (void*)winTreeGetImageIndex(ih, value);

  /* Update all images, starting at root node */
  winTreeUpdateImages(ih, (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0), ITREE_UPDATEIMAGE_LEAF);

  return 1;
}

static int winTreeSetImageExpandedAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  TVITEM item;
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;
  itemData->image_expanded = (short)winTreeGetImageIndex(ih, value);

  if (itemData->kind == ITREE_BRANCH && item.state & TVIS_EXPANDED)
  {
    if (itemData->image_expanded == -1)
      item.iSelectedImage = item.iImage = (int)ih->data->def_image_expanded;
    else
      item.iSelectedImage = item.iImage = itemData->image_expanded;
    item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&item);
  }

  return 1;
}

static int winTreeSetImageAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  TVITEM item;
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;
  itemData->image = (short)winTreeGetImageIndex(ih, value);

  if (itemData->kind == ITREE_BRANCH)
  {
    if (!(item.state & TVIS_EXPANDED))
    {
      if (itemData->image == -1)
        item.iSelectedImage = item.iImage = (int)ih->data->def_image_collapsed;
      else
        item.iSelectedImage = item.iImage = itemData->image;

      item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
      SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&item);
    }
  }
  else
  {
    if (itemData->image == -1)
      item.iSelectedImage = item.iImage = (int)ih->data->def_image_leaf;
    else
      item.iSelectedImage = item.iImage = itemData->image;

    item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&item);
  }

  return 1;
}

static int winTreeSetTopItemAttrib(Ihandle* ih, const char* value)
{
  HTREEITEM hItem = winTreeFindNodeFromString(ih, value);
  if (hItem)
    SendMessage(ih->handle, TVM_ENSUREVISIBLE, 0, (LPARAM)hItem);
  return 0;
}

static int winTreeSetSpacingAttrib(Ihandle* ih, const char* value)
{
  if (!iupStrToInt(value, &ih->data->spacing))
    ih->data->spacing = 1;

  if(ih->data->spacing < 1)
    ih->data->spacing = 1;

  if (ih->handle)
  {
    int old_spacing = iupAttribGetInt(ih, "_IUPWIN_OLDSPACING");
    int height = SendMessage(ih->handle, TVM_GETITEMHEIGHT, 0, 0);
    height -= 2*old_spacing;
    height += 2*ih->data->spacing;
    SendMessage(ih->handle, TVM_SETITEMHEIGHT, height, 0);
    iupAttribSetInt(ih, "_IUPWIN_OLDSPACING", ih->data->spacing);
    return 0;
  }
  else
    return 1; /* store until not mapped, when mapped will be set again */
}

static int winTreeSetExpandAllAttrib(Ihandle* ih, const char* value)
{
  HTREEITEM hItemRoot = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
  HTREEITEM hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItemRoot); /* skip the root node that is always expanded */
  int expand = iupStrBoolean(value);
  winTreeExpandTree(ih, hItem, expand);
  return 0;
}

static int winTreeSetBgColorAttrib(Ihandle* ih, const char* value)
{
  unsigned char r, g, b;
  if (iupStrToRGB(value, &r, &g, &b))
  {
    COLORREF cr = RGB(r,g,b);
    SendMessage(ih->handle, TVM_SETBKCOLOR, 0, (LPARAM)cr);

    /* update internal image cache */
    iupTreeUpdateImages(ih);
  }
  return 0;
}

static char* winTreeGetBgColorAttrib(Ihandle* ih)
{
  COLORREF cr = (COLORREF)SendMessage(ih->handle, TVM_GETBKCOLOR, 0, 0);
  if (cr == (COLORREF)-1)
    return IupGetGlobal("TXTBGCOLOR");
  else
  {
    char* str = iupStrGetMemory(20);
    sprintf(str, "%d %d %d", (int)GetRValue(cr), (int)GetGValue(cr), (int)GetBValue(cr));
    return str;
  }
}

static void winTreeSetRenameCaretPos(HWND hEdit, const char* value)
{
  int pos = 1;

  if (iupStrToInt(value, &pos))
  {
    if (pos < 1) pos = 1;
    pos--; /* IUP starts at 1 */

    SendMessage(hEdit, EM_SETSEL, (WPARAM)pos, (LPARAM)pos);
  }
}

static void winTreeSetRenameSelectionPos(HWND hEdit, const char* value)
{
  int start = 1, end = 1;

  if (iupStrToIntInt(value, &start, &end, ':') != 2) 
    return;

  if(start < 1 || end < 1) 
    return;

  start--; /* IUP starts at 1 */
  end--;

  SendMessage(hEdit, EM_SETSEL, (WPARAM)start, (LPARAM)end);
}

static char* winTreeGetTitle(Ihandle* ih, HTREEITEM hItem)
{
  TVITEM item;
  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_TEXT; 
  item.pszText = iupStrGetMemory(255);
  item.cchTextMax = 255;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  return item.pszText;
}

static char* winTreeGetTitleAttrib(Ihandle* ih, const char* name_id)
{
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;
  return winTreeGetTitle(ih, hItem);
}

static int winTreeSetTitleAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  TVITEM item; 
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_TEXT; 
  item.pszText = (char*)value;
  SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&item);
  return 0;
}

static char* winTreeGetFindUserDataAttrib(Ihandle* ih, const char* name_id)
{
  int id;
  char* str = (char*)(name_id+1); /* skip ':' */
  void* userdata = NULL;
  if (sscanf(str, "%p", &userdata)!=1)
    return NULL;
  id = winTreeGetUserDataId(ih, userdata);
  if (id == -1)
    return NULL;
  str = iupStrGetMemory(16);
  sprintf(str, "%d", id);
  return str;
}

static char* winTreeGetUserDataAttrib(Ihandle* ih, const char* name_id)
{
  TVITEM item;
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  return itemData->userdata;
}

static int winTreeSetUserDataAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  TVITEM item; 
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  itemData->userdata = (void*)value;

  return 0;
}

static char* winTreeGetTitleFontAttrib(Ihandle* ih, const char* name_id)
{
  TVITEM item;
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  return iupwinFindHFont(itemData->hFont);
}

static int winTreeSetTitleFontAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  TVITEM item; 
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  if (value)
    itemData->hFont = iupwinGetHFont(value);
  else
    itemData->hFont = NULL;

  iupdrvDisplayUpdate(ih);

  return 0;
}

static char* winTreeGetIndentationAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(255);
  int indent = (int)SendMessage(ih->handle, TVM_GETINDENT, 0, 0);
  sprintf(str, "%d", indent);
  return str;
}

static int winTreeSetIndentationAttrib(Ihandle* ih, const char* value)
{
  int indent;
  if (iupStrToInt(value, &indent))
    SendMessage(ih->handle, TVM_SETINDENT, (WPARAM)indent, 0);
  return 0;
}

static char* winTreeGetStateAttrib(Ihandle* ih, const char* name_id)
{
  TVITEM item;
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE|TVIF_PARAM; 
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  if (itemData->kind == ITREE_BRANCH)
  {
    if (winTreeIsItemExpanded(ih, hItem))
      return "EXPANDED";
    else
      return "COLLAPSED";
  }

  return NULL;
}

static int winTreeSetStateAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  winTreeExpandItem(ih, hItem, iupStrEqualNoCase(value, "EXPANDED"));
  return 0;
}

static char* winTreeGetDepthAttrib(Ihandle* ih, const char* name_id)
{
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  HTREEITEM hItemRoot = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
  int depth = 0;
  char* str;

  if (!hItem)
    return NULL;

  while((hItemRoot != hItem) && (hItem != NULL))
  {
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem);
    depth++;
  }

  str = iupStrGetMemory(10);
  sprintf(str, "%d", depth);
  return str;
}

static int winTreeSetMoveNodeAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  HTREEITEM hItemDst, hParent, hItemSrc;

  if (!ih->handle)  /* do not store the action before map */
    return 0;
  hItemSrc = winTreeFindNodeFromString(ih, name_id);
  if (!hItemSrc)
    return 0;
  hItemDst = winTreeFindNodeFromString(ih, value);
  if (!hItemDst)
    return 0;

  /* If Drag item is an ancestor of Drop item then return */
  hParent = hItemDst;
  while(hParent)
  {
    hParent = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hParent);
    if (hParent == hItemSrc)
      return 0;
  }

  /* Copying the node and its children to the new position */
  winTreeCopyNode(ih, hItemSrc, hItemDst, 0);  /* not a full copy, preserve user data */

  /* do not delete the user data, we copy the references in CopyNode */
  SendMessage(ih->handle, TVM_DELETEITEM, 0, (LPARAM)hItemSrc);

  return 0;
}

static int winTreeSetCopyNodeAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  HTREEITEM hItemDst, hParent, hItemSrc;

  if (!ih->handle)  /* do not store the action before map */
    return 0;
  hItemSrc = winTreeFindNodeFromString(ih, name_id);
  if (!hItemSrc)
    return 0;
  hItemDst = winTreeFindNodeFromString(ih, value);
  if (!hItemDst)
    return 0;

  /* If Drag item is an ancestor of Drop item then return */
  hParent = hItemDst;
  while(hParent)
  {
    hParent = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hParent);
    if (hParent == hItemSrc)
      return 0;
  }

  /* Copying the node and its children to the new position */
  winTreeCopyNode(ih, hItemSrc, hItemDst, 1);

  return 0;
}

static char* winTreeGetColorAttrib(Ihandle* ih, const char* name_id)
{
  unsigned char r, g, b;
  char* str;
  TVITEM item;
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  r = GetRValue(itemData->color);
  g = GetGValue(itemData->color);
  b = GetBValue(itemData->color);

  str = iupStrGetMemory(12);
  sprintf(str, "%d %d %d", r, g, b);
  return str;
}
 
static int winTreeSetColorAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  unsigned char r, g, b;
  TVITEM item;
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE | TVIF_PARAM;
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  if (iupStrToRGB(value, &r, &g, &b))
  {
    itemData->color = RGB(r,g,b);
    iupdrvDisplayUpdate(ih);
  }

 return 0;
}

static int winTreeSetFgColorAttrib(Ihandle* ih, const char* value)
{
  unsigned char r, g, b;

  if (iupStrToRGB(value, &r, &g, &b))
  {
    COLORREF color = RGB(r,g,b);
    SendMessage(ih->handle, TVM_SETTEXTCOLOR, 0, (LPARAM)color);
  }
  else
    SendMessage(ih->handle, TVM_SETTEXTCOLOR, 0, (LPARAM)CLR_DEFAULT);

  return 0;
}

static char* winTreeGetChildCountAttrib(Ihandle* ih, const char* name_id)
{
  int count;
  char* str;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;

  count = 0;
  hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
  while(hItem != NULL)
  {
    count++;

    /* Go to next sibling item */
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
  }

  str = iupStrGetMemory(10);
  sprintf(str, "%d", count);
  return str;
}

static char* winTreeGetCountAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(10);
  sprintf(str, "%d", (int)SendMessage(ih->handle, TVM_GETCOUNT, 0, 0));
  return str;
}

static char* winTreeGetKindAttrib(Ihandle* ih, const char* name_id)
{
  TVITEM item; 
  winTreeItemData* itemData;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE|TVIF_PARAM; 
  SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
  itemData = (winTreeItemData*)item.lParam;

  if(itemData->kind == ITREE_BRANCH)
    return "BRANCH";
  else
    return "LEAF";
}

static char* winTreeGetParentAttrib(Ihandle* ih, const char* name_id)
{
  char* str;
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;

  hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem);
  if (!hItem)
    return NULL;

  str = iupStrGetMemory(10);
  sprintf(str, "%d", winTreeGetNodeId(ih, hItem));
  return str;
}

static void winTreeDelNodeData(Ihandle* ih, HTREEITEM hItem)
{
  TVITEM item; 
  HTREEITEM hChildItem;

  item.hItem = hItem;
  item.mask = TVIF_HANDLE|TVIF_PARAM; 
  if (SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item))
  {
    winTreeItemData* itemData = (winTreeItemData*)item.lParam;
    if (itemData)
    {
      IFnis cb = (IFnis)IupGetCallback(ih, "NODEREMOVED_CB");
      if (cb) cb(ih, winTreeGetNodeId(ih, hItem), (char*)itemData->userdata);
      free(itemData);
      item.lParam = (LPARAM)NULL;
      SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(LPTVITEM)&item);
    }
  }

  hChildItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
  while (hChildItem)
  {
    winTreeDelNodeData(ih, hChildItem);
    hChildItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hChildItem);
  }
}

static int winTreeSetDelNodeAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  if (!ih->handle)  /* do not store the action before map */
    return 0;
  if(iupStrEqualNoCase(value, "SELECTED")) /* selected here means the specified one */
  {
    HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
    HTREEITEM hItemRoot  = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);

    /* the root node can't be deleted */
    if(!hItem || hItem == hItemRoot)
      return 0;

    /* deleting the specified node (and it's children) */
    winTreeDelNodeData(ih, hItem);
    SendMessage(ih->handle, TVM_DELETEITEM, 0, (LPARAM)hItem);
    
    return 0;
  }
  else if(iupStrEqualNoCase(value, "CHILDREN"))  /* children of the specified one */
  {
    HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
    HTREEITEM hChildItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);

    if(!hItem)
      return 0;

    /* deleting the selected node's children */
    while (hChildItem)
    {
      winTreeDelNodeData(ih, hChildItem);
      SendMessage(ih->handle, TVM_DELETEITEM, 0, (LPARAM)hChildItem);
      hChildItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
    }

    return 0;
  }
  else if(iupStrEqualNoCase(value, "MARKED"))
  {
    int i, count;
    Iarray* markedArray;
    HTREEITEM* hItemArrayData;
    HTREEITEM  hItemRoot = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);

    /* Delete the array of marked nodes */
    markedArray = winTreeGetSelectedArray(ih);
    hItemArrayData = (HTREEITEM*)iupArrayGetData(markedArray);
    count = iupArrayCount(markedArray);

    for(i = 0; i < count; i++)
    {
      if (hItemArrayData[i] != hItemRoot)  /* the root node can't be deleted */
      {
        winTreeDelNodeData(ih, hItemArrayData[i]);
        SendMessage(ih->handle, TVM_DELETEITEM, 0, (LPARAM)hItemArrayData[i]);
      }
    }

    iupArrayDestroy(markedArray);

    return 0;
  }

  return 0;
}

static int winTreeSetRenameAttrib(Ihandle* ih, const char* value)
{  
  HTREEITEM hItemFocus = winTreeGetFocusNode(ih);
  if (ih->data->show_rename)
  {
    IFni cbShowRename = (IFni)IupGetCallback(ih, "SHOWRENAME_CB");
    if (cbShowRename)
      cbShowRename(ih, winTreeGetNodeId(ih, hItemFocus));

    SetFocus(ih->handle); /* the tree must have focus to activate the edit */
    SendMessage(ih->handle, TVM_EDITLABEL, 0, (LPARAM)hItemFocus);
  }
  else
  {
    IFnis cbRenameNode = (IFnis)IupGetCallback(ih, "RENAMENODE_CB");
    if (cbRenameNode)
      cbRenameNode(ih, winTreeGetNodeId(ih, hItemFocus), winTreeGetTitle(ih, hItemFocus));  
  }
  
  (void)value;
  return 0;
}

static char* winTreeGetMarkedAttrib(Ihandle* ih, const char* name_id)
{
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return NULL;

  if (winTreeIsItemSelected(ih, hItem))
    return "YES";
  else 
    return "NO";
}

static int winTreeSetMarkedAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  winTreeSelectItem(ih, hItem, iupStrBoolean(value));
  return 0;
}

static int winTreeSetMarkStartAttrib(Ihandle* ih, const char* name_id)
{
  HTREEITEM hItem = winTreeFindNodeFromString(ih, name_id);
  if (!hItem)
    return 0;

  iupAttribSetStr(ih, "_IUPTREE_MARKSTART_NODE", (char*)hItem);

  return 1;
}

static char* winTreeGetValueAttrib(Ihandle* ih)
{
  char* str;
  HTREEITEM hItemFocus = winTreeGetFocusNode(ih);
  if (!hItemFocus)
    return "0"; /* default VALUE is root */

  str = iupStrGetMemory(16);
  sprintf(str, "%d", winTreeGetNodeId(ih, hItemFocus));
  return str;
}

static int winTreeSetMarkAttrib(Ihandle* ih, const char* value)
{
  if (ih->data->mark_mode==ITREE_MARK_SINGLE)
    return 0;

  if(iupStrEqualNoCase(value, "BLOCK"))
  {
    HTREEITEM hItemFocus = winTreeGetFocusNode(ih);
    winTreeSelectRange(ih, (HTREEITEM)iupAttribGet(ih, "_IUPTREE_MARKSTART_NODE"), hItemFocus, 0);
  }
  else if(iupStrEqualNoCase(value, "CLEARALL"))
    winTreeClearSelection(ih, NULL);
  else if(iupStrEqualNoCase(value, "MARKALL"))
    winTreeSelectAll(ih);
  else if(iupStrEqualNoCase(value, "INVERTALL")) /* INVERTALL *MUST* appear before INVERT, or else INVERTALL will never be called. */
    winTreeForEach(ih, NULL, (winTreeNodeFunc)winTreeInvertSelectFunc, NULL);
  else if(iupStrEqualPartial(value, "INVERT")) /* iupStrEqualPartial allows the use of "INVERTid" form */
  {
    HTREEITEM hItem = winTreeFindNodeFromString(ih, &value[strlen("INVERT")]);
    if (!hItem)
      return 0;

    winTreeSelectItem(ih, hItem, -1); /* toggle */
  }
  else
  {
    HTREEITEM hItem1, hItem2;

    char str1[50], str2[50];
    if (iupStrToStrStr(value, str1, str2, '-')!=2)
      return 0;

    hItem1 = winTreeFindNodeFromString(ih, str1);
    if (!hItem1)  
      return 0;
    hItem2 = winTreeFindNodeFromString(ih, str2);
    if (!hItem2)  
      return 0;

    winTreeSelectRange(ih, hItem1, hItem2, 0);
  }

  return 1;
} 

static int winTreeSetValueAttrib(Ihandle* ih, const char* value)
{
  HTREEITEM hItem = NULL;
  HTREEITEM hItemFocus;

  if (winTreeSetMarkAttrib(ih, value))
    return 0;

  hItemFocus = winTreeGetFocusNode(ih);

  if(iupStrEqualNoCase(value, "ROOT"))
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
  else if(iupStrEqualNoCase(value, "LAST"))
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_LASTVISIBLE, 0);
  else if(iupStrEqualNoCase(value, "PGUP"))
  {
    int i;
    HTREEITEM hItemPrev = hItemFocus;
    HTREEITEM hItemNext = hItemFocus;
    for(i = 0; i < 10; i++)
    {
      hItemNext = hItemPrev;
      hItemPrev = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)hItemPrev);
      if(hItemPrev == NULL)
      {
        hItemPrev = hItemNext;
        break;
      }
    }
    
    hItem = hItemPrev;
  }
  else if(iupStrEqualNoCase(value, "PGDN"))
  {
    int i;
    HTREEITEM hItemPrev = hItemFocus;
    HTREEITEM hItemNext = hItemFocus;
    
    for(i = 0; i < 10; i++)
    {
      hItemPrev = hItemNext;
      hItemNext = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItemNext);
      if(hItemNext == NULL)
      {
        hItemNext = hItemPrev;
        break;
      }
    }
    
    hItem = hItemNext;
  }
  else if(iupStrEqualNoCase(value, "NEXT"))
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItemFocus);
  else if(iupStrEqualNoCase(value, "PREVIOUS"))
    hItem = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)hItemFocus);
  else
    hItem = winTreeFindNodeFromString(ih, value);

  if (hItem)
  {
    if (ih->data->mark_mode==ITREE_MARK_SINGLE)
    {
      iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", "1");
      winTreeSelectItem(ih, hItem, 1);
      iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", NULL);
    }
    winTreeSetFocusNode(ih, hItem);
  }

  return 0;
} 

void iupdrvTreeUpdateMarkMode(Ihandle *ih)
{
  /* does nothing, must handle single and multiple selection manually in Windows */
  (void)ih;
}

/*********************************************************************************************************/


static int winTreeEditProc(Ihandle* ih, HWND cbedit, UINT msg, WPARAM wp, LPARAM lp, LRESULT *result)
{
  switch (msg)
  {
    case WM_GETDLGCODE:
    {
      MSG* pMsg = (MSG*)lp;

      if (pMsg && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
      {
        if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
        {
          /* these keys are not processed if the return code is not this */
          *result = DLGC_WANTALLKEYS;
          return 1;
        }
      }
    }
  }
  
  (void)wp;
  (void)cbedit;
  (void)ih;
  return 0;
}

static LRESULT CALLBACK winTreeEditWinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{   
  int ret = 0;
  LRESULT result = 0;
  WNDPROC oldProc;
  Ihandle *ih;

  ih = iupwinHandleGet(hwnd); 
  if (!ih)
    return DefWindowProc(hwnd, msg, wp, lp);  /* should never happen */

  /* retrieve the control previous procedure for subclassing */
  oldProc = (WNDPROC)IupGetCallback(ih, "_IUPWIN_EDITOLDPROC_CB");

  ret = winTreeEditProc(ih, hwnd, msg, wp, lp, &result);

  if (ret)
    return result;
  else
    return CallWindowProc(oldProc, hwnd, msg, wp, lp);
}

static void winTreeDrag(Ihandle* ih, int x, int y)
{
  HTREEITEM	hItemDrop;

  HIMAGELIST dragImageList = (HIMAGELIST)iupAttribGet(ih, "_IUPTREE_DRAGIMAGELIST");
  if (dragImageList)
  {
    POINT pnt;
    pnt.x = x;
    pnt.y = y;
    GetCursorPos(&pnt);
    ClientToScreen(GetDesktopWindow(), &pnt) ;
    ImageList_DragMove(pnt.x, pnt.y);
  }

  if ((hItemDrop = winTreeFindNodePointed(ih)) != NULL)
  {
    if(dragImageList)
      ImageList_DragShowNolock(FALSE);

    SendMessage(ih->handle, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hItemDrop);

    /* store the drop item to be executed */
    iupAttribSetStr(ih, "_IUPTREE_DROPITEM", (char*)hItemDrop);

    if(dragImageList)
      ImageList_DragShowNolock(TRUE);
  }
}

static void winTreeDrop(Ihandle* ih)
{
  HTREEITEM	 hItemDrag     =  (HTREEITEM)iupAttribGet(ih, "_IUPTREE_DRAGITEM");
  HTREEITEM	 hItemDrop     =  (HTREEITEM)iupAttribGet(ih, "_IUPTREE_DROPITEM");
  HIMAGELIST dragImageList = (HIMAGELIST)iupAttribGet(ih, "_IUPTREE_DRAGIMAGELIST");
  HTREEITEM  hParent;
  int is_ctrl;

  if (dragImageList)
  {
    ImageList_DragLeave(ih->handle);
    ImageList_EndDrag();
    ImageList_Destroy(dragImageList);
    iupAttribSetStr(ih, "_IUPTREE_DRAGIMAGELIST", NULL);
  }

  ReleaseCapture();
  ShowCursor(TRUE);

  /* Remove drop target highlighting */
  SendMessage(ih->handle, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)NULL);

  iupAttribSetStr(ih, "_IUPTREE_DRAGITEM", NULL);
  iupAttribSetStr(ih, "_IUPTREE_DROPITEM", NULL);

  if (!hItemDrop || hItemDrag == hItemDrop)
    return;

  /* If Drag item is an ancestor of Drop item then return */
  hParent = hItemDrop;
  while(hParent)
  {
    hParent = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hParent);
    if (hParent == hItemDrag)
      return;
  }

  if (winTreeCallDragDropCb(ih, hItemDrag, hItemDrop, &is_ctrl) == IUP_CONTINUE)
  {
    /* Copy the dragged item to the new position. */
    HTREEITEM  hItemNew = winTreeCopyNode(ih, hItemDrag, hItemDrop, is_ctrl);

    if (!is_ctrl)
    {
      /* do not delete the user data, we copy the references in CopyNode */
      SendMessage(ih->handle, TVM_DELETEITEM, 0, (LPARAM)hItemDrag);
    }

    SendMessage(ih->handle, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItemNew); /* set focus and selection */
  }
}  

static void winTreeExtendSelect(Ihandle* ih, int x, int y)
{
  HTREEITEM hItemFirstSel;
  TVHITTESTINFO info;
  HTREEITEM hItem;
  info.pt.x = x;
  info.pt.y = y;
  hItem = (HTREEITEM)SendMessage(ih->handle, TVM_HITTEST, 0, (LPARAM)&info);

  if (!(info.flags & TVHT_ONITEM) || !hItem)
    return;

  hItemFirstSel = (HTREEITEM)iupAttribGet(ih, "_IUPTREE_FIRSTSELITEM");
  if (hItemFirstSel)
  {
    iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", "1");
    winTreeSelectRange(ih, hItemFirstSel, hItem, 1);
    iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", NULL);

    iupAttribSetStr(ih, "_IUPTREE_LASTSELITEM", (char*)hItem);
    winTreeSetFocusNode(ih, hItem);
  }
}

static int winTreeMouseMultiSelect(Ihandle* ih, int x, int y)
{
  TVHITTESTINFO info;
  HTREEITEM hItem;
  info.pt.x = x;
  info.pt.y = y;
  hItem = (HTREEITEM)SendMessage(ih->handle, TVM_HITTEST, 0, (LPARAM)&info);

  if (!(info.flags & TVHT_ONITEM) || !hItem)
    return 0;

  if (GetKeyState(VK_CONTROL) & 0x8000) /* Control key is down */
  {
    /* Toggle selection state */
    winTreeSelectItem(ih, hItem, -1);
    iupAttribSetStr(ih, "_IUPTREE_FIRSTSELITEM", (char*)hItem);

    winTreeCallSelectionCb(ih, winTreeIsItemSelected(ih, hItem), hItem);
    winTreeSetFocusNode(ih, hItem);

    return 1;
  }
  else if (GetKeyState(VK_SHIFT) & 0x8000) /* Shift key is down */
  {
    HTREEITEM hItemFirstSel = (HTREEITEM)iupAttribGet(ih, "_IUPTREE_FIRSTSELITEM");
    if (hItemFirstSel)
    {
      iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", "1");
      winTreeSelectRange(ih, hItemFirstSel, hItem, 1);
      iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", NULL);

      winTreeCallMultiSelectionCb(ih);
      winTreeSetFocusNode(ih, hItem);
      return 1;
    }
  }

  /* simple click */
  winTreeClearSelection(ih, hItem);
  iupAttribSetStr(ih, "_IUPTREE_FIRSTSELITEM", (char*)hItem);
  iupAttribSetStr(ih, "_IUPTREE_EXTENDSELECT", "1");

  return 0;
}

static int winTreeProc(Ihandle* ih, UINT msg, WPARAM wp, LPARAM lp, LRESULT *result)
{
  switch (msg)
  {
  case WM_CTLCOLOREDIT:
    {
      HWND hEdit = (HWND)iupAttribGet(ih, "_IUPWIN_EDITBOX");
      if (hEdit)
      {
        winTreeItemData* itemData = (winTreeItemData*)iupAttribGet(ih, "_IUPWIN_EDIT_DATA");
        HDC hDC = (HDC)wp;
        COLORREF cr;

        SetTextColor(hDC, itemData->color);

        cr = (COLORREF)SendMessage(ih->handle, TVM_GETBKCOLOR, 0, 0);
        SetBkColor(hDC, cr);
        SetDCBrushColor(hDC, cr);
        *result = (LRESULT)GetStockObject(DC_BRUSH);
        return 1;
      }

      break;
    }
  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    {
      /*   ------------Comment from wxWidgets--------------------
        the tree control greys out the selected item when it loses focus and
        paints it as selected again when it regains it, but it won't do it
        for the other items itself - help it */
      if (ih->data->mark_mode == ITREE_MARK_MULTIPLE)
      {
        HTREEITEM hItemChild = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);
        RECT rect;

        while(hItemChild != NULL)
        {
          *(HTREEITEM*)&rect = hItemChild;
          if (SendMessage(ih->handle, TVM_GETITEMRECT, TRUE, (LPARAM)&rect))
            InvalidateRect(ih->handle, &rect, FALSE);

          /* Go to next visible item */
          hItemChild = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItemChild);
        }
      }
      break;
    }
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN:
    {
      if (iupwinBaseProc(ih, msg, wp, lp, result)==1)
        return 1;

      if (wp == VK_RETURN)
      {
        HTREEITEM hItemFocus = winTreeGetFocusNode(ih);
        if (winTreeCallBranchLeafCb(ih, hItemFocus) != IUP_IGNORE)
          winTreeExpandItem(ih, hItemFocus, -1);

        *result = 0;
        return 1;
      }      
      else if (wp == VK_F2)
      {
        winTreeSetRenameAttrib(ih, NULL);
        *result = 0;
        return 1;
      }
      else if (wp == VK_SPACE)
      {
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
          HTREEITEM hItemFocus = winTreeGetFocusNode(ih);
          /* Toggle selection state */
          winTreeSelectItem(ih, hItemFocus, -1);
        }
      }
      else if (wp == VK_UP || wp == VK_DOWN)
      {
        HTREEITEM hItemFocus = winTreeGetFocusNode(ih);
        if (wp == VK_UP)
          hItemFocus = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)hItemFocus);
        else
          hItemFocus = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItemFocus);
        if (!hItemFocus)
          return 0;

        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
          /* Only move focus */
          winTreeSetFocusNode(ih, hItemFocus);

          *result = 0;
          return 1;
        }
        else if (GetKeyState(VK_SHIFT) & 0x8000)
        {
          HTREEITEM hItemFirstSel = (HTREEITEM)iupAttribGet(ih, "_IUPTREE_FIRSTSELITEM");
          if (hItemFirstSel)
          {
            iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", "1");
            winTreeSelectRange(ih, hItemFirstSel, hItemFocus, 1);
            iupAttribSetStr(ih, "_IUPTREE_IGNORE_SELECTION_CB", NULL);

            winTreeCallMultiSelectionCb(ih);
            winTreeSetFocusNode(ih, hItemFocus);

            *result = 0;
            return 1;
          }
        }

        if (ih->data->mark_mode==ITREE_MARK_MULTIPLE)
        {
          iupAttribSetStr(ih, "_IUPTREE_FIRSTSELITEM", (char*)hItemFocus);
          winTreeClearSelection(ih, NULL);
          /* normal processing will select the focus item */
        }
      }

      return 0;
    }
  case WM_LBUTTONDOWN:
    if (iupwinButtonDown(ih, msg, wp, lp)==-1)
    {
      *result = 0;
      return 1;
    }

    if (ih->data->mark_mode==ITREE_MARK_MULTIPLE)
    {
      /* must set focus on left button down or the tree won't show its focus */
      if (iupAttribGetBoolean(ih, "CANFOCUS"))
        SetFocus(ih->handle);

      if (winTreeMouseMultiSelect(ih, (int)(short)LOWORD(lp), (int)(short)HIWORD(lp)))
      {
        *result = 0; /* abort the normal processing if we process multiple selection */
        return 1;
      }
    }
    break;
  case WM_MBUTTONDOWN:
  case WM_RBUTTONDOWN:
  case WM_LBUTTONDBLCLK:
  case WM_MBUTTONDBLCLK:
  case WM_RBUTTONDBLCLK:
    if (iupwinButtonDown(ih, msg, wp, lp)==-1)
    {
      *result = 0;
      return 1;
    }
    break;
  case WM_MOUSEMOVE:
    if (ih->data->show_dragdrop && (HTREEITEM)iupAttribGet(ih, "_IUPTREE_DRAGITEM") != NULL)
      winTreeDrag(ih, (int)(short)LOWORD(lp), (int)(short)HIWORD(lp));
    else if (iupAttribGet(ih, "_IUPTREE_EXTENDSELECT"))
      winTreeExtendSelect(ih, (int)(short)LOWORD(lp), (int)(short)HIWORD(lp));

    iupwinMouseMove(ih, msg, wp, lp);
    break;
  case WM_LBUTTONUP:
  case WM_MBUTTONUP:
  case WM_RBUTTONUP:
    if (iupwinButtonUp(ih, msg, wp, lp)==-1)
    {
      *result = 0;
      return 1;
    }

    if (iupAttribGet(ih, "_IUPTREE_EXTENDSELECT"))
    {
      iupAttribSetStr(ih, "_IUPTREE_EXTENDSELECT", NULL);
      if (iupAttribGet(ih, "_IUPTREE_LASTSELITEM"))
      {
        winTreeCallMultiSelectionCb(ih);
        iupAttribSetStr(ih, "_IUPTREE_LASTSELITEM", NULL);
      }
    }

    if (ih->data->show_dragdrop && (HTREEITEM)iupAttribGet(ih, "_IUPTREE_DRAGITEM") != NULL)
      winTreeDrop(ih);

    break;
  case WM_CHAR:
    {
      if (wp==VK_TAB)  /* the keys have the same definitions as the chars */
      {
        *result = 0;
        return 1;   /* abort default processing to avoid beep */
      }
      break;
    }
  }

  return iupwinBaseProc(ih, msg, wp, lp, result);
}

static COLORREF winTreeInvertColor(COLORREF color)
{
  return RGB(~GetRValue(color), ~GetGValue(color), ~GetBValue(color));
}

static int winTreeWmNotify(Ihandle* ih, NMHDR* msg_info, int *result)
{
  if (msg_info->code == TVN_ITEMCHANGINGA || msg_info->code == TVN_ITEMCHANGINGW) /* Vista Only */
  {
    if (ih->data->mark_mode==ITREE_MARK_MULTIPLE)
    {
      NMTVITEMCHANGE* info = (NMTVITEMCHANGE*)msg_info;
      HTREEITEM hItem = (HTREEITEM)iupAttribGet(ih, "_IUPTREE_ALLOW_CHANGE");
      if (hItem && hItem != info->hItem)  /* Workaround for Vista SetFocus */
      {
        *result = TRUE;  /* prevent the change */
        return 1;
      }
    }
  }
  else if (msg_info->code == TVN_SELCHANGED)
  {
    NMTREEVIEW* info = (NMTREEVIEW*)msg_info;
    winTreeCallSelectionCb(ih, 0, info->itemOld.hItem);  /* node unselected */
    winTreeCallSelectionCb(ih, 1, info->itemNew.hItem);  /* node selected */
  }
  else if(msg_info->code == TVN_BEGINLABELEDIT)
  {
    char* value;
    HWND hEdit;
    NMTVDISPINFO* info = (NMTVDISPINFO*)msg_info;
           
    if (iupAttribGet(ih, "_IUPTREE_EXTENDSELECT"))
    {
      *result = TRUE;  /* prevent the change */
      return 1;
    }

    hEdit = (HWND)SendMessage(ih->handle, TVM_GETEDITCONTROL, 0, 0);

    /* save the edit box. */
    iupwinHandleAdd(ih, hEdit);
    iupAttribSetStr(ih, "_IUPWIN_EDITBOX", (char*)hEdit);

    /* subclass the edit box. */
    IupSetCallback(ih, "_IUPWIN_EDITOLDPROC_CB", (Icallback)GetWindowLongPtr(hEdit, GWLP_WNDPROC));
    SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)winTreeEditWinProc);

    value = iupAttribGetStr(ih, "RENAMECARET");
    if (value)
      winTreeSetRenameCaretPos(hEdit, value);

    value = iupAttribGetStr(ih, "RENAMESELECTION");
    if (value)
      winTreeSetRenameSelectionPos(hEdit, value);

    {
      winTreeItemData* itemData;
      TVITEM item; 
      item.hItem = info->item.hItem;
      item.mask = TVIF_HANDLE | TVIF_PARAM;
      SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
      itemData = (winTreeItemData*)item.lParam;
      iupAttribSetStr(ih, "_IUPWIN_EDIT_DATA", (char*)itemData);

      if (itemData->hFont)
        SendMessage(hEdit, WM_SETFONT, (WPARAM)itemData->hFont, MAKELPARAM(TRUE,0));
    }
  }
  else if(msg_info->code == TVN_ENDLABELEDIT)
  {
    NMTVDISPINFO* info = (NMTVDISPINFO*)msg_info;

    iupAttribSetStr(ih, "_IUPWIN_EDITBOX", NULL);

    if (info->item.pszText)
    {
      IFnis cbRename = (IFnis)IupGetCallback(ih, "RENAME_CB");
      if (cbRename)
      {
        if (cbRename(ih, winTreeGetNodeId(ih, info->item.hItem), info->item.pszText) == IUP_IGNORE)
        {
          *result = FALSE;
          return 1;
        }
      }

      *result = TRUE;
      return 1;
    }
  }
  else if(msg_info->code == TVN_BEGINDRAG)
  {
    if (ih->data->show_dragdrop)
    {
      NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)msg_info;
      HTREEITEM   hItemDrag   = pNMTreeView->itemNew.hItem;
      HIMAGELIST  dragImageList;

      /* store the drag-and-drop item */
      iupAttribSetStr(ih, "_IUPTREE_DRAGITEM", (char*)hItemDrag);

      /* get the image list for dragging */
      dragImageList = (HIMAGELIST)SendMessage(ih->handle, TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItemDrag);
      if (dragImageList)
      {
        POINT pt = pNMTreeView->ptDrag;
        ImageList_BeginDrag(dragImageList, 0, 0, 0);

        ClientToScreen(ih->handle, &pt);
        ImageList_DragEnter(NULL, pt.x, pt.y);

        iupAttribSetStr(ih, "_IUPTREE_DRAGIMAGELIST", (char*)dragImageList);
      }

      ShowCursor(FALSE);
      SetCapture(ih->handle);  /* drag only inside the tree */
    }
  }
  else if(msg_info->code == NM_DBLCLK)
  {
    HTREEITEM hItemFocus = winTreeGetFocusNode(ih);
    TVITEM item;
    winTreeItemData* itemData;

    /* Get Children: branch or leaf */
    item.mask = TVIF_HANDLE|TVIF_PARAM; 
    item.hItem = hItemFocus;
    SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
    itemData = (winTreeItemData*)item.lParam;

    if (itemData->kind == ITREE_LEAF)
    {
      IFni cbExecuteLeaf = (IFni)IupGetCallback(ih, "EXECUTELEAF_CB");
      if(cbExecuteLeaf)
        cbExecuteLeaf(ih, winTreeGetNodeId(ih, hItemFocus));
    }
  }
  else if(msg_info->code == TVN_ITEMEXPANDING)
  {
    /* mouse click by user: click on the "+" sign or double click on the selected node */
    NMTREEVIEW* tree_info = (NMTREEVIEW*)msg_info;
    HTREEITEM hItem = tree_info->itemNew.hItem;

    if (winTreeCallBranchLeafCb(ih, hItem) != IUP_IGNORE)
    {
      TVITEM item;
      winTreeItemData* itemData = (winTreeItemData*)tree_info->itemNew.lParam;

      if (tree_info->action == TVE_EXPAND)
        item.iSelectedImage = item.iImage = (itemData->image_expanded!=-1)? itemData->image_expanded: (int)ih->data->def_image_expanded;
      else
        item.iSelectedImage = item.iImage = (itemData->image!=-1)? itemData->image: (int)ih->data->def_image_collapsed;

      item.hItem = hItem;
      item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
      SendMessage(ih->handle, TVM_SETITEM, 0, (LPARAM)(LPTVITEM)&item);
    }
    else
    {
      *result = TRUE;  /* prevent the change */
      return 1;
    }
  }
  else if(msg_info->code == NM_RCLICK)
  {
    HTREEITEM hItem = winTreeFindNodePointed(ih);
    IFni cbRightClick  = (IFni)IupGetCallback(ih, "RIGHTCLICK_CB");
    if (cbRightClick)
      cbRightClick(ih, winTreeGetNodeId(ih, hItem));
  }
  else if (msg_info->code == NM_CUSTOMDRAW)
  {
    NMTVCUSTOMDRAW *customdraw = (NMTVCUSTOMDRAW*)msg_info;

    if (customdraw->nmcd.dwDrawStage == CDDS_PREPAINT)
    {
      *result = CDRF_NOTIFYPOSTPAINT|CDRF_NOTIFYITEMDRAW;
      return 1;
    }
 
    if (customdraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
    {
      TVITEM item;
      winTreeItemData* itemData;
      HTREEITEM hItem = (HTREEITEM)customdraw->nmcd.dwItemSpec;

      item.hItem = hItem;
      item.mask = TVIF_HANDLE | TVIF_PARAM;
      SendMessage(ih->handle, TVM_GETITEM, 0, (LPARAM)(LPTVITEM)&item);
      itemData = (winTreeItemData*)item.lParam;

      if (winTreeIsItemSelected(ih, hItem))
        customdraw->clrText = winTreeInvertColor(itemData->color);
      else
        customdraw->clrText = itemData->color;

      *result = CDRF_NOTIFYSUBITEMDRAW|CDRF_NOTIFYPOSTPAINT;

      if (itemData->hFont)
      {
        SelectObject(customdraw->nmcd.hdc, itemData->hFont);
        *result |= CDRF_NEWFONT;
      }

      return 1;
    }
  }

  return 0;  /* allow the default processsing */
}

static int winTreeConvertXYToPos(Ihandle* ih, int x, int y)
{
  TVHITTESTINFO info;
  HTREEITEM hItem;
  info.pt.x = x;
  info.pt.y = y;
  hItem = (HTREEITEM)SendMessage(ih->handle, TVM_HITTEST, 0, (LPARAM)&info);
  if (hItem)
    return winTreeGetNodeId(ih, hItem);
  return -1;
}


/*******************************************************************************************/

static void winTreeUnMapMethod(Ihandle* ih)
{
  Iarray* bmp_array;
  HIMAGELIST image_list;

  HTREEITEM itemRoot = (HTREEITEM)SendMessage(ih->handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
  winTreeDelNodeData(ih, itemRoot);

  image_list = (HIMAGELIST)SendMessage(ih->handle, TVM_GETIMAGELIST, TVSIL_NORMAL, 0);
  if (image_list)
    ImageList_Destroy(image_list);

  bmp_array = (Iarray*)iupAttribGet(ih, "_IUPWIN_BMPARRAY");
  if (bmp_array)
    iupArrayDestroy(bmp_array);

  iupdrvBaseUnMapMethod(ih);
}

static int winTreeMapMethod(Ihandle* ih)
{
  DWORD dwStyle = WS_CHILD | WS_BORDER | TVS_SHOWSELALWAYS;

  /* can be set only on the Tree View creation */

  if (!ih->data->show_dragdrop)
    dwStyle |= TVS_DISABLEDRAGDROP;

  if (ih->data->show_rename)
    dwStyle |= TVS_EDITLABELS;

  if (!iupAttribGetBoolean(ih, "HIDELINES"))
    dwStyle |= TVS_HASLINES;

  if (!iupAttribGetBoolean(ih, "HIDEBUTTONS"))
    dwStyle |= TVS_HASBUTTONS;

  if (iupAttribGetBoolean(ih, "CANFOCUS"))
    dwStyle |= WS_TABSTOP;

  if (!ih->parent)
    return IUP_ERROR;

  if (!iupwinCreateWindowEx(ih, WC_TREEVIEW, 0, dwStyle))
    return IUP_ERROR;

  IupSetCallback(ih, "_IUPWIN_CTRLPROC_CB", (Icallback)winTreeProc);
  IupSetCallback(ih, "_IUPWIN_NOTIFY_CB",   (Icallback)winTreeWmNotify);

  /* Force background update before setting the images */
  {
    char* value = iupAttribGet(ih, "BGCOLOR");
    if (value)
    {
      winTreeSetBgColorAttrib(ih, value);
      iupAttribSetStr(ih, "BGCOLOR", NULL);
    }
    else if (iupwinGetSystemMajorVersion()<6) /* force background in XP because of the editbox background */
      winTreeSetBgColorAttrib(ih, IupGetGlobal("TXTBGCOLOR"));
  }

  /* Initialize the default images */
  ih->data->def_image_leaf = (void*)winTreeGetImageIndex(ih, "IMGLEAF");
  ih->data->def_image_collapsed = (void*)winTreeGetImageIndex(ih, "IMGCOLLAPSED");
  ih->data->def_image_expanded = (void*)winTreeGetImageIndex(ih, "IMGEXPANDED");

  /* Add the Root Node */
  winTreeAddRootNode(ih);

  /* configure for DRAG&DROP of files */
  if (IupGetCallback(ih, "DROPFILES_CB"))
    iupAttribSetStr(ih, "DRAGDROP", "YES");

  IupSetCallback(ih, "_IUP_XY2POS_CB", (Icallback)winTreeConvertXYToPos);

  return IUP_NOERROR;
}

void iupdrvTreeInitClass(Iclass* ic)
{
  /* Driver Dependent Class functions */
  ic->Map = winTreeMapMethod;
  ic->UnMap = winTreeUnMapMethod;

  /* Visual */
  iupClassRegisterAttribute(ic, "BGCOLOR", winTreeGetBgColorAttrib, winTreeSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "TXTBGCOLOR", IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "FGCOLOR", NULL, winTreeSetFgColorAttrib, IUPAF_SAMEASSYSTEM, "TXTFGCOLOR", IUPAF_DEFAULT);

  /* IupTree Attributes - GENERAL */
  iupClassRegisterAttribute(ic, "EXPANDALL",  NULL, winTreeSetExpandAllAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "INDENTATION", winTreeGetIndentationAttrib, winTreeSetIndentationAttrib, NULL, NULL, IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "COUNT", winTreeGetCountAttrib, NULL, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "DRAGDROP", NULL, iupwinSetDragDropAttrib, NULL, NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "SPACING", iupTreeGetSpacingAttrib, winTreeSetSpacingAttrib, NULL, NULL, IUPAF_NOT_MAPPED);
  iupClassRegisterAttribute(ic, "TOPITEM", NULL, winTreeSetTopItemAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);

  /* IupTree Attributes - IMAGES */
  iupClassRegisterAttributeId(ic, "IMAGE", NULL, winTreeSetImageAttrib, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "IMAGEEXPANDED", NULL, winTreeSetImageExpandedAttrib, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  
  iupClassRegisterAttribute(ic, "IMAGELEAF",            NULL, winTreeSetImageLeafAttrib, IUPAF_SAMEASSYSTEM, "IMGLEAF", IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMAGEBRANCHCOLLAPSED", NULL, winTreeSetImageBranchCollapsedAttrib, IUPAF_SAMEASSYSTEM, "IMGCOLLAPSED", IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMAGEBRANCHEXPANDED",  NULL, winTreeSetImageBranchExpandedAttrib, IUPAF_SAMEASSYSTEM, "IMGEXPANDED", IUPAF_NO_INHERIT);

  /* IupTree Attributes - NODES */
  iupClassRegisterAttributeId(ic, "STATE",  winTreeGetStateAttrib,  winTreeSetStateAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "DEPTH",  winTreeGetDepthAttrib,  NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "KIND",   winTreeGetKindAttrib,   NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "PARENT", winTreeGetParentAttrib, NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "NAME",   winTreeGetTitleAttrib,   winTreeSetTitleAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "TITLE",   winTreeGetTitleAttrib,   winTreeSetTitleAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "CHILDCOUNT",   winTreeGetChildCountAttrib,   NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "USERDATA",   winTreeGetUserDataAttrib,   winTreeSetUserDataAttrib, IUPAF_NO_STRING|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "COLOR",  winTreeGetColorAttrib,  winTreeSetColorAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "TITLEFONT", winTreeGetTitleFontAttrib, winTreeSetTitleFontAttrib, IUPAF_NO_INHERIT);

  /* IupTree Attributes - MARKS */
  iupClassRegisterAttributeId(ic, "MARKED",   winTreeGetMarkedAttrib,   winTreeSetMarkedAttrib,   IUPAF_NO_INHERIT);
  iupClassRegisterAttribute  (ic, "MARK",    NULL,    winTreeSetMarkAttrib,    NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute  (ic, "STARTING", NULL, winTreeSetMarkStartAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute  (ic, "MARKSTART", NULL, winTreeSetMarkStartAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  iupClassRegisterAttribute  (ic, "VALUE",    winTreeGetValueAttrib,    winTreeSetValueAttrib,    NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  /* IupTree Attributes - ACTION */
  iupClassRegisterAttributeId(ic, "DELNODE", NULL, winTreeSetDelNodeAttrib, IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "RENAME",  NULL, winTreeSetRenameAttrib,  NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "MOVENODE",  NULL, winTreeSetMoveNodeAttrib,  IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "COPYNODE",  NULL, winTreeSetCopyNodeAttrib,  IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "FINDUSERDATA", winTreeGetFindUserDataAttrib, NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
}
