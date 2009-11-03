/** \file
 * \brief iuptree control internal definitions.
 *
 * See Copyright Notice in iup.h
 */

#ifndef __IUP_TREE_H 
#define __IUP_TREE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Kinds of node */
#define ITREE_BRANCH        0
#define ITREE_LEAF          1

#define ITREE_MARK_SINGLE 0
#define ITREE_MARK_MULTIPLE 1

#define ITREE_UPDATEIMAGE_LEAF      1
#define ITREE_UPDATEIMAGE_COLLAPSED 2
#define ITREE_UPDATEIMAGE_EXPANDED  3

void iupdrvTreeInitClass(Iclass* ic);
void iupTreeUpdateImages(Ihandle *ih);
void iupdrvTreeAddNode(Ihandle* ih, const char* id_string, int kind, const char* title, int add);
void iupdrvTreeUpdateMarkMode(Ihandle *ih);

char* iupTreeGetSpacingAttrib(Ihandle* ih);

/* Structure of the tree */
struct _IcontrolData
{
  int mark_mode,
      add_expanded,
      show_dragdrop,
      show_rename,
      spacing;

  void* def_image_leaf;       /* Default image leaf */
  void* def_image_collapsed;  /* Default image collapsed */
  void* def_image_expanded;   /* Default image expanded */

  void* def_image_leaf_mask;       /* Motif Only */
  void* def_image_collapsed_mask;  
  void* def_image_expanded_mask;   

  int id_control;  /* auxiliary variable for computing or finding the id of a node */
};


#ifdef __cplusplus
}
#endif

#endif
