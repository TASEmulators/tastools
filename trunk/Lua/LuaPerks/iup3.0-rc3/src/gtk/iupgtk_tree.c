/** \file
 * \brief Tree Control
 *
 * See Copyright Notice in iup.h
 */

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_layout.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_stdcontrols.h"
#include "iup_key.h"
#include "iup_image.h"
#include "iup_array.h"
#include "iup_tree.h"

#include "iup_drvinfo.h"
#include "iupgtk_drv.h"

enum
{
  IUPGTK_TREE_IMAGE,
  IUPGTK_TREE_HAS_IMAGE,
  IUPGTK_TREE_IMAGE_EXPANDED,
  IUPGTK_TREE_HAS_IMAGE_EXPANDED,
  IUPGTK_TREE_TITLE,
  IUPGTK_TREE_KIND,
  IUPGTK_TREE_COLOR,
  IUPGTK_TREE_FONT,
  IUPGTK_TREE_USERDATA
};

static GtkTreeIter gtkTreeInvalidIter = {0,0,0,0};

/*****************************************************************************/
/* COPYING ITEMS (Branches and its children)                                 */
/*****************************************************************************/
/* Insert the copied item in a new location. Returns the new item. */
static void gtkTreeCopyItem(GtkTreeModel* model, GtkTreeIter* iterItem, GtkTreeIter* iterParent, int position, GtkTreeIter *iterNewItem, int full_copy)
{
  GtkTreeStore* store = GTK_TREE_STORE(model);
  int kind;
  char* title;
  gboolean has_image, has_image_expanded;
  PangoFontDescription* font;
  void* userdata;
  GdkColor *color;
  GdkPixbuf* image, *image_expanded;

  gtk_tree_model_get(GTK_TREE_MODEL(store), iterItem, IUPGTK_TREE_IMAGE,      &image,
                                                      IUPGTK_TREE_HAS_IMAGE,      &has_image,
                                                      IUPGTK_TREE_IMAGE_EXPANDED,  &image_expanded,
                                                      IUPGTK_TREE_HAS_IMAGE_EXPANDED,  &has_image_expanded,
                                                      IUPGTK_TREE_TITLE,  &title,
                                                      IUPGTK_TREE_KIND,  &kind,
                                                      IUPGTK_TREE_COLOR, &color, 
                                                      IUPGTK_TREE_FONT, &font, 
                                                      IUPGTK_TREE_USERDATA, &userdata,
                                                      -1);

  if (position == 2)
    gtk_tree_store_append(store, iterNewItem, iterParent);
  else if (position == 1)                                      /* copy as first child of expanded branch */
    gtk_tree_store_insert(store, iterNewItem, iterParent, 0);  /* iterParent is parent of the new item (firstchild of it) */
  else                                                                  /* copy as next brother of item or collapsed branch */
    gtk_tree_store_insert_after(store, iterNewItem, NULL, iterParent);  /* iterParent is sibling of the new item */

  if (full_copy) /* during a full copy the userdata reference is not copied */
    userdata = NULL;

  gtk_tree_store_set(store, iterNewItem,  IUPGTK_TREE_IMAGE,      image,
                                          IUPGTK_TREE_HAS_IMAGE,  has_image,
                                          IUPGTK_TREE_IMAGE_EXPANDED,  image_expanded,
                                          IUPGTK_TREE_HAS_IMAGE_EXPANDED, has_image_expanded,
                                          IUPGTK_TREE_TITLE,  title,
                                          IUPGTK_TREE_KIND,  kind,
                                          IUPGTK_TREE_COLOR, color, 
                                          IUPGTK_TREE_FONT, font,
                                          IUPGTK_TREE_USERDATA, userdata,
                                          -1);
}

static void gtkTreeCopyChildren(Ihandle* ih, GtkTreeModel* model, GtkTreeIter *iterItemSrc, GtkTreeIter *iterItemDst, int full_copy)
{
  GtkTreeIter iterChildSrc;
  int hasItem = gtk_tree_model_iter_children(model, &iterChildSrc, iterItemSrc);  /* get the firstchild */
  while(hasItem)
  {
    GtkTreeIter iterNewItem;
    gtkTreeCopyItem(model, &iterChildSrc, iterItemDst, 2, &iterNewItem, full_copy);  /* append always */

    /* Recursively transfer all the items */
    gtkTreeCopyChildren(ih, model, &iterChildSrc, &iterNewItem, full_copy);  

    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, &iterChildSrc);
  }
}

/* Copies all items in a branch to a new location. Returns the new branch node. */
static void gtkTreeCopyNode(Ihandle* ih, GtkTreeModel* model, GtkTreeIter *iterItemSrc, GtkTreeIter *iterItemDst, GtkTreeIter* iterNewItem, int full_copy)
{
  int kind, position = 0; /* insert after iterItemDst */
  gtk_tree_model_get(model, iterItemDst, IUPGTK_TREE_KIND, &kind, -1);

  if (kind == ITREE_BRANCH)
  {
    GtkTreePath* path = gtk_tree_model_get_path(model, iterItemDst);
    if (gtk_tree_view_row_expanded(GTK_TREE_VIEW(ih->handle), path))
      position = 1;  /* insert as first child of iterItemDst */
    gtk_tree_path_free(path);
  }

  gtkTreeCopyItem(model, iterItemSrc, iterItemDst, position, iterNewItem, full_copy);  

  gtkTreeCopyChildren(ih, model, iterItemSrc, iterNewItem, full_copy);
}

/*****************************************************************************/
/* FINDING ITEMS                                                             */
/*****************************************************************************/

static void gtkTreeInvertAllNodeMarking(Ihandle* ih, GtkTreeModel* model, GtkTreeSelection* selection, GtkTreeIter* iterItem)
{
  GtkTreeIter iterChild;
  int hasItem = TRUE;

  while(hasItem)
  {
    if(gtk_tree_selection_iter_is_selected(selection, iterItem))
      gtk_tree_selection_unselect_iter(selection, iterItem);
    else
      gtk_tree_selection_select_iter(selection, iterItem);

    /* Check whether we have child items */
    if(gtk_tree_model_iter_has_child(model, iterItem))
    {
      gtk_tree_model_iter_children(model, &iterChild, iterItem);  /* get the firstchild */
      gtkTreeInvertAllNodeMarking(ih, model, selection, &iterChild);
    }

    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, iterItem);
  }
}

static GtkTreeIter gtkTreeFindVisibleNodeId(Ihandle* ih, GtkTreeModel* model, GtkTreeIter iterItem, GtkTreeIter iterNode)
{
  GtkTreeIter iterChild;
  GtkTreePath* path;
  int hasItem = TRUE;

  while(hasItem)
  {
    /* ID control to traverse items */
    ih->data->id_control++;   /* not the real id since it counts only the visible ones */

    /* StateID founded! */
    if(iterItem.user_data == iterNode.user_data)
      return iterItem;

    path = gtk_tree_model_get_path(model, &iterItem);

    /* Check whether we have child items and it is expanded (visible) */
    if (gtk_tree_model_iter_has_child(model, &iterItem) && gtk_tree_view_row_expanded(GTK_TREE_VIEW(ih->handle), path))
    {
      gtk_tree_model_iter_children(model, &iterChild, &iterItem);  /* get the firstchild */
      iterChild = gtkTreeFindVisibleNodeId(ih, model, iterChild, iterNode);

      /* StateID founded! */
      if(iterChild.user_data)
      {
        gtk_tree_path_free(path);
        return iterChild;
      }
    }

    gtk_tree_path_free(path);
    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, &iterItem);
  }

  return gtkTreeInvalidIter; /* invalid since gtk_tree_model_iter_next returned false */
}

static GtkTreeIter gtkTreeFindVisibleNodeFromId(Ihandle* ih, GtkTreeModel* model, GtkTreeIter iterItem)
{
  GtkTreeIter iterChild;
  GtkTreePath* path;
  int hasItem = TRUE;

  while(hasItem)
  {
    /* ID control to traverse items */
    ih->data->id_control--;   /* not the real id since it counts only the visible ones */

    /* StateID founded! */
    if(ih->data->id_control < 0)
      return iterItem;

    path = gtk_tree_model_get_path(model, &iterItem);

    /* Check whether we have child items and it is expanded (visible) */
    if(gtk_tree_model_iter_has_child(model, &iterItem) && gtk_tree_view_row_expanded(GTK_TREE_VIEW(ih->handle), path))
    {
      gtk_tree_model_iter_children(model, &iterChild, &iterItem);  /* get the firstchild */
      iterChild = gtkTreeFindVisibleNodeFromId(ih, model, iterChild);

      /* StateID founded! */
      if(ih->data->id_control < 0)
      {
        gtk_tree_path_free(path);
        return iterChild;
      }
    }

    gtk_tree_path_free(path);
    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, &iterItem);
  }

  return gtkTreeInvalidIter; /* invalid since gtk_tree_model_iter_next returned false */
}

static GtkTreeIter gtkTreeGetLastVisibleNode(Ihandle* ih, GtkTreeModel* model, GtkTreeIter iterItem)
{
  GtkTreeIter iterChild, iterPrev = gtkTreeInvalidIter;
  GtkTreePath* path = gtk_tree_model_get_path(model, &iterItem);

  /* Check whether we have child items and it is expanded (visible) */
  if(gtk_tree_model_iter_has_child(model, &iterItem) && gtk_tree_view_row_expanded(GTK_TREE_VIEW(ih->handle), path))
  {
    int hasItem = TRUE;
    gtk_tree_model_iter_children(model, &iterChild, &iterItem);  /* get the firstchild */

    while(hasItem)
    {
      iterPrev = iterChild;

      /* Go to next sibling item */
      hasItem = gtk_tree_model_iter_next(model, &iterChild);
    }

    iterItem = gtkTreeGetLastVisibleNode(ih, model, iterPrev);
  }
  gtk_tree_path_free(path);

  return iterItem;
}

static GtkTreeIter gtkTreeFindNodeID(Ihandle* ih, GtkTreeModel* model, GtkTreeIter iterItem, GtkTreeIter iterNode)
{
  GtkTreeIter iterChild;
  int hasItem = TRUE;

  while(hasItem)
  {
    /* ID control to traverse items */
    ih->data->id_control++;

    /* StateID founded! */
    if (iterItem.user_data == iterNode.user_data)
      return iterItem;

    /* Check whether we have child items */
    if (gtk_tree_model_iter_has_child(model, &iterItem))
    {
      gtk_tree_model_iter_children(model, &iterChild, &iterItem);  /* get the firstchild */
      iterChild = gtkTreeFindNodeID(ih, model, iterChild, iterNode);

      /* StateID founded! */
      if(iterChild.user_data)
        return iterChild;
    }

    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, &iterItem);
  }

  return gtkTreeInvalidIter; /* invalid since gtk_tree_model_iter_next returned false */
}

static int gtkTreeGetNodeId(Ihandle* ih, GtkTreeIter iterItem)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterRoot;
  gtk_tree_model_get_iter_first(model, &iterRoot);

  ih->data->id_control = -1;
  iterItem = gtkTreeFindNodeID(ih, model, iterRoot, iterItem);
  if (iterItem.user_data)
    return ih->data->id_control;
  else
    return -1;
}

static GtkTreeIter gtkTreeFindUserData(Ihandle* ih, GtkTreeModel* model, GtkTreeIter iterItem, void* userdata)
{
  GtkTreeIter iterChild;
  int hasItem = TRUE;
  void* node_userdata;

  while(hasItem)
  {
    /* ID control to traverse items */
    ih->data->id_control++;

    gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_USERDATA, &node_userdata, -1);

    /* userdata founded! */
    if (node_userdata == userdata)
      return iterItem;

    /* Check whether we have child items */
    if (gtk_tree_model_iter_has_child(model, &iterItem))
    {
      gtk_tree_model_iter_children(model, &iterChild, &iterItem);  /* get the firstchild */
      iterChild = gtkTreeFindUserData(ih, model, iterChild, userdata);

      /* userdata founded! */
      if (iterChild.user_data)
        return iterChild;
    }

    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, &iterItem);
  }

  return gtkTreeInvalidIter; /* invalid since gtk_tree_model_iter_next returned false */
}

static int gtkTreeGetUserDataId(Ihandle* ih, GtkTreeModel* model, void* userdata)
{
  GtkTreeIter iterRoot, iterItem;
  gtk_tree_model_get_iter_first(model, &iterRoot);

  ih->data->id_control = -1;
  iterItem = gtkTreeFindUserData(ih, model, iterRoot, userdata);
  if (iterItem.user_data)
    return ih->data->id_control;
  else
    return -1;
}

static void gtkTreeCallNodeRemovedRec(Ihandle* ih, GtkTreeModel* model, GtkTreeIter iterItem, IFnis cb)
{
  GtkTreeIter iterChild;
  int hasItem = TRUE;
  void* node_userdata;

  while(hasItem)
  {
    /* ID control to traverse items */
    ih->data->id_control++;

    gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_USERDATA, &node_userdata, -1);

    cb(ih, ih->data->id_control, (char*)node_userdata);

    /* Check whether we have child items */
    if (gtk_tree_model_iter_has_child(model, &iterItem))
    {
      gtk_tree_model_iter_children(model, &iterChild, &iterItem);  /* get the firstchild */
      gtkTreeCallNodeRemovedRec(ih, model, iterChild, cb);
    }

    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, &iterItem);
  }
}

static void gtkTreeCallNodeRemoved(Ihandle* ih, GtkTreeModel* model, GtkTreeIter *iterItem)
{
  IFnis cb = (IFnis)IupGetCallback(ih, "NODEREMOVED_CB");
  if (cb) 
  {
    ih->data->id_control = gtkTreeGetNodeId(ih, *iterItem)-1;
    gtkTreeCallNodeRemovedRec(ih, model, *iterItem, cb);
  }
}

static gboolean gtkTreeFindNodeFromID(Ihandle* ih, GtkTreeModel* model, GtkTreeIter *iterItem, int *id)
{
  GtkTreeIter iterChild;
  int hasItem = TRUE;

  while(hasItem)
  {
    /* ID control to traverse items */
    (*id)--;

    /* StateID founded! */
    if (*id < 0)
      return TRUE;

    /* Check whether we have child items */
    if (gtk_tree_model_iter_has_child(model, iterItem))
    {
      gtk_tree_model_iter_children(model, &iterChild, iterItem);  /* get the firstchild */
      
      if (gtkTreeFindNodeFromID(ih, model, &iterChild, id))
      {
        *iterItem = iterChild;
        return TRUE;
      }
    }

    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, iterItem);
  }

  return FALSE;
}

static gboolean gtkTreeFindNodeFromString(Ihandle* ih, GtkTreeModel* model, const char* name_id, GtkTreeIter *iterItem)
{
  if (name_id[0])
  {
    int id;
    if (iupStrToInt(name_id, &id))
    {
      gtk_tree_model_get_iter_first(model, iterItem);
      return gtkTreeFindNodeFromID(ih, model, iterItem, &id);
    }
  }
  else
  {
    GtkTreePath* path = NULL;
    gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &path, NULL);
    if (path)
    {
      gtk_tree_model_get_iter(model, iterItem, path);
      gtk_tree_path_free(path);
      return TRUE;
    }
  }
  return FALSE;
}

/*****************************************************************************/
/* MANIPULATING IMAGES                                                       */
/*****************************************************************************/
static void gtkTreeUpdateImages(Ihandle* ih, GtkTreeModel* model, GtkTreeIter iterItem, int mode)
{
  GtkTreeIter iterChild;
  int hasItem = TRUE;
  int kind;

  while(hasItem)
  {
    gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_KIND, &kind, -1);

    if (kind == ITREE_BRANCH)
    {
      if (mode == ITREE_UPDATEIMAGE_EXPANDED)
      {
        gboolean has_image_expanded = FALSE;
        gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_HAS_IMAGE_EXPANDED, &has_image_expanded, -1);
        if (!has_image_expanded)
          gtk_tree_store_set(GTK_TREE_STORE(model), &iterItem, IUPGTK_TREE_IMAGE_EXPANDED, ih->data->def_image_expanded, -1);
      }
      else if(mode == ITREE_UPDATEIMAGE_COLLAPSED)
      {
        gboolean has_image = FALSE;
        gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_HAS_IMAGE, &has_image, -1);
        if (!has_image)
          gtk_tree_store_set(GTK_TREE_STORE(model), &iterItem, IUPGTK_TREE_IMAGE, ih->data->def_image_collapsed, -1);
      }

      if (gtk_tree_model_iter_has_child(model, &iterItem))
      {

        /* Recursively traverse child items */
        gtk_tree_model_iter_children(model, &iterChild, &iterItem);
        gtkTreeUpdateImages(ih, model, iterChild, mode);
      }
    }
    else 
    {
      if (mode == ITREE_UPDATEIMAGE_LEAF)
      {
        gboolean has_image = FALSE;
        gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_HAS_IMAGE, &has_image, -1);
        if (!has_image)
          gtk_tree_store_set(GTK_TREE_STORE(model), &iterItem, IUPGTK_TREE_IMAGE, ih->data->def_image_leaf, -1);
      }
    }

    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, &iterItem);
  }
}

static void gtkTreeExpandItem(Ihandle* ih, GtkTreePath* path, int expand)
{
  if (expand == -1)
    expand = !gtk_tree_view_row_expanded(GTK_TREE_VIEW(ih->handle), path); /* toggle */

  if (expand)
    gtk_tree_view_expand_row(GTK_TREE_VIEW(ih->handle), path, FALSE);
  else
    gtk_tree_view_collapse_row(GTK_TREE_VIEW(ih->handle), path);
}

int iupgtkGetColor(const char* value, GdkColor *color)
{
  unsigned char r, g, b;
  if (iupStrToRGB(value, &r, &g, &b))
  {
    iupgdkColorSet(color, r, g, b);
    return 1;
  }
  return 0;
}

/*****************************************************************************/
/* ADDING ITEMS                                                              */
/*****************************************************************************/
void iupdrvTreeAddNode(Ihandle* ih, const char* name_id, int kind, const char* title, int add)
{
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GtkTreeIter iterPrev, iterNewItem, iterParent;
  GtkTreePath* path;
  GdkColor color = {0L,0,0,0};
  int kindPrev;

  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), name_id, &iterPrev))
    return;

  gtk_tree_model_get(GTK_TREE_MODEL(store), &iterPrev, IUPGTK_TREE_KIND, &kindPrev, -1);

  if (kindPrev == ITREE_BRANCH && add)
    gtk_tree_store_insert(store, &iterNewItem, &iterPrev, 0);  /* iterPrev is parent of the new item (firstchild of it) */
  else
    gtk_tree_store_insert_after(store, &iterNewItem, NULL, &iterPrev);  /* iterPrev is sibling of the new item */

  iupgtkGetColor(iupAttribGetStr(ih, "FGCOLOR"), &color);

  /* set the attributes of the new node */
  gtk_tree_store_set(store, &iterNewItem, IUPGTK_TREE_HAS_IMAGE, FALSE,
                                          IUPGTK_TREE_HAS_IMAGE_EXPANDED, FALSE,
                                          IUPGTK_TREE_TITLE, iupgtkStrConvertToUTF8(title),
                                          IUPGTK_TREE_KIND, kind,
                                          IUPGTK_TREE_COLOR, &color, -1);

  if (kind == ITREE_LEAF)
    gtk_tree_store_set(store, &iterNewItem, IUPGTK_TREE_IMAGE, ih->data->def_image_leaf, -1);
  else
    gtk_tree_store_set(store, &iterNewItem, IUPGTK_TREE_IMAGE, ih->data->def_image_collapsed,
                                            IUPGTK_TREE_IMAGE_EXPANDED, ih->data->def_image_expanded, -1);

  if (kindPrev == ITREE_BRANCH && add)
    iterParent = iterPrev;
  else
    gtk_tree_model_iter_parent(GTK_TREE_MODEL(store), &iterParent, &iterNewItem);

  /* If this is the first child of the parent, then handle the ADDEXPANDED attribute */
  if (gtk_tree_model_iter_n_children(GTK_TREE_MODEL(store), &iterParent) == 1)
  {
    int depth;
    path = gtk_tree_model_get_path(GTK_TREE_MODEL(store), &iterParent);
    depth = gtk_tree_path_get_depth(path)-1;
    if (ih->data->add_expanded || depth==0)  /* if this is the first child of the root, expand always */
    {
      iupAttribSetStr(ih, "_IUPTREE_IGNORE_BRANCHOPEN_CB", "1");
      gtk_tree_view_expand_row(GTK_TREE_VIEW(ih->handle), path, FALSE);
    }
    else
      gtk_tree_view_collapse_row(GTK_TREE_VIEW(ih->handle), path);
    gtk_tree_path_free(path);
  }
}

static void gtkTreeAddRootNode(Ihandle* ih)
{
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GtkTreePath* path;
  GtkTreeIter  iterRoot;
  GdkColor color = {0L,0,0,0};

  iupgtkGetColor(iupAttribGetStr(ih, "FGCOLOR"), &color);

  gtk_tree_store_append(store, &iterRoot, NULL);  /* root node */
  gtk_tree_store_set(store, &iterRoot, IUPGTK_TREE_IMAGE, ih->data->def_image_collapsed,
                                       IUPGTK_TREE_HAS_IMAGE, FALSE,
                                       IUPGTK_TREE_IMAGE_EXPANDED, ih->data->def_image_expanded,
                                       IUPGTK_TREE_HAS_IMAGE_EXPANDED, FALSE,
                                       IUPGTK_TREE_KIND, ITREE_BRANCH,
                                       IUPGTK_TREE_COLOR, &color, -1);

  path = gtk_tree_model_get_path(GTK_TREE_MODEL(store), &iterRoot);
  /* MarkStart node */
  iupAttribSetStr(ih, "_IUPTREE_MARKSTART_NODE", (char*)path);

  /* Set the default VALUE */
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(ih->handle), path, NULL, FALSE);
}

/*****************************************************************************/
/* AUXILIAR FUNCTIONS                                                        */
/*****************************************************************************/
static void gtkTreeOpenCloseEvent(Ihandle* ih)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  GtkTreePath* path;
  int kind;

  if (!gtkTreeFindNodeFromString(ih, model, "", &iterItem))
    return;

  path = gtk_tree_model_get_path(model, &iterItem);
  if (path)
  {
    gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_KIND, &kind, -1);

    if (kind == ITREE_LEAF)  /* leafs */
      gtk_tree_view_row_activated(GTK_TREE_VIEW(ih->handle), path, (GtkTreeViewColumn*)iupAttribGet(ih, "_IUPGTK_COLUMN"));     
    else  /* branches */
      gtkTreeExpandItem(ih, path, -1); /* toggle */

    gtk_tree_path_free(path);
  }
}

static gboolean gtkTreeSelected_Foreach_Func(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, GList **rowref_list)
{
  GtkTreeRowReference *rowref;

  rowref = gtk_tree_row_reference_new(model, path);
  *rowref_list = g_list_append(*rowref_list, rowref);

  (void)iter;
  return FALSE; /* do not stop walking the store, call us with next row */
}

static gboolean gtkTreeSelected_Iter_Func(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, GList **rowref_list)
{
  GtkTreeRowReference *rowref;
  GtkTreeIter iterParent;
  if (!gtk_tree_model_iter_parent(model, &iterParent, iter)) /* the root node can't be deleted */
    return FALSE; /* do not stop walking the store, call us with next row */

  rowref = gtk_tree_row_reference_new(model, path);
  *rowref_list = g_list_append(*rowref_list, rowref);

  return FALSE; /* do not stop walking the store, call us with next row */
}

/*****************************************************************************/
/* CALLBACKS                                                                 */
/*****************************************************************************/
static void gtkTreeCallMultiSelectionCb(Ihandle* ih)
{
  IFnIi cbMulti = (IFnIi)IupGetCallback(ih, "MULTISELECTION_CB");
  IFnii cbSelec = (IFnii)IupGetCallback(ih, "SELECTION_CB");
  if (cbMulti || cbSelec)
  {
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));
    GtkTreeIter iterRoot;
    GList *rr_list = NULL;
    GList *node;
    int* id_rowItem;
    int count_selected_rows, i = 0;

    gtk_tree_model_get_iter_first(model, &iterRoot);

    gtk_tree_selection_selected_foreach(selection, (GtkTreeSelectionForeachFunc)gtkTreeSelected_Foreach_Func, &rr_list);
    count_selected_rows = g_list_length(rr_list);
    id_rowItem = malloc(sizeof(int) * count_selected_rows);

    for(node = rr_list; node != NULL; node = node->next)
    {
      GtkTreePath* path = gtk_tree_row_reference_get_path(node->data);
      if (path)
      {
        GtkTreeIter iterItem;
        gtk_tree_model_get_iter(model, &iterItem, path);

        id_rowItem[i] = gtkTreeGetNodeId(ih, iterItem);
        i++;

        gtk_tree_path_free(path);
      }
    }

    g_list_foreach(rr_list, (GFunc) gtk_tree_row_reference_free, NULL);
    g_list_free(rr_list);

    if (cbMulti)
      cbMulti(ih, id_rowItem, count_selected_rows);
    else
    {
      for (i=0; i<count_selected_rows; i++)
        cbSelec(ih, id_rowItem[i], 1);
    }

    free(id_rowItem);
  }
}


/*****************************************************************************/
/* GET AND SET ATTRIBUTES                                                    */
/*****************************************************************************/


static char* gtkTreeGetIndentationAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(255);
#if GTK_CHECK_VERSION(2, 12, 0)
  int indent = gtk_tree_view_get_level_indentation(GTK_TREE_VIEW(ih->handle));
#else
  int indent = 0;
#endif
  sprintf(str, "%d", indent);
  return str;
}

static int gtkTreeSetIndentationAttrib(Ihandle* ih, const char* value)
{
#if GTK_CHECK_VERSION(2, 12, 0)
  int indent;
  if (iupStrToInt(value, &indent))
    gtk_tree_view_set_level_indentation(GTK_TREE_VIEW(ih->handle), indent);
#endif
  return 0;
}

static int gtkTreeSetTopItemAttrib(Ihandle* ih, const char* value)
{
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GtkTreeIter iterItem;
  GtkTreePath* path;

  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), value, &iterItem))
    return 0;

  path = gtk_tree_model_get_path(GTK_TREE_MODEL(store), &iterItem);

  if (!gtk_tree_view_row_expanded(GTK_TREE_VIEW(ih->handle), path))
    gtk_tree_view_expand_to_path(GTK_TREE_VIEW(ih->handle), path);

  gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(ih->handle), path, NULL, FALSE, 0, 0);  /* scroll to visible */

  gtk_tree_path_free(path);
 
  return 0;
}

static int gtkTreeSetSpacingAttrib(Ihandle* ih, const char* value)
{
  if(!iupStrToInt(value, &ih->data->spacing))
    ih->data->spacing = 1;

  if(ih->data->spacing < 1)
    ih->data->spacing = 1;

  if (ih->handle)
  {
    GtkCellRenderer *renderer_img = (GtkCellRenderer*)iupAttribGet(ih, "_IUPGTK_RENDERER_IMG");
    GtkCellRenderer *renderer_txt = (GtkCellRenderer*)iupAttribGet(ih, "_IUPGTK_RENDERER_TEXT");
    g_object_set(G_OBJECT(renderer_img), "ypad", ih->data->spacing, NULL);
    g_object_set(G_OBJECT(renderer_txt), "ypad", ih->data->spacing, NULL);
    return 0;
  }
  else
    return 1; /* store until not mapped, when mapped will be set again */
}

static int gtkTreeSetExpandAllAttrib(Ihandle* ih, const char* value)
{
  if (iupStrBoolean(value))
    gtk_tree_view_expand_all(GTK_TREE_VIEW(ih->handle));
  else
  {
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
    GtkTreeIter  iterRoot;
    GtkTreePath* pathRoot;

    gtk_tree_view_collapse_all(GTK_TREE_VIEW(ih->handle));

    /* The root node is always expanded */
    gtk_tree_model_get_iter_first(model, &iterRoot);
    pathRoot = gtk_tree_model_get_path(model, &iterRoot);
    gtk_tree_view_expand_row(GTK_TREE_VIEW(ih->handle), pathRoot, FALSE);
    gtk_tree_path_free(pathRoot);
  }

  return 0;
}

static char* gtkTreeGetDepthAttrib(Ihandle* ih, const char* name_id)
{
  char* depth;
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), name_id, &iterItem))
    return NULL;

  depth = iupStrGetMemory(10);
  sprintf(depth, "%d", gtk_tree_store_iter_depth(store, &iterItem));
  return depth;
}

static int gtkTreeSetMoveNodeAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  GtkTreeModel* model;
  GtkTreeIter iterItemSrc, iterItemDst, iterNewItem;
  GtkTreeIter iterParent, iterNextParent;

  if (!ih->handle)  /* do not store the action before map */
    return 0;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItemSrc))
    return 0;

  if (!gtkTreeFindNodeFromString(ih, model, value, &iterItemDst))
    return 0;

  /* If Drag item is an ancestor of Drop item then return */
  iterParent = iterItemDst;
  while(gtk_tree_model_iter_parent(model, &iterNextParent, &iterParent))
  {
    if (iterNextParent.user_data == iterItemSrc.user_data)
      return 0;
    iterParent = iterNextParent;
  }

  /* Copying the node and its children to the new position */
  gtkTreeCopyNode(ih, model, &iterItemSrc, &iterItemDst, &iterNewItem, 0);  /* not a full copy, preserve user data */

  /* Deleting the node of its old position */
  /* do not delete the user data, we copy the references in CopyNode */
  gtk_tree_store_remove(GTK_TREE_STORE(model), &iterItemSrc);

  return 0;
}

static int gtkTreeSetCopyNodeAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  GtkTreeModel* model;
  GtkTreeIter iterItemSrc, iterItemDst, iterNewItem;
  GtkTreeIter iterParent, iterNextParent;

  if (!ih->handle)  /* do not store the action before map */
    return 0;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItemSrc))
    return 0;

  if (!gtkTreeFindNodeFromString(ih, model, value, &iterItemDst))
    return 0;

  /* If Drag item is an ancestor of Drop item then return */
  iterParent = iterItemDst;
  while(gtk_tree_model_iter_parent(model, &iterNextParent, &iterParent))
  {
    if (iterNextParent.user_data == iterItemSrc.user_data)
      return 0;
    iterParent = iterNextParent;
  }

  /* Copying the node and its children to the new position */
  gtkTreeCopyNode(ih, model, &iterItemSrc, &iterItemDst, &iterNewItem, 1);

  return 0;
}

static char* gtkTreeGetColorAttrib(Ihandle* ih, const char* name_id)
{
  char* str;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  GdkColor *color;

  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return NULL;

  gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_COLOR, &color, -1);
  if (!color)
    return NULL;

  str = iupStrGetMemory(20);
  sprintf(str, "%d %d %d", iupCOLOR16TO8(color->red),
                           iupCOLOR16TO8(color->green),
                           iupCOLOR16TO8(color->blue));
  return str;
}

static int gtkTreeSetColorAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GtkTreeIter iterItem;
  GdkColor color;
  unsigned char r, g, b;

  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), name_id, &iterItem))
    return 0;

  if (!iupStrToRGB(value, &r, &g, &b))
    return 0;

  iupgdkColorSet(&color, r, g, b);
  gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_COLOR, &color, -1);

  return 0;
}

static char* gtkTreeGetParentAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  GtkTreeIter iterParent;
  char* str;

  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return NULL;

  if (!gtk_tree_model_iter_parent(model, &iterParent, &iterItem))
    return NULL;

  str = iupStrGetMemory(10);
  sprintf(str, "%d", gtkTreeGetNodeId(ih, iterParent));
  return str;
}

static char* gtkTreeGetChildCountAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  char* str;

  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return NULL;

  str = iupStrGetMemory(10);
  sprintf(str, "%d", gtk_tree_model_iter_n_children(model, &iterItem));
  return str;
}

static int gtkTreeCount(GtkTreeModel* model, GtkTreeIter iterBranch)
{
  GtkTreeIter iterChild;
  int count = 0;
  int hasItem = gtk_tree_model_iter_children(model, &iterChild, &iterBranch);  /* get the firstchild */
  count++;
  while(hasItem)
  {
    count += gtkTreeCount(model, iterChild);

    /* Go to next sibling item */
    hasItem = gtk_tree_model_iter_next(model, &iterChild);
  }

  return count;
}

static char* gtkTreeGetCountAttrib(Ihandle* ih)
{
  GtkTreeIter iterRoot;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  char* str = iupStrGetMemory(10);
  gtk_tree_model_get_iter_first(model, &iterRoot);
  sprintf(str, "%d", gtkTreeCount(model, iterRoot));
  return str;
}

static char* gtkTreeGetKindAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  int kind;

  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return NULL;

  gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_KIND, &kind, -1);

  if(!kind)
    return "BRANCH";
  else
    return "LEAF";
}

static char* gtkTreeGetStateAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;

  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return NULL;

  if (gtk_tree_model_iter_has_child(model, &iterItem))
  {
    GtkTreePath* path = gtk_tree_model_get_path(model, &iterItem);
    int expanded = gtk_tree_view_row_expanded(GTK_TREE_VIEW(ih->handle), path);
    gtk_tree_path_free(path);

    if (expanded)
      return "EXPANDED";
    else
      return "COLLAPSED";
  }

  return NULL;
}

static int gtkTreeSetStateAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  GtkTreePath* path;

  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return 0;

  path = gtk_tree_model_get_path(model, &iterItem);
  gtkTreeExpandItem(ih, path, iupStrEqualNoCase(value, "EXPANDED"));
  gtk_tree_path_free(path);

  return 0;
}

static char* gtkTreeGetTitle(GtkTreeModel* model, GtkTreeIter iterItem)
{
  char* title;
  gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_TITLE, &title, -1);
  return iupgtkStrConvertFromUTF8(title);
}

static char* gtkTreeGetTitleAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return NULL;
  return gtkTreeGetTitle(model, iterItem);
}

static int gtkTreeSetTitleAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), name_id, &iterItem))
    return 0;
  gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_TITLE, iupgtkStrConvertToUTF8(value), -1);
  return 0;
}

static int gtkTreeSetTitleFontAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  PangoFontDescription* fontdesc = NULL;
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), name_id, &iterItem))
    return 0;
  if (value)
    fontdesc = iupgtkGetPangoFontDesc(value);
  gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_FONT, fontdesc, -1);
  return 0;
}

static char* gtkTreeGetTitleFontAttrib(Ihandle* ih, const char* name_id)
{
  PangoFontDescription* fontdesc;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return NULL;
  gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_FONT, &fontdesc, -1);
  return pango_font_description_to_string(fontdesc);
}

static char* gtkTreeGetFindUserDataAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  int id;
  char* str = (char*)(name_id+1); /* skip ':' */
  void* userdata = NULL;
  if (sscanf(str, "%p", &userdata)!=1)
    return NULL;
  id = gtkTreeGetUserDataId(ih, model, userdata);
  if (id == -1)
    return NULL;
  str = iupStrGetMemory(16);
  sprintf(str, "%d", id);
  return str;
}

static char* gtkTreeGetUserDataAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  char* userdata;
  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return NULL;
  gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_USERDATA, &userdata, -1);
  return userdata;
}

static int gtkTreeSetUserDataAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), name_id, &iterItem))
    return 0;
  gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_USERDATA, value, -1);
  return 0;
}

static char* gtkTreeGetValueAttrib(Ihandle* ih)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreePath* path = NULL;
  char* str;

  gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &path, NULL);
  if (path)
  {
    GtkTreeIter iterItem;
    gtk_tree_model_get_iter(model, &iterItem, path);
    gtk_tree_path_free(path);

    str = iupStrGetMemory(16);
    sprintf(str, "%d", gtkTreeGetNodeId(ih, iterItem));
    return str;
  }

  return "0"; /* default VALUE is root */
}

static int gtkTreeSetMarkAttrib(Ihandle* ih, const char* value)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterRoot;

  if (ih->data->mark_mode==ITREE_MARK_SINGLE)
    return 0;

  gtk_tree_model_get_iter_first(model, &iterRoot);

  if(iupStrEqualNoCase(value, "BLOCK"))
  {
    GtkTreePath* pathFocus;
    gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &pathFocus, NULL);
    gtk_tree_selection_select_range(selection, (GtkTreePath*)iupAttribGet(ih, "_IUPTREE_MARKSTART_NODE"), pathFocus);
    gtk_tree_path_free(pathFocus);
  }
  else if(iupStrEqualNoCase(value, "CLEARALL"))
    gtk_tree_selection_unselect_all(selection);
  else if(iupStrEqualNoCase(value, "MARKALL"))
    gtk_tree_selection_select_all(selection);
  else if(iupStrEqualNoCase(value, "INVERTALL")) /* INVERTALL *MUST* appear before INVERT, or else INVERTALL will never be called. */
    gtkTreeInvertAllNodeMarking(ih, model, selection, &iterRoot);
  else if(iupStrEqualPartial(value, "INVERT"))
  {
    /* iupStrEqualPartial allows the use of "INVERTid" form */
    GtkTreeIter iterItem;
    if (!gtkTreeFindNodeFromString(ih, model, &value[strlen("INVERT")], &iterItem))
      return 0;

    if(gtk_tree_selection_iter_is_selected(selection, &iterItem))
      gtk_tree_selection_unselect_iter(selection, &iterItem);
    else
      gtk_tree_selection_select_iter(selection, &iterItem);
  }
  else
  {
    GtkTreePath *path1, *path2;
    GtkTreeIter iterItem1, iterItem2;
    char str1[50], str2[50];
    if (iupStrToStrStr(value, str1, str2, '-')!=2)
      return 0;

    if (!gtkTreeFindNodeFromString(ih, model, str1, &iterItem1))
      return 0;
    if (!gtkTreeFindNodeFromString(ih, model, str2, &iterItem2))
      return 0;

    path1 = gtk_tree_model_get_path(model, &iterItem1);
    path2 = gtk_tree_model_get_path(model, &iterItem2);
    gtk_tree_selection_select_range(selection, path1, path2);
    gtk_tree_path_free(path1);
    gtk_tree_path_free(path2);
  }

  return 1;
} 

static int gtkTreeSetValueAttrib(Ihandle* ih, const char* value)
{
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterRoot, iterItem;
  GtkTreePath* path;

  if (gtkTreeSetMarkAttrib(ih, value))
    return 0;

  gtk_tree_model_get_iter_first(model, &iterRoot);

  if (iupStrEqualNoCase(value, "ROOT"))
    iterItem = iterRoot;
  else if(iupStrEqualNoCase(value, "LAST"))
    iterItem = gtkTreeGetLastVisibleNode(ih, model, iterRoot);
  else if(iupStrEqualNoCase(value, "PGUP"))
  {
    GtkTreeIter iterPrev;

    GtkTreePath* pathFocus;
    gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &pathFocus, NULL);
    gtk_tree_model_get_iter(model, &iterPrev, pathFocus);
    gtk_tree_path_free(pathFocus);

    ih->data->id_control = -1;
    gtkTreeFindVisibleNodeId(ih, model, iterRoot, iterPrev);
    ih->data->id_control -= 10;  /* less 10 visible nodes */

    if(ih->data->id_control < 0)
      ih->data->id_control = 0;  /* Begin of tree = Root id */

    iterItem = gtkTreeFindVisibleNodeFromId(ih, model, iterRoot);
  }
  else if(iupStrEqualNoCase(value, "PGDN"))
  {
    GtkTreeIter iterNext;

    GtkTreePath* pathFocus;
    gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &pathFocus, NULL);
    gtk_tree_model_get_iter(model, &iterNext, pathFocus);
    gtk_tree_path_free(pathFocus);

    ih->data->id_control = -1;
    gtkTreeFindVisibleNodeId(ih, model, iterRoot, iterNext);
    ih->data->id_control += 10;  /* more 10 visible nodes */

    iterNext = gtkTreeFindVisibleNodeFromId(ih, model, iterRoot);

    if (ih->data->id_control >= 0)
      iterNext = gtkTreeGetLastVisibleNode(ih, model, iterRoot);

    iterItem = iterNext;
  }
  else if(iupStrEqualNoCase(value, "NEXT"))
  {
    GtkTreeIter iterNext;

    GtkTreePath* pathFocus;
    gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &pathFocus, NULL);
    gtk_tree_model_get_iter(model, &iterNext, pathFocus);
    gtk_tree_path_free(pathFocus);

    ih->data->id_control = -1;
    gtkTreeFindVisibleNodeId(ih, model, iterRoot, iterNext);
    ih->data->id_control++;  /* more 1 visible node */

    iterNext = gtkTreeFindVisibleNodeFromId(ih, model, iterRoot);

    if (ih->data->id_control >= 0)
      iterNext = gtkTreeGetLastVisibleNode(ih, model, iterRoot);

    iterItem = iterNext;
  }
  else if(iupStrEqualNoCase(value, "PREVIOUS"))
  {
    GtkTreeIter iterPrev;

    GtkTreePath* pathFocus;
    gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &pathFocus, NULL);
    gtk_tree_model_get_iter(model, &iterPrev, pathFocus);
    gtk_tree_path_free(pathFocus);

    ih->data->id_control = -1;
    gtkTreeFindVisibleNodeId(ih, model, iterRoot, iterPrev);
    ih->data->id_control--;   /* less 1 visible node */

    if (ih->data->id_control < 0)
      ih->data->id_control = 0;

    iterItem = gtkTreeFindVisibleNodeFromId(ih, model, iterRoot);
    if (!iterItem.user_data)
      return 0;
  }
  else
  {
    if (!gtkTreeFindNodeFromString(ih, model, value, &iterItem))
      return 0;
  }

  /* select */
  if (ih->data->mark_mode==ITREE_MARK_SINGLE)
  {
    iupAttribSetStr(ih, "_IUP_IGNORE_SELECTION", "1");
    gtk_tree_selection_select_iter(selection, &iterItem);
  }

  path = gtk_tree_model_get_path(model, &iterItem);
  gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(ih->handle), path, NULL, FALSE, 0, 0); /* scroll to visible */
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(ih->handle), path, NULL, FALSE);  /* set focus */
  gtk_tree_path_free(path);

  iupAttribSetInt(ih, "_IUPTREE_OLDVALUE", gtkTreeGetNodeId(ih, iterItem));

  return 0;
} 

static int gtkTreeSetMarkStartAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreePath *pathMarkStart, *pathMarkStartPrev;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return 0;

  pathMarkStart = gtk_tree_model_get_path(model, &iterItem);

  pathMarkStartPrev = (GtkTreePath*)iupAttribGet(ih, "_IUPTREE_MARKSTART_NODE");
  if (pathMarkStartPrev)
    gtk_tree_path_free(pathMarkStartPrev);

  iupAttribSetStr(ih, "_IUPTREE_MARKSTART_NODE", (char*)pathMarkStart);

  return 1;
}

static char* gtkTreeGetMarkedAttrib(Ihandle* ih, const char* name_id)
{
  GtkTreeSelection* selection;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return 0;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));
  if (gtk_tree_selection_iter_is_selected(selection, &iterItem))
    return "YES";
  else
    return "NO";
}

static int gtkTreeSetMarkedAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  GtkTreeSelection* selection;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
    return 0;

  iupAttribSetStr(ih, "_IUP_IGNORE_SELECTION", "1");

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));
  if (iupStrBoolean(value))
    gtk_tree_selection_select_iter(selection, &iterItem);
  else
    gtk_tree_selection_unselect_iter(selection, &iterItem);

  return 0;
}

static int gtkTreeSetDelNodeAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  if (!ih->handle)  /* do not store the action before map */
    return 0;
  if (iupStrEqualNoCase(value, "SELECTED"))  /* selected here means the specified one */
  {
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
    GtkTreeIter iterItem;
    GtkTreeIter iterParent;

    if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
      return 0;

    if (!gtk_tree_model_iter_parent(model, &iterParent, &iterItem)) /* the root node can't be deleted */
      return 0;

    gtkTreeCallNodeRemoved(ih, model, &iterItem);

    /* deleting the specified node (and it's children) */
    gtk_tree_store_remove(GTK_TREE_STORE(model), &iterItem);
  }
  else if(iupStrEqualNoCase(value, "CHILDREN"))  /* children of the specified one */
  {
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
    GtkTreeIter iterItem, iterChild;
    int hasChildren;

    if (!gtkTreeFindNodeFromString(ih, model, name_id, &iterItem))
      return 0;

    hasChildren = gtk_tree_model_iter_children(model, &iterChild, &iterItem);

    /* deleting the selected node's children */
    while(hasChildren)
    {
      gtkTreeCallNodeRemoved(ih, model, &iterChild);
      hasChildren = gtk_tree_store_remove(GTK_TREE_STORE(model), &iterChild);
    }
  }
  else if(iupStrEqualNoCase(value, "MARKED"))  /* Delete the array of marked nodes */
  {
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));
    GList *rr_list = NULL;
    GList *node;

    gtk_tree_selection_selected_foreach(selection, (GtkTreeSelectionForeachFunc)gtkTreeSelected_Iter_Func, &rr_list);

    for(node = rr_list; node != NULL; node = node->next)
    {
      GtkTreePath* path = gtk_tree_row_reference_get_path(node->data);
      if (path)
      {
        GtkTreeIter iterItem;
        if (gtk_tree_model_get_iter(model, &iterItem, path))
        {
          gtkTreeCallNodeRemoved(ih, model, &iterItem);
          gtk_tree_store_remove(GTK_TREE_STORE(model), &iterItem);
        }
        gtk_tree_path_free(path);
      }
      gtk_tree_row_reference_free(node->data);
    }
    g_list_free(rr_list);
  }

  return 0;
}

static int gtkTreeSetRenameAttrib(Ihandle* ih, const char* value)
{  
  if (ih->data->show_rename)
  {
    GtkTreePath* path;
    IFni cbShowRename = (IFni)IupGetCallback(ih, "SHOWRENAME_CB");
    GtkTreeViewColumn *focus_column;

    gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &path, &focus_column);

    if (cbShowRename)
    {
      GtkTreeIter iterItem;
      GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
      gtk_tree_model_get_iter(model, &iterItem, path);
      cbShowRename(ih, gtkTreeGetNodeId(ih, iterItem));
    }

    gtk_tree_view_set_cursor(GTK_TREE_VIEW(ih->handle), path, focus_column, TRUE);
    gtk_tree_path_free(path);
  }
  else
  {
    IFnis cbRenameNode = (IFnis)IupGetCallback(ih, "RENAMENODE_CB");
    if (cbRenameNode)
    {
      GtkTreePath* path;
      GtkTreeIter iterItem;
      GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
      gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &path, NULL);
      gtk_tree_model_get_iter(model, &iterItem, path);
      gtk_tree_path_free(path);
      cbRenameNode(ih, gtkTreeGetNodeId(ih, iterItem), gtkTreeGetTitle(model, iterItem));  
    }
  }

  (void)value;
  return 0;
}

static int gtkTreeSetImageExpandedAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  int kind;
  GtkTreeStore*  store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GdkPixbuf* pixExpand = iupImageGetImage(value, ih, 0);
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), name_id, &iterItem))
    return 0;

  gtk_tree_model_get(GTK_TREE_MODEL(store), &iterItem, IUPGTK_TREE_KIND, &kind, -1);

  if (kind == ITREE_BRANCH)
  {
    if (pixExpand)
      gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_IMAGE_EXPANDED, pixExpand, 
                                           IUPGTK_TREE_HAS_IMAGE_EXPANDED, TRUE, -1);
    else
      gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_IMAGE_EXPANDED, ih->data->def_image_expanded, 
                                           IUPGTK_TREE_HAS_IMAGE_EXPANDED, FALSE, -1);
  }

  return 1;
}

static int gtkTreeSetImageAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  GtkTreeStore* store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle)));
  GdkPixbuf* pixImage = iupImageGetImage(value, ih, 0);
  GtkTreeIter iterItem;
  if (!gtkTreeFindNodeFromString(ih, GTK_TREE_MODEL(store), name_id, &iterItem))
    return 0;

  if (pixImage)
  {
    gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_IMAGE, pixImage, 
                                         IUPGTK_TREE_HAS_IMAGE, TRUE, -1);
  }
  else
  {
    int kind;
    gtk_tree_model_get(GTK_TREE_MODEL(store), &iterItem, IUPGTK_TREE_KIND, &kind, -1);
    if (kind == ITREE_BRANCH)
      gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_IMAGE, ih->data->def_image_collapsed, 
                                           IUPGTK_TREE_HAS_IMAGE, FALSE, -1);
    else
      gtk_tree_store_set(store, &iterItem, IUPGTK_TREE_IMAGE, ih->data->def_image_leaf, 
                                           IUPGTK_TREE_HAS_IMAGE, FALSE, -1);
  }

  return 1;
}

static int gtkTreeSetImageBranchExpandedAttrib(Ihandle* ih, const char* value)
{
  GtkTreeIter iterRoot;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  ih->data->def_image_expanded = iupImageGetImage(value, ih, 0);

  gtk_tree_model_get_iter_first(model, &iterRoot);

  /* Update all images, starting at root node */
  gtkTreeUpdateImages(ih, model, iterRoot, ITREE_UPDATEIMAGE_EXPANDED);

  return 1;
}

static int gtkTreeSetImageBranchCollapsedAttrib(Ihandle* ih, const char* value)
{
  GtkTreeIter iterRoot;
  GtkTreeModel*  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  ih->data->def_image_collapsed = iupImageGetImage(value, ih, 0);

  gtk_tree_model_get_iter_first(model, &iterRoot);

  /* Update all images, starting at root node */
  gtkTreeUpdateImages(ih, model, iterRoot, ITREE_UPDATEIMAGE_COLLAPSED);

  return 1;
}

static int gtkTreeSetImageLeafAttrib(Ihandle* ih, const char* value)
{
  GtkTreeIter iterRoot;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  ih->data->def_image_leaf = iupImageGetImage(value, ih, 0);

  gtk_tree_model_get_iter_first(model, &iterRoot);

  /* Update all images, starting at root node */
  gtkTreeUpdateImages(ih, model, iterRoot, ITREE_UPDATEIMAGE_LEAF);

  return 1;
}

static int gtkTreeSetBgColorAttrib(Ihandle* ih, const char* value)
{
  unsigned char r, g, b;

  GtkScrolledWindow* scrolled_window = (GtkScrolledWindow*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (scrolled_window)
  {
    /* ignore given value, must use only from parent for the scrollbars */
    char* parent_value = iupBaseNativeParentGetBgColor(ih);

    if (iupStrToRGB(parent_value, &r, &g, &b))
    {
      GtkWidget* sb;

      if (!GTK_IS_SCROLLED_WINDOW(scrolled_window))
        scrolled_window = (GtkScrolledWindow*)iupAttribGet(ih, "_IUPGTK_SCROLLED_WINDOW");

      iupgtkBaseSetBgColor((GtkWidget*)scrolled_window, r, g, b);

#if GTK_CHECK_VERSION(2, 8, 0)
      sb = gtk_scrolled_window_get_hscrollbar(scrolled_window);
      if (sb) iupgtkBaseSetBgColor(sb, r, g, b);

      sb = gtk_scrolled_window_get_vscrollbar(scrolled_window);
      if (sb) iupgtkBaseSetBgColor(sb, r, g, b);
#endif
    }
  }

  if (!iupStrToRGB(value, &r, &g, &b))
    return 0;

  {
    GtkCellRenderer* renderer_txt = (GtkCellRenderer*)iupAttribGet(ih, "_IUPGTK_RENDERER_TEXT");
    GtkCellRenderer* renderer_img = (GtkCellRenderer*)iupAttribGet(ih, "_IUPGTK_RENDERER_IMG");
    GdkColor color;
    iupgdkColorSet(&color, r, g, b);
    g_object_set(G_OBJECT(renderer_txt), "cell-background-gdk", &color, NULL);
    g_object_set(G_OBJECT(renderer_img), "cell-background-gdk", &color, NULL);
  }

  iupdrvBaseSetBgColorAttrib(ih, value);   /* use given value for contents */

  /* no need to update internal image cache in GTK */

  return 1;
}

static int gtkTreeSetFgColorAttrib(Ihandle* ih, const char* value)
{
  unsigned char r, g, b;
  if (!iupStrToRGB(value, &r, &g, &b))
    return 0;

  iupgtkBaseSetFgColor(ih->handle, r, g, b);

  {
    GtkCellRenderer* renderer_txt = (GtkCellRenderer*)iupAttribGet(ih, "_IUPGTK_RENDERER_TEXT");
    GdkColor color;
    iupgdkColorSet(&color, r, g, b);
    g_object_set(G_OBJECT(renderer_txt), "foreground-gdk", &color, NULL);
    g_object_get(G_OBJECT(renderer_txt), "foreground-gdk", &color, NULL);
    color.blue = 0;
  }

  return 1;
}

void iupdrvTreeUpdateMarkMode(Ihandle *ih)
{
  GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));
  gtk_tree_selection_set_mode(selection, (ih->data->mark_mode==ITREE_MARK_SINGLE)? GTK_SELECTION_SINGLE: GTK_SELECTION_MULTIPLE);

  if (ih->data->mark_mode==ITREE_MARK_MULTIPLE && !ih->data->show_dragdrop)
  {
#if GTK_CHECK_VERSION(2, 10, 0)
      gtk_tree_view_set_rubber_banding(GTK_TREE_VIEW(ih->handle), TRUE);
#endif
  }
}


/***********************************************************************************************/


static void gtkTreeSetRenameCaretPos(GtkCellEditable *editable, const char* value)
{
  int pos = 1;

  if (iupStrToInt(value, &pos))
  {
    if (pos < 1) pos = 1;
    pos--; /* IUP starts at 1 */

    gtk_editable_set_position(GTK_EDITABLE(editable), pos);
  }
}

static void gtkTreeSetRenameSelectionPos(GtkCellEditable *editable, const char* value)
{
  int start = 1, end = 1;

  if (iupStrToIntInt(value, &start, &end, ':') != 2) 
    return;

  if(start < 1 || end < 1) 
    return;

  start--; /* IUP starts at 1 */
  end--;

  gtk_editable_select_region(GTK_EDITABLE(editable), start, end);
}

/*****************************************************************************/
/* SIGNALS                                                                   */
/*****************************************************************************/

static void gtkTreeCellTextEditingStarted(GtkCellRenderer *cell, GtkCellEditable *editable, const gchar *path_string, Ihandle *ih)
{
  char* value;
  GtkTreeIter iterItem;
  PangoFontDescription* fontdesc = NULL;
  GdkColor *color = NULL;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));

  value = iupAttribGetStr(ih, "RENAMECARET");
  if (value)
    gtkTreeSetRenameCaretPos(editable, value);

  value = iupAttribGetStr(ih, "RENAMESELECTION");
  if (value)
    gtkTreeSetRenameSelectionPos(editable, value);

  gtk_tree_model_get_iter_from_string(model, &iterItem, path_string);
  gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_FONT, &fontdesc, -1);
  if (fontdesc)
    gtk_widget_modify_font(GTK_WIDGET(editable), fontdesc);

  gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_COLOR, &color, -1);
  if (color)
    iupgtkBaseSetFgGdkColor(GTK_WIDGET(editable), color);

  (void)cell;
}

static void gtkTreeCellTextEdited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, Ihandle* ih)
{
  GtkTreeModel* model;
  GtkTreeIter iterItem;
  IFnis cbRename;

  if (!new_text)
    return;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  if (!gtk_tree_model_get_iter_from_string(model, &iterItem, path_string))
    return;

  cbRename = (IFnis)IupGetCallback(ih, "RENAME_CB");
  if (cbRename)
  {
    if (cbRename(ih, gtkTreeGetNodeId(ih, iterItem), iupgtkStrConvertFromUTF8(new_text)) == IUP_IGNORE)
      return;
  }

  /* It is the responsibility of the application to update the model and store new_text at the position indicated by path. */
  gtk_tree_store_set(GTK_TREE_STORE(model), &iterItem, IUPGTK_TREE_TITLE, new_text, -1);

  (void)cell;
}

static int gtkTreeCallDragDropCb(Ihandle* ih, GtkTreeIter *iterDrag, GtkTreeIter *iterDrop, int *is_ctrl)
{
  IFniiii cbDragDrop = (IFniiii)IupGetCallback(ih, "DRAGDROP_CB");
  int is_shift = 0;
  char key[5];
  iupdrvGetKeyState(key);
  if (key[0] == 'S')
    is_shift = 1;
  if (key[1] == 'C')
    *is_ctrl = 1;
  else
    *is_ctrl = 0;

  if (cbDragDrop)
  {
    int drag_id = gtkTreeGetNodeId(ih, *iterDrag);
    int drop_id = gtkTreeGetNodeId(ih, *iterDrop);
    return cbDragDrop(ih, drag_id, drop_id, is_shift, *is_ctrl);
  }

  return IUP_CONTINUE; /* allow to move by default if callback not defined */
}

static void gtkTreeDragDataReceived(GtkWidget *widget, GdkDragContext *context, gint x, gint y, 
                                    GtkSelectionData *selection_data, guint info, guint time, Ihandle* ih)
{
  GtkTreePath* pathDrag = (GtkTreePath*)iupAttribGet(ih, "_IUPTREE_DRAGITEM");
  GtkTreePath* pathDrop = (GtkTreePath*)iupAttribGet(ih, "_IUPTREE_DROPITEM");
  int accepted = FALSE;
  int is_ctrl;

  if (pathDrag && pathDrop)
  {
    GtkTreeIter iterDrag, iterDrop, iterParent, iterNextParent;
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));

    gtk_tree_model_get_iter(model, &iterDrag, pathDrag);
    gtk_tree_model_get_iter(model, &iterDrop, pathDrop);

    iterParent = iterDrop;
    while(gtk_tree_model_iter_parent(model, &iterNextParent, &iterParent))
    {
      if (iterNextParent.user_data == iterDrag.user_data)
        goto gtkTreeDragDataReceived_FINISH;
      iterParent = iterNextParent;
    }

    accepted = TRUE;

    if (gtkTreeCallDragDropCb(ih, &iterDrag, &iterDrop, &is_ctrl) == IUP_CONTINUE)
    {
      GtkTreeIter iterNewItem;

      /* Copy the dragged item to the new position. */
      gtkTreeCopyNode(ih, model, &iterDrag, &iterDrop, &iterNewItem, is_ctrl);

      if (!is_ctrl)
      {
        /* do not delete the user data, we copy the references in CopyNode */
        gtk_tree_store_remove(GTK_TREE_STORE(model), &iterDrag);
      }

      /* set focus and selection */
      {
        GtkTreePath *pathNew;
        GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));

        pathNew = gtk_tree_model_get_path(model, &iterNewItem);
        gtk_tree_selection_select_path(selection, pathNew);

        gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(ih->handle), pathNew, NULL, FALSE, 0, 0);
        gtk_tree_view_set_cursor(GTK_TREE_VIEW(ih->handle), pathNew, NULL, FALSE);

        gtk_tree_path_free(pathNew);
      }
    }
  }

gtkTreeDragDataReceived_FINISH:
  if (pathDrag) gtk_tree_path_free(pathDrag);
  if (pathDrop) gtk_tree_path_free(pathDrop);

  iupAttribSetStr(ih, "_IUPTREE_DRAGITEM", NULL);
  iupAttribSetStr(ih, "_IUPTREE_DROPITEM", NULL);

  gtk_drag_finish(context, accepted, (context->action == GDK_ACTION_MOVE), time);

  (void)widget;
  (void)info;
  (void)x;
  (void)y;
  (void)selection_data;
}

static gboolean gtkTreeDragDrop(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, Ihandle* ih)
{
  GtkTreePath* path;
  GtkTreeViewDropPosition pos;
  GdkAtom target = GDK_NONE;

  /* unset any highlight row */
  gtk_tree_view_set_drag_dest_row (GTK_TREE_VIEW(widget), NULL, GTK_TREE_VIEW_DROP_BEFORE);

  if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(ih->handle), x, y, &path, &pos))
  {
    if ((pos == GTK_TREE_VIEW_DROP_INTO_OR_BEFORE || pos == GTK_TREE_VIEW_DROP_INTO_OR_AFTER) && 
        (gtk_tree_path_compare(path, (GtkTreePath*)iupAttribGet(ih, "_IUPTREE_DRAGITEM")) != 0))
    {
      target = gtk_drag_dest_find_target(widget, context, gtk_drag_dest_get_target_list(widget));
      if (target != GDK_NONE)
      {
        iupAttribSetStr(ih, "_IUPTREE_DROPITEM", (char*)path);
        gtk_drag_get_data(widget, context, target, time);
        return TRUE;
      }
    }
  }

  (void)widget;
  return FALSE;
}

static void gtkTreeDragLeave(GtkWidget *widget, GdkDragContext *context, guint time)
{
  /* unset any highlight row */
  gtk_tree_view_set_drag_dest_row(GTK_TREE_VIEW(widget), NULL, GTK_TREE_VIEW_DROP_BEFORE);
  (void)context;
  (void)time;
}

static gboolean gtkTreeDragMotion(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, Ihandle* ih)
{
  GtkTreePath* path;
  GtkTreeViewDropPosition pos;
  GtkTreePath* pathDrag = (GtkTreePath*)iupAttribGet(ih, "_IUPTREE_DRAGITEM");
  if (pathDrag && gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(ih->handle), x, y, &path, &pos))
  {
    if ((pos == GTK_TREE_VIEW_DROP_INTO_OR_BEFORE || pos == GTK_TREE_VIEW_DROP_INTO_OR_AFTER) && 
        (gtk_tree_path_compare(path, pathDrag) != 0))
    {
      gtk_tree_view_set_drag_dest_row(GTK_TREE_VIEW(widget), path, pos);
      gdk_drag_status(context, context->actions, time);
      return TRUE;
    }

    gtk_tree_path_free(path);
  }

  (void)widget;
  return FALSE;
}

static void gtkTreeDragBegin(GtkWidget *widget, GdkDragContext *context, Ihandle* ih)
{
  int x = iupAttribGetInt(ih, "_IUPTREE_DRAG_X");
  int y = iupAttribGetInt(ih, "_IUPTREE_DRAG_Y");
  GtkTreePath* path;
  GtkTreeViewDropPosition pos;
  if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(ih->handle), x, y, &path, &pos))
  {
    if ((pos == GTK_TREE_VIEW_DROP_INTO_OR_BEFORE || pos == GTK_TREE_VIEW_DROP_INTO_OR_AFTER))
    {
      GdkPixmap* pixmap;
      iupAttribSetStr(ih, "_IUPTREE_DRAGITEM", (char*)path);

      pixmap = gtk_tree_view_create_row_drag_icon(GTK_TREE_VIEW(ih->handle), path);
      gtk_drag_source_set_icon(widget, gtk_widget_get_colormap(widget), pixmap, NULL);
      g_object_unref(pixmap);
      return;
    }
  }

  (void)context;
  (void)widget;
}

static void gtkTreeSelectionChanged(GtkTreeSelection* selection, Ihandle* ih)
{
  IFnii cbSelec;
  int is_ctrl = 0;

  if (ih->data->mark_mode == ITREE_MARK_MULTIPLE)
  {
    char key[5];
    iupdrvGetKeyState(key);
    if (key[0] == 'S')
      return;
    else if (key[1] == 'C')
      is_ctrl = 1;

    if (iupAttribGetInt(ih, "_IUPTREE_EXTENDSELECT")==2 && !is_ctrl)
    {
      iupAttribSetStr(ih, "_IUPTREE_EXTENDSELECT", NULL);
      gtkTreeCallMultiSelectionCb(ih);
      return;
    }
  }

  cbSelec = (IFnii)IupGetCallback(ih, "SELECTION_CB");
  if (cbSelec)
  {
    int curpos = -1, is_selected = 0;

    if (iupAttribGet(ih, "_IUP_IGNORE_SELECTION"))
    {
      iupAttribSetStr(ih, "_IUP_IGNORE_SELECTION", NULL);
      return;
    }

    {
      GtkTreeIter iterFocus;
      GtkTreePath* pathFocus;
      GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
      gtk_tree_view_get_cursor(GTK_TREE_VIEW(ih->handle), &pathFocus, NULL);
      if (pathFocus)
      {
        gtk_tree_model_get_iter(model, &iterFocus, pathFocus);
        gtk_tree_path_free(pathFocus);
        curpos = gtkTreeGetNodeId(ih, iterFocus);
        is_selected = gtk_tree_selection_iter_is_selected(selection, &iterFocus);
      }
    }

    if (curpos == -1)
      return;

    if (is_ctrl) 
      cbSelec(ih, curpos, is_selected);
    else
    {
      int oldpos = iupAttribGetInt(ih, "_IUPTREE_OLDVALUE");
      if(oldpos != curpos)
      {
        cbSelec(ih, oldpos, 0);  /* unselected */
        cbSelec(ih, curpos, 1);  /*   selected */

        iupAttribSetInt(ih, "_IUPTREE_OLDVALUE", curpos);
      }
    }
  }
}

static gboolean gtkTreeTestExpandRow(GtkTreeView* tree_view, GtkTreeIter *iterItem, GtkTreePath *path, Ihandle* ih)
{
  IFni cbBranchOpen = (IFni)IupGetCallback(ih, "BRANCHOPEN_CB");
  if (cbBranchOpen)
  {
    if (iupAttribGet(ih, "_IUPTREE_IGNORE_BRANCHOPEN_CB"))
    {
      iupAttribSetStr(ih, "_IUPTREE_IGNORE_BRANCHOPEN_CB", NULL);
      return FALSE;
    }

    if (cbBranchOpen(ih, gtkTreeGetNodeId(ih, *iterItem)) == IUP_IGNORE)
      return TRUE;  /* prevent the change */
  }

  (void)path;
  (void)tree_view;
  return FALSE;
}

static gboolean gtkTreeTestCollapseRow(GtkTreeView* tree_view, GtkTreeIter *iterItem, GtkTreePath *path, Ihandle* ih)
{
  IFni cbBranchClose = (IFni)IupGetCallback(ih, "BRANCHCLOSE_CB");
  if (cbBranchClose)
  {
    if (cbBranchClose(ih, gtkTreeGetNodeId(ih, *iterItem)) == IUP_IGNORE)
      return TRUE;
  }

  (void)path;
  (void)tree_view;
  return FALSE;
}

static void gtkTreeRowActived(GtkTreeView* tree_view, GtkTreePath *path, GtkTreeViewColumn *column, Ihandle* ih)
{
  GtkTreeIter iterItem;
  GtkTreeModel* model;
  int kind;  /* used for nodes defined as branches, but do not have children */
  IFni cbExecuteLeaf  = (IFni)IupGetCallback(ih, "EXECUTELEAF_CB");
  if (!cbExecuteLeaf)
    return;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
  gtk_tree_model_get_iter(model, &iterItem, path);
  gtk_tree_model_get(model, &iterItem, IUPGTK_TREE_KIND, &kind, -1);

  /* just to leaf nodes */
  if(gtk_tree_model_iter_has_child(model, &iterItem) == 0 && kind == ITREE_LEAF)
    cbExecuteLeaf(ih, gtkTreeGetNodeId(ih, iterItem));

  (void)column;
  (void)tree_view;
}

static int gtkTreeConvertXYToPos(Ihandle* ih, int x, int y)
{
  GtkTreePath* path;
  if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(ih->handle), x, y, &path, NULL))
  {
    GtkTreeIter iterItem;
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
    gtk_tree_model_get_iter(model, &iterItem, path);
    gtk_tree_path_free (path);
    return gtkTreeGetNodeId(ih, iterItem);
  }
  return -1;
}

static gboolean gtkTreeButtonEvent(GtkWidget *treeview, GdkEventButton *evt, Ihandle* ih)
{
  if (iupgtkButtonEvent(treeview, evt, ih) == TRUE)
    return TRUE;

  if (evt->type == GDK_BUTTON_PRESS && evt->button == 3)  /* right single click */
  {
    IFni cbRightClick  = (IFni)IupGetCallback(ih, "RIGHTCLICK_CB");
    if (cbRightClick)
    {
      int id = gtkTreeConvertXYToPos(ih, (int)evt->x, (int)evt->y);
      if (id != -1)
        cbRightClick(ih, id);
      return TRUE;
    }
  }
  else if (evt->type == GDK_2BUTTON_PRESS && evt->button == 1)  /* left double click */
  {
    GtkTreePath *path;

    if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview), (gint) evt->x, (gint) evt->y, &path, NULL, NULL, NULL))
    {
      GtkTreeIter iter;
      GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(ih->handle));
      int kind;  /* used for nodes defined as branches, but do not have children */

      gtk_tree_model_get_iter(model, &iter, path);
      gtk_tree_model_get(model, &iter, IUPGTK_TREE_KIND, &kind, -1);

      if (kind == ITREE_BRANCH)
        gtkTreeExpandItem(ih, path, -1); /* toggle */

      gtk_tree_path_free(path);
    }
  }
  else if (evt->type == GDK_BUTTON_RELEASE && evt->button == 1)  /* left single release */
  {
    if (ih->data->mark_mode==ITREE_MARK_MULTIPLE && (evt->state & GDK_SHIFT_MASK))
      gtkTreeCallMultiSelectionCb(ih); /* Multi Selection Callback */

    if (ih->data->mark_mode==ITREE_MARK_MULTIPLE && 
        !(evt->state & GDK_SHIFT_MASK) && !(evt->state & GDK_CONTROL_MASK))
    {
      if (iupAttribGet(ih, "_IUPTREE_EXTENDSELECT"))
        iupAttribSetStr(ih, "_IUPTREE_EXTENDSELECT", "2");
    }
  }
  else if (evt->type == GDK_BUTTON_PRESS && evt->button == 1)  /* left single press */
  {
    iupAttribSetInt(ih, "_IUPTREE_DRAG_X", (int)evt->x);
    iupAttribSetInt(ih, "_IUPTREE_DRAG_Y", (int)evt->y);

    if (ih->data->mark_mode==ITREE_MARK_MULTIPLE && 
        !(evt->state & GDK_SHIFT_MASK) && !(evt->state & GDK_CONTROL_MASK))
      iupAttribSetStr(ih, "_IUPTREE_EXTENDSELECT", "1");
  }
  
  return FALSE;
}

static gboolean gtkTreeKeyReleaseEvent(GtkWidget *widget, GdkEventKey *evt, Ihandle *ih)
{
  if (ih->data->mark_mode==ITREE_MARK_MULTIPLE && (evt->state & GDK_SHIFT_MASK))
  {
    if (evt->keyval == GDK_Up || evt->keyval == GDK_Down || evt->keyval == GDK_Home || evt->keyval == GDK_End)
      gtkTreeCallMultiSelectionCb(ih); /* Multi Selection Callback */
  }

  (void)widget;
  return TRUE;
}

static gboolean gtkTreeKeyPressEvent(GtkWidget *widget, GdkEventKey *evt, Ihandle *ih)
{
  if (iupgtkKeyPressEvent(widget, evt, ih) == TRUE)
    return TRUE;

  if (evt->keyval == GDK_F2)
  {
    gtkTreeSetRenameAttrib(ih, NULL);
    return TRUE;
  }
  else if (evt->keyval == GDK_Return || evt->keyval == GDK_KP_Enter)
  {
    gtkTreeOpenCloseEvent(ih);
    return TRUE;
  }

  return FALSE;
}

static void gtkTreeEnableDragDrop(Ihandle* ih)
{
  const GtkTargetEntry row_targets[] = {
    { "GTK_TREE_MODEL_ROW", GTK_TARGET_SAME_WIDGET, 0 }
  };

  if (iupAttribGetBoolean(ih, "AUTODRAGDROP"))
  {
    gtk_tree_view_enable_model_drag_source (GTK_TREE_VIEW(ih->handle),
			        GDK_BUTTON1_MASK,
			        row_targets,
			        G_N_ELEMENTS(row_targets),
			        GDK_ACTION_MOVE|GDK_ACTION_COPY);
    gtk_tree_view_enable_model_drag_dest (GTK_TREE_VIEW(ih->handle),
			      row_targets,
			      G_N_ELEMENTS(row_targets),
			      GDK_ACTION_MOVE|GDK_ACTION_COPY);
  }
  else
  {
    gtk_drag_source_set(ih->handle, GDK_BUTTON1_MASK, row_targets, G_N_ELEMENTS(row_targets), GDK_ACTION_MOVE|GDK_ACTION_COPY);
    gtk_drag_dest_set(ih->handle, GDK_BUTTON1_MASK, row_targets, G_N_ELEMENTS(row_targets), GDK_ACTION_MOVE|GDK_ACTION_COPY);

    g_signal_connect(G_OBJECT(ih->handle),  "drag-begin", G_CALLBACK(gtkTreeDragBegin), ih);
    g_signal_connect(G_OBJECT(ih->handle), "drag-motion", G_CALLBACK(gtkTreeDragMotion), ih);
    g_signal_connect(G_OBJECT(ih->handle), "drag-leave", G_CALLBACK(gtkTreeDragLeave), NULL);
    g_signal_connect(G_OBJECT(ih->handle),   "drag-drop", G_CALLBACK(gtkTreeDragDrop), ih);
    g_signal_connect(G_OBJECT(ih->handle), "drag-data-received", G_CALLBACK(gtkTreeDragDataReceived), ih);
  }
}

/*****************************************************************************/

static int gtkTreeMapMethod(Ihandle* ih)
{
  GtkScrolledWindow* scrolled_window = NULL;
  GtkTreeStore *store;
  GtkCellRenderer *renderer_img, *renderer_txt;
  GtkTreeSelection* selection;
  GtkTreeViewColumn *column;

  store = gtk_tree_store_new(9, GDK_TYPE_PIXBUF, G_TYPE_BOOLEAN, GDK_TYPE_PIXBUF, G_TYPE_BOOLEAN,
                                G_TYPE_STRING, G_TYPE_INT, GDK_TYPE_COLOR, PANGO_TYPE_FONT_DESCRIPTION, G_TYPE_POINTER);

  ih->handle = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

  g_object_unref(store);

  if (!ih->handle)
    return IUP_ERROR;

  scrolled_window = (GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL);
  iupAttribSetStr(ih, "_IUP_EXTRAPARENT", (char*)scrolled_window);

  /* Column and renderers */
  column = gtk_tree_view_column_new();
  iupAttribSetStr(ih, "_IUPGTK_COLUMN",   (char*)column);

  renderer_img = gtk_cell_renderer_pixbuf_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(column), renderer_img, FALSE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(column), renderer_img, "pixbuf", IUPGTK_TREE_IMAGE,
                                                              "pixbuf-expander-open", IUPGTK_TREE_IMAGE_EXPANDED,
                                                            "pixbuf-expander-closed", IUPGTK_TREE_IMAGE, 
                                                            NULL);
  iupAttribSetStr(ih, "_IUPGTK_RENDERER_IMG", (char*)renderer_img);

  renderer_txt = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(column), renderer_txt, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(column), renderer_txt, "text", IUPGTK_TREE_TITLE,
                                                                     "is-expander", IUPGTK_TREE_KIND,
                                                                     "font-desc", IUPGTK_TREE_FONT,
                                                                  "foreground-gdk", IUPGTK_TREE_COLOR, 
                                                                  NULL);
  iupAttribSetStr(ih, "_IUPGTK_RENDERER_TEXT", (char*)renderer_txt);

  if (ih->data->show_rename)
    g_object_set(G_OBJECT(renderer_txt), "editable", TRUE, NULL);

  g_object_set(G_OBJECT(renderer_txt), "xpad", 0, NULL);
  g_object_set(G_OBJECT(renderer_txt), "ypad", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ih->handle), column);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(ih->handle), FALSE);
  gtk_tree_view_set_enable_search(GTK_TREE_VIEW(ih->handle), FALSE);

#if GTK_CHECK_VERSION(2, 10, 0)
  if (iupAttribGetBoolean(ih, "HIDELINES"))
    gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(ih->handle), FALSE);
  else
    gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(ih->handle), TRUE);
#endif

#if GTK_CHECK_VERSION(2, 12, 0)
  if (iupAttribGetBoolean(ih, "HIDEBUTTONS"))
    gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(ih->handle), FALSE);
  else
    gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(ih->handle), TRUE);
#endif

  if (ih->data->show_dragdrop)
    gtkTreeEnableDragDrop(ih);

  gtk_container_add((GtkContainer*)scrolled_window, ih->handle);
  gtk_widget_show((GtkWidget*)scrolled_window);
  gtk_scrolled_window_set_shadow_type(scrolled_window, GTK_SHADOW_IN); 

  gtk_scrolled_window_set_policy(scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ih->handle));
 
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
  gtk_tree_view_set_reorderable(GTK_TREE_VIEW(ih->handle), FALSE);

  /* callbacks */
  g_signal_connect(selection,            "changed", G_CALLBACK(gtkTreeSelectionChanged), ih);
  
  g_signal_connect(renderer_txt, "editing-started", G_CALLBACK(gtkTreeCellTextEditingStarted), ih);
  g_signal_connect(renderer_txt,          "edited", G_CALLBACK(gtkTreeCellTextEdited), ih);

  g_signal_connect(G_OBJECT(ih->handle), "enter-notify-event", G_CALLBACK(iupgtkEnterLeaveEvent), ih);
  g_signal_connect(G_OBJECT(ih->handle), "leave-notify-event", G_CALLBACK(iupgtkEnterLeaveEvent), ih);
  g_signal_connect(G_OBJECT(ih->handle), "focus-in-event",     G_CALLBACK(iupgtkFocusInOutEvent), ih);
  g_signal_connect(G_OBJECT(ih->handle), "focus-out-event",    G_CALLBACK(iupgtkFocusInOutEvent), ih);
  g_signal_connect(G_OBJECT(ih->handle), "show-help",          G_CALLBACK(iupgtkShowHelp), ih);
  g_signal_connect(G_OBJECT(ih->handle), "motion-notify-event",G_CALLBACK(iupgtkMotionNotifyEvent), ih);

  g_signal_connect(G_OBJECT(ih->handle),    "test-expand-row", G_CALLBACK(gtkTreeTestExpandRow), ih);
  g_signal_connect(G_OBJECT(ih->handle),  "test-collapse-row", G_CALLBACK(gtkTreeTestCollapseRow), ih);
  g_signal_connect(G_OBJECT(ih->handle),      "row-activated", G_CALLBACK(gtkTreeRowActived), ih);
  g_signal_connect(G_OBJECT(ih->handle),    "key-press-event", G_CALLBACK(gtkTreeKeyPressEvent), ih);
  g_signal_connect(G_OBJECT(ih->handle),  "key-release-event", G_CALLBACK(gtkTreeKeyReleaseEvent), ih);
  g_signal_connect(G_OBJECT(ih->handle), "button-press-event", G_CALLBACK(gtkTreeButtonEvent), ih);
  g_signal_connect(G_OBJECT(ih->handle), "button-release-event",G_CALLBACK(gtkTreeButtonEvent), ih);

  /* add to the parent, all GTK controls must call this. */
  iupgtkBaseAddToParent(ih);

  if (!iupAttribGetBoolean(ih, "CANFOCUS"))
    GTK_WIDGET_FLAGS(ih->handle) &= ~GTK_CAN_FOCUS;

  gtk_widget_realize((GtkWidget*)scrolled_window);
  gtk_widget_realize(ih->handle);

  /* Initialize the default images */
  ih->data->def_image_leaf = iupImageGetImage("IMGLEAF", ih, 0);
  ih->data->def_image_collapsed = iupImageGetImage("IMGCOLLAPSED", ih, 0);
  ih->data->def_image_expanded = iupImageGetImage("IMGEXPANDED", ih, 0);

  gtkTreeAddRootNode(ih);

  /* configure for DRAG&DROP of files */
  if (IupGetCallback(ih, "DROPFILES_CB"))
    iupAttribSetStr(ih, "DRAGDROP", "YES");

  IupSetCallback(ih, "_IUP_XY2POS_CB", (Icallback)gtkTreeConvertXYToPos);

  return IUP_NOERROR;
}

void iupdrvTreeInitClass(Iclass* ic)
{
  /* Driver Dependent Class functions */
  ic->Map = gtkTreeMapMethod;

  /* Visual */
  iupClassRegisterAttribute(ic, "BGCOLOR", NULL, gtkTreeSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "TXTBGCOLOR", IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "FGCOLOR", NULL, gtkTreeSetFgColorAttrib, IUPAF_SAMEASSYSTEM, "TXTFGCOLOR", IUPAF_DEFAULT);

  /* IupTree Attributes - GENERAL */
  iupClassRegisterAttribute(ic, "EXPANDALL",  NULL, gtkTreeSetExpandAllAttrib,  NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "INDENTATION",  gtkTreeGetIndentationAttrib, gtkTreeSetIndentationAttrib, NULL, NULL, IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "COUNT", gtkTreeGetCountAttrib, NULL, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "DRAGDROP", NULL, iupgtkSetDragDropAttrib, NULL, NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "SPACING", iupTreeGetSpacingAttrib, gtkTreeSetSpacingAttrib, NULL, NULL, IUPAF_NOT_MAPPED);
  iupClassRegisterAttribute(ic, "TOPITEM", NULL, gtkTreeSetTopItemAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);

  /* IupTree Attributes - IMAGES */
  iupClassRegisterAttributeId(ic, "IMAGE", NULL, gtkTreeSetImageAttrib, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "IMAGEEXPANDED", NULL, gtkTreeSetImageExpandedAttrib, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);

  iupClassRegisterAttribute(ic, "IMAGELEAF",            NULL, gtkTreeSetImageLeafAttrib, IUPAF_SAMEASSYSTEM, "IMGLEAF", IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMAGEBRANCHCOLLAPSED", NULL, gtkTreeSetImageBranchCollapsedAttrib, IUPAF_SAMEASSYSTEM, "IMGCOLLAPSED", IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMAGEBRANCHEXPANDED",  NULL, gtkTreeSetImageBranchExpandedAttrib, IUPAF_SAMEASSYSTEM, "IMGEXPANDED", IUPAF_NO_INHERIT);

  /* IupTree Attributes - NODES */
  iupClassRegisterAttributeId(ic, "STATE",  gtkTreeGetStateAttrib,  gtkTreeSetStateAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "DEPTH",  gtkTreeGetDepthAttrib,  NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "KIND",   gtkTreeGetKindAttrib,   NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "PARENT", gtkTreeGetParentAttrib, NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "COLOR",  gtkTreeGetColorAttrib,  gtkTreeSetColorAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "NAME",   gtkTreeGetTitleAttrib,   gtkTreeSetTitleAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "TITLE",   gtkTreeGetTitleAttrib,   gtkTreeSetTitleAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "USERDATA",   gtkTreeGetUserDataAttrib,   gtkTreeSetUserDataAttrib, IUPAF_NO_STRING|IUPAF_NO_INHERIT);
  
  iupClassRegisterAttributeId(ic, "CHILDCOUNT",   gtkTreeGetChildCountAttrib,   NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "TITLEFONT", gtkTreeGetTitleFontAttrib, gtkTreeSetTitleFontAttrib, IUPAF_NO_INHERIT);

  /* IupTree Attributes - MARKS */
  iupClassRegisterAttributeId(ic, "MARKED",   gtkTreeGetMarkedAttrib,   gtkTreeSetMarkedAttrib,   IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute  (ic, "MARK",    NULL,    gtkTreeSetMarkAttrib,    NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute  (ic, "STARTING", NULL, gtkTreeSetMarkStartAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute  (ic, "MARKSTART", NULL, gtkTreeSetMarkStartAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  iupClassRegisterAttribute  (ic, "VALUE",    gtkTreeGetValueAttrib,    gtkTreeSetValueAttrib,    NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  /* IupTree Attributes - ACTION */
  iupClassRegisterAttributeId(ic, "DELNODE", NULL, gtkTreeSetDelNodeAttrib, IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "RENAME",  NULL, gtkTreeSetRenameAttrib,  NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "MOVENODE",  NULL, gtkTreeSetMoveNodeAttrib,  IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "COPYNODE",  NULL, gtkTreeSetCopyNodeAttrib,  IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "FINDUSERDATA", gtkTreeGetFindUserDataAttrib, NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);

  iupClassRegisterAttribute  (ic, "AUTODRAGDROP",    NULL,    NULL,    NULL, NULL, IUPAF_DEFAULT);
}
