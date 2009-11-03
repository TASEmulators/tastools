/** \file
 * \brief Tree control
 *
 * See Copyright Notice in iup.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_stdcontrols.h"
#include "iup_layout.h"
#include "iup_tree.h"
#include "iup_assert.h"


#define ITREE_IMG_WIDTH   16
#define ITREE_IMG_HEIGHT  16

static void iTreeInitializeImages(void)
{
  Ihandle *image_leaf, *image_blank, *image_paper;  
  Ihandle *image_collapsed, *image_expanded;  

  unsigned char img_leaf[ITREE_IMG_WIDTH*ITREE_IMG_HEIGHT] = 
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 4, 4, 5, 5, 5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 4, 5, 5, 1, 6, 1, 5, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 4, 4, 5, 5, 1, 6, 1, 5, 0, 0, 0, 0,
    0, 0, 0, 3, 4, 4, 4, 5, 5, 1, 1, 5, 0, 0, 0, 0,
    0, 0, 0, 2, 3, 4, 4, 4, 5, 5, 5, 4, 0, 0, 0, 0,
    0, 0, 0, 2, 3, 3, 4, 4, 4, 5, 4, 4, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 3, 3, 4, 4, 4, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 2, 3, 3, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  unsigned char img_collapsed[ITREE_IMG_WIDTH*ITREE_IMG_HEIGHT] =
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
    0, 0, 2, 2, 2, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0,  
    0, 2, 6, 5, 5, 7, 7, 2, 3, 0, 0, 0, 0, 0, 0, 0, 
    2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 0, 0, 
    2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 4, 3, 0, 
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 1, 4, 3, 0, 
    2, 5, 7, 7, 7, 7, 7, 7, 7, 1, 7, 1, 7, 4, 3, 0, 
    2, 5, 7, 7, 7, 7, 7, 7, 7, 7, 1, 7, 1, 4, 3, 0, 
    2, 5, 7, 7, 7, 7, 7, 1, 7, 1, 7, 1, 7, 4, 3, 0, 
    2, 5, 7, 7, 7, 7, 7, 7, 1, 7, 1, 7, 1, 4, 3, 0, 
    2, 5, 7, 7, 7, 1, 7, 1, 7, 1, 7, 1, 1, 4, 3, 0, 
    2, 5, 1, 7, 1, 7, 1, 7, 1, 7, 1, 1, 1, 4, 3, 0, 
    2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 0,  
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
  };

  unsigned char img_expanded[ITREE_IMG_WIDTH*ITREE_IMG_HEIGHT] =
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 2, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 
    0, 2, 1, 3, 3, 3, 3, 3, 1, 2, 2, 2, 2, 2, 2, 0, 
    0, 2, 1, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 6, 4, 
    0, 2, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 6, 4, 
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6, 3, 6, 4, 
    2, 1, 3, 3, 3, 3, 3, 3, 5, 3, 5, 6, 4, 6, 6, 4, 
    2, 1, 3, 3, 3, 3, 5, 3, 3, 5, 3, 6, 4, 6, 6, 4, 
    0, 2, 0, 3, 3, 3, 3, 3, 5, 3, 5, 5, 2, 4, 2, 4, 
    0, 2, 0, 3, 3, 5, 3, 5, 3, 5, 5, 5, 6, 4, 2, 4, 
    0, 0, 2, 0, 5, 3, 5, 3, 5, 5, 5, 5, 6, 2, 4, 4, 
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 
    0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
  };

  unsigned char img_blank[ITREE_IMG_WIDTH*ITREE_IMG_HEIGHT] =
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 0, 0, 0, 0,
    0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 5, 4, 0, 0, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 4, 0, 0,
    0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0
  };

  unsigned char img_paper[ITREE_IMG_WIDTH*ITREE_IMG_HEIGHT] =
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 0, 0, 0, 0,
    0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 5, 4, 0, 0, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 4, 0, 0,
    0, 0, 3, 1, 4, 3, 4, 3, 4, 3, 4, 2, 2, 2, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 1, 3, 4, 3, 4, 3, 4, 3, 4, 1, 5, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 1, 4, 3, 4, 3, 4, 3, 4, 3, 1, 5, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 1, 3, 4, 3, 4, 3, 4, 3, 4, 1, 5, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 1, 4, 3, 4, 3, 4, 3, 4, 3, 1, 5, 2, 0,
    0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 2, 0,
    0, 0, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0
  };

  image_leaf      = IupImage(ITREE_IMG_WIDTH, ITREE_IMG_HEIGHT, img_leaf);
  image_collapsed = IupImage(ITREE_IMG_WIDTH, ITREE_IMG_HEIGHT, img_collapsed);
  image_expanded  = IupImage(ITREE_IMG_WIDTH, ITREE_IMG_HEIGHT, img_expanded);
  image_blank     = IupImage(ITREE_IMG_WIDTH, ITREE_IMG_HEIGHT, img_blank);
  image_paper     = IupImage(ITREE_IMG_WIDTH, ITREE_IMG_HEIGHT, img_paper);

  IupSetAttribute(image_leaf, "0", "BGCOLOR");
  IupSetAttribute(image_leaf, "1", "192 192 192");
  IupSetAttribute(image_leaf, "2", "56 56 56");
  IupSetAttribute(image_leaf, "3", "99 99 99");
  IupSetAttribute(image_leaf, "4", "128 128 128");
  IupSetAttribute(image_leaf, "5", "161 161 161");
  IupSetAttribute(image_leaf, "6", "222 222 222");

  IupSetAttribute(image_collapsed, "0", "BGCOLOR");
  IupSetAttribute(image_collapsed, "1", "255 206 156");
  IupSetAttribute(image_collapsed, "2", "156 156 0");
  IupSetAttribute(image_collapsed, "3", "0 0 0");
  IupSetAttribute(image_collapsed, "4", "206 206 99");
  IupSetAttribute(image_collapsed, "5", "255 255 206");
  IupSetAttribute(image_collapsed, "6", "247 247 247");
  IupSetAttribute(image_collapsed, "7", "255 255 156");

  IupSetAttribute(image_expanded, "0", "BGCOLOR");
  IupSetAttribute(image_expanded, "1", "255 255 255");
  IupSetAttribute(image_expanded, "2", "156 156 0");
  IupSetAttribute(image_expanded, "3", "255 255 156");
  IupSetAttribute(image_expanded, "4", "0 0 0");
  IupSetAttribute(image_expanded, "5", "255 206 156");
  IupSetAttribute(image_expanded, "6", "206 206 99");

  IupSetAttribute(image_blank, "0", "BGCOLOR");
  IupSetAttribute(image_blank, "1", "255 255 255");
  IupSetAttribute(image_blank, "2", "000 000 000");
  IupSetAttribute(image_blank, "3", "119 119 119");
  IupSetAttribute(image_blank, "4", "136 136 136");
  IupSetAttribute(image_blank, "5", "187 187 187");

  IupSetAttribute(image_paper, "0", "BGCOLOR");
  IupSetAttribute(image_paper, "1", "255 255 255");
  IupSetAttribute(image_paper, "2", "000 000 000");
  IupSetAttribute(image_paper, "3", "119 119 119");
  IupSetAttribute(image_paper, "4", "136 136 136");
  IupSetAttribute(image_paper, "5", "187 187 187");

  IupSetHandle("IMGLEAF",      image_leaf);
  IupSetHandle("IMGCOLLAPSED", image_collapsed);
  IupSetHandle("IMGEXPANDED",  image_expanded);
  IupSetHandle("IMGBLANK",     image_blank);
  IupSetHandle("IMGPAPER",     image_paper);
}

void iupTreeUpdateImages(Ihandle *ih)
{
  int inherit;

  char* value = iupAttribGet(ih, "IMAGELEAF");
  if (!value) value = "IMGLEAF";
  iupClassObjectSetAttribute(ih, "IMAGELEAF", value, &inherit);

  value = iupAttribGet(ih, "IMAGEBRANCHCOLLAPSED");
  if (!value) value = "IMGCOLLAPSED";
  iupClassObjectSetAttribute(ih, "IMAGEBRANCHCOLLAPSED", value, &inherit);

  value = iupAttribGet(ih, "IMAGEBRANCHEXPANDED");
  if (!value) value = "IMGEXPANDED";
  iupClassObjectSetAttribute(ih, "IMAGEBRANCHEXPANDED", value, &inherit);
}

char* iupTreeGetSpacingAttrib(Ihandle* ih)
{
  char *str = iupStrGetMemory(50);
  sprintf(str, "%d", ih->data->spacing);
  return str;
}

static char* iTreeGetMarkModeAttrib(Ihandle* ih)
{
  if (ih->data->mark_mode==ITREE_MARK_SINGLE)
    return "SINGLE";
  else
    return "MULTIPLE";
}

static int iTreeSetMarkModeAttrib(Ihandle* ih, const char* value)
{
  if (iupStrEqualNoCase(value, "MULTIPLE"))
    ih->data->mark_mode = ITREE_MARK_MULTIPLE;    
  else 
    ih->data->mark_mode = ITREE_MARK_SINGLE;
  if (ih->handle)
    iupdrvTreeUpdateMarkMode(ih);
  return 0;
}

static int iTreeSetShiftAttrib(Ihandle* ih, const char* value)
{
  if (iupStrBoolean(value) && iupAttribGetBoolean(ih, "CTRL"))
    iTreeSetMarkModeAttrib(ih, "MULTIPLE");
  else
    iTreeSetMarkModeAttrib(ih, "SINGLE");
  return 1;
}

static int iTreeSetCtrlAttrib(Ihandle* ih, const char* value)
{
  if (iupStrBoolean(value) && iupAttribGetBoolean(ih, "SHIFT"))
    iTreeSetMarkModeAttrib(ih, "MULTIPLE");
  else
    iTreeSetMarkModeAttrib(ih, "SINGLE");
  return 1;
}

static char* iTreeGetShowRenameAttrib(Ihandle* ih)
{
  if (ih->data->show_rename)
    return "YES";
  else
    return "NO";
}

static int iTreeSetShowRenameAttrib(Ihandle* ih, const char* value)
{
  /* valid only before map */
  if (ih->handle)
    return 0;

  if (iupStrBoolean(value))
    ih->data->show_rename = 1;
  else
    ih->data->show_rename = 0;

  return 0;
}

static char* iTreeGetShowDragDropAttrib(Ihandle* ih)
{
  if (ih->data->show_dragdrop)
    return "YES";
  else
    return "NO";
}

static int iTreeSetShowDragDropAttrib(Ihandle* ih, const char* value)
{
  /* valid only before map */
  if (ih->handle)
    return 0;

  if (iupStrBoolean(value))
    ih->data->show_dragdrop = 1;
  else
    ih->data->show_dragdrop = 0;

  return 0;
}

static int iTreeSetAddLeafAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  if (!ih->handle)  /* do not store the action before map */
    return 0;
  iupdrvTreeAddNode(ih, name_id, ITREE_LEAF, value, 1);
  return 0;
}

static int iTreeSetAddBranchAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  if (!ih->handle)  /* do not store the action before map */
    return 0;
  iupdrvTreeAddNode(ih, name_id, ITREE_BRANCH, value, 1);
  return 0;
}

static int iTreeSetInsertLeafAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  if (!ih->handle)  /* do not store the action before map */
    return 0;
  iupdrvTreeAddNode(ih, name_id, ITREE_LEAF, value, 0);
  return 0;
}

static int iTreeSetInsertBranchAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  if (!ih->handle)  /* do not store the action before map */
    return 0;
  iupdrvTreeAddNode(ih, name_id, ITREE_BRANCH, value, 0);
  return 0;
}

static char* iTreeGetAddExpandedAttrib(Ihandle* ih)
{
  if (ih->data->add_expanded)
    return "YES";
  else
    return "NO";
}

static int iTreeSetAddExpandedAttrib(Ihandle* ih, const char* value)
{
  if (iupStrBoolean(value))
    ih->data->add_expanded = 1;
  else
    ih->data->add_expanded = 0;

  return 0;
}

static int iTreeCreateMethod(Ihandle* ih, void **params)
{
  (void)params;

  ih->data = iupALLOCCTRLDATA();

  IupSetAttribute(ih, "RASTERSIZE", "400x200");
  IupSetAttribute(ih, "EXPAND", "YES");

  ih->data->add_expanded = 1;

  return IUP_NOERROR;
}

Ihandle* IupTree(void)
{
  return IupCreate("tree");
}

Iclass* iupTreeGetClass(void)
{
  Iclass* ic = iupClassNew(NULL);

  ic->name = "tree";
  ic->format = NULL; /* no parameters */
  ic->nativetype = IUP_TYPECONTROL;
  ic->childtype = IUP_CHILDNONE;
  ic->is_interactive = 1;
  ic->has_attrib_id = 1;   /* has attributes with IDs that must be parsed */

  /* Class functions */
  ic->Create = iTreeCreateMethod;
  ic->LayoutUpdate = iupdrvBaseLayoutUpdateMethod;
  ic->UnMap = iupdrvBaseUnMapMethod;

  /* Callbacks */
  iupClassRegisterCallback(ic, "SELECTION_CB",      "ii");
  iupClassRegisterCallback(ic, "MULTISELECTION_CB", "Ii");
  iupClassRegisterCallback(ic, "BRANCHOPEN_CB",     "i");
  iupClassRegisterCallback(ic, "BRANCHCLOSE_CB",    "i");
  iupClassRegisterCallback(ic, "EXECUTELEAF_CB",    "i");
  iupClassRegisterCallback(ic, "RENAMENODE_CB",     "is");
  iupClassRegisterCallback(ic, "SHOWRENAME_CB",     "i");
  iupClassRegisterCallback(ic, "RENAME_CB",         "is");
  iupClassRegisterCallback(ic, "DRAGDROP_CB",       "iiii");
  iupClassRegisterCallback(ic, "RIGHTCLICK_CB",     "i");

  /* Common */
  iupBaseRegisterCommonAttrib(ic);

  /* Visual */
  iupBaseRegisterVisualAttrib(ic);

  /* IupTree Attributes - GENERAL */
  iupClassRegisterAttribute(ic, "SHOWDRAGDROP",    iTreeGetShowDragDropAttrib,    iTreeSetShowDragDropAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "SHOWRENAME",      iTreeGetShowRenameAttrib,      iTreeSetShowRenameAttrib,   NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "ADDEXPANDED",     iTreeGetAddExpandedAttrib,     iTreeSetAddExpandedAttrib,  IUPAF_SAMEASSYSTEM, "YES", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* IupTree Attributes - MARKS */
  iupClassRegisterAttribute(ic, "CTRL",  NULL, iTreeSetCtrlAttrib,  NULL, NULL, IUPAF_NOT_MAPPED);
  iupClassRegisterAttribute(ic, "SHIFT", NULL, iTreeSetShiftAttrib, NULL, NULL, IUPAF_NOT_MAPPED);
  iupClassRegisterAttribute(ic, "MARKMODE",  iTreeGetMarkModeAttrib, iTreeSetMarkModeAttrib,  IUPAF_SAMEASSYSTEM, "SINGLE", IUPAF_NOT_MAPPED);

  /* IupTree Attributes - ACTION */
  iupClassRegisterAttributeId(ic, "ADDLEAF",   NULL, iTreeSetAddLeafAttrib,   IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "ADDBRANCH", NULL, iTreeSetAddBranchAttrib, IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "INSERTLEAF",   NULL, iTreeSetInsertLeafAttrib,   IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "INSERTBRANCH", NULL, iTreeSetInsertBranchAttrib, IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  
  /* Default node images */
  iTreeInitializeImages();

  iupdrvTreeInitClass(ic);

  return ic;
}


/********************************************************************************************/


void IupTreeSetAttribute(Ihandle* ih, const char* a, int id, char* v)
{
  char* attr = iupStrGetMemory(50);
  sprintf(attr, "%s%d", a, id);
  IupSetAttribute(ih, attr, v);
}

void IupTreeStoreAttribute(Ihandle* ih, const char* a, int id, char* v)
{
  char* attr = iupStrGetMemory(50);
  sprintf(attr, "%s%d", a, id);
  IupStoreAttribute(ih, attr, v);
}

char* IupTreeGetAttribute(Ihandle* ih, const char* a, int id)
{
  char* attr = iupStrGetMemory(50);
  sprintf(attr, "%s%d", a, id);
  return IupGetAttribute(ih, attr);
}

int IupTreeGetInt(Ihandle* ih, const char* a, int id)
{
  char* attr = iupStrGetMemory(50);
  sprintf(attr, "%s%d", a, id);
  return IupGetInt(ih, attr);
}

float IupTreeGetFloat(Ihandle* ih, const char* a, int id)
{
  char* attr = iupStrGetMemory(50);
  sprintf(attr, "%s%d", a, id);
  return IupGetFloat(ih, attr);
}

void IupTreeSetfAttribute(Ihandle* ih, const char* a, int id, char* f, ...)
{
  static char v[SHRT_MAX];
  char* attr = iupStrGetMemory(50);
  va_list arglist;
  sprintf(attr, "%s%d", a, id);
  va_start(arglist, f);
  vsprintf(v, f, arglist);
  va_end(arglist);
  IupStoreAttribute(ih, attr, v);
}


/************************************************************************************/


int IupTreeSetUserId(Ihandle* ih, int id, void* userdata)
{
  char attr[30];
  sprintf(attr,"USERDATA%d",id);
  IupSetAttribute(ih, attr, userdata);
  return IupGetAttribute(ih, attr)? 1: 0;
}

int IupTreeGetId(Ihandle* ih, void *userdata)
{
  int id = -1;
  char* value;
  char attr[30];
  sprintf(attr,"FINDUSERDATA:%p",userdata);
  value = IupGetAttribute(ih, attr);
  if (!value) return -1;

  iupStrToInt(value, &id);
  return id;
}

void* IupTreeGetUserId(Ihandle* ih, int id)
{
  char attr[30];
  sprintf(attr,"USERDATA%d",id);
  return IupGetAttribute(ih, attr);
}
