/** \file
 * \brief Ihandle <-> Name table manager.
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdlib.h>

#include "iup.h"

#include "iup_str.h"
#include "iup_table.h"
#include "iup_names.h"
#include "iup_object.h"
#include "iup_class.h"
#include "iup_assert.h"
#include "iup_str.h"


static Itable *inames_strtable = NULL;   /* table indexed by name containing Ihandle* address */
static Itable *inames_ihtable = NULL;    /* table indexed by Ihandle* address containing names */

void iupNamesDestroyHandles(void)
{
  char *name;
  Ihandle** ih_array, *ih;
  int count, i = 0;

  count = iupTableCount(inames_strtable);
  if (!count)
    return;

  ih_array = (Ihandle**)malloc(count * sizeof(Ihandle*));

  /* store the names before updating so we can remove elements in the loop */
  name = iupTableFirst(inames_strtable);
  while (name)
  {
    ih = (Ihandle*)iupTableGetCurr(inames_strtable);
    if (iupObjectCheck(ih))
    {
      ih_array[i] = ih;
      i++;
    }
    name = iupTableNext(inames_strtable);
  }

  count = i;
  for (i = 0; i < count; i++)
  {
    if (iupObjectCheck(ih_array[i]))
      IupDestroy(ih_array[i]);
  }

  free(ih_array);
}

void iupNamesInit(void)
{
  inames_strtable = iupTableCreate(IUPTABLE_STRINGINDEXED);
  inames_ihtable = iupTableCreate(IUPTABLE_POINTERINDEXED);
}

void iupNamesFinish(void)
{
  iupTableDestroy(inames_strtable);
  inames_strtable = NULL;

  iupTableDestroy(inames_ihtable);
  inames_ihtable = NULL;
}

void iupRemoveAllNames(Ihandle* ih)
{
  char *name;
  Ihandle *cur_ih;

  name = iupTableFirst(inames_strtable);
  while (name)
  {
    cur_ih = (Ihandle*)iupTableGetCurr(inames_strtable);
    if (iupObjectCheck(cur_ih) && cur_ih == ih)
      iupTableRemoveCurr(inames_strtable);

    name = iupTableNext(inames_strtable);
  }

  iupTableRemove(inames_ihtable, (char*)ih);
}

Ihandle *IupGetHandle(const char *name)
{
  if (!name) /* no iupASSERT needed here */
    return NULL;
  return (Ihandle*)iupTableGet (inames_strtable, name);
}

Ihandle* IupSetHandle(const char *name, Ihandle *ih)
{
  Ihandle *old_ih;

  iupASSERT(name!=NULL);
  if (!name)
    return NULL;

  old_ih = iupTableGet(inames_strtable, name);
  if (ih != NULL)
  {
    iupTableSet(inames_strtable, name, ih, IUPTABLE_POINTER);
    iupTableSet(inames_ihtable, (char*)ih, (char*)name, IUPTABLE_STRING);  /* keep only the last name set */
  }
  else
  {
    ih = iupTableGet(inames_strtable, name);
    iupTableRemove(inames_strtable, name);
    if (ih) 
    {
      char* cur_name = iupTableGet(inames_ihtable, (char*)ih);
      if (iupStrEqualNoCase(cur_name, name))
        iupTableRemove(inames_ihtable, (char*)ih);
    }
  }
  return old_ih;
}

int IupGetAllNames(char** names, int n)
{
  int i = 0;
  char* name;

  if (!names || !n)
    return iupTableCount(inames_strtable);

  name = iupTableFirst(inames_strtable);
  while (name)
  {
    names[i] = name;
    i++;
    if (i == n)
      break;

    name = iupTableNext(inames_strtable);
  }
  return i;
}

static int iNamesCountDialogs(void)
{
  int i = 0;
  char* name = iupTableFirst(inames_strtable);
  while (name)
  {
    Ihandle* dlg = (Ihandle*)iupTableGetCurr(inames_strtable);
    if (iupObjectCheck(dlg) && dlg->iclass->nativetype == IUP_TYPEDIALOG)
      i++;

    name = iupTableNext(inames_strtable);
  }
  return i;
}

int IupGetAllDialogs(char** names, int n)
{
  int i = 0;
  char* name;

  if (!names || !n)
    return iNamesCountDialogs();

  name = iupTableFirst(inames_strtable);
  while (name)
  {
    Ihandle* dlg = (Ihandle*)iupTableGetCurr(inames_strtable);
    if (iupObjectCheck(dlg) && dlg->iclass->nativetype == IUP_TYPEDIALOG)
    {
      names[i] = name;
      i++;
      if (i == n)
        break;
    }

    name = iupTableNext(inames_strtable);
  }
  return i;
}

char* IupGetName(Ihandle* ih)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return NULL;
  return iupTableGet(inames_ihtable, (char*)ih);
}
