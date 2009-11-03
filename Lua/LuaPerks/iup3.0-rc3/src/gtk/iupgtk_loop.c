/** \file
 * \brief GTK Message Loop
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>    
#include <string.h>    

#include <gtk/gtk.h>

#include "iup.h"
#include "iupcbs.h"


/* local variables */
static IFidle gtk_idle_cb = NULL;
static guint gtk_idle_id;

static gboolean gtkIdleFunc(gpointer data)
{
  (void)data;
  if (gtk_idle_cb)
  {
    int ret = gtk_idle_cb();
    if (ret == IUP_CLOSE)
    {
      gtk_idle_cb = NULL;
      IupExitLoop();
      return FALSE; /* removes the idle */
    }
    if (ret == IUP_IGNORE)
    {
      gtk_idle_cb = NULL;
      return FALSE; /* removes the idle */
    }

    return TRUE; /* keeps the idle */
  }

  return FALSE; /* removes the idle */
}

void iupdrvSetIdleFunction(Icallback f)
{
  if (gtk_idle_cb)
    g_source_remove(gtk_idle_id);

  gtk_idle_cb = (IFidle)f;

  if (gtk_idle_cb)
    gtk_idle_id = g_idle_add(gtkIdleFunc, NULL);
}

void IupExitLoop(void)
{
  if (gtk_main_iteration_do(FALSE)==FALSE)
    gtk_main_quit();
}

int IupMainLoopLevel(void)
{
  return gtk_main_level();
}

int IupMainLoop(void)
{
  gtk_main();
  return IUP_NOERROR;
}

int IupLoopStep(void)
{
  if (gtk_main_iteration_do(FALSE))
    return IUP_CLOSE;
  return IUP_DEFAULT;
}

void IupFlush(void)
{
  IFidle old_gtk_idle_cb = NULL;
  if (gtk_idle_cb)
  {
    old_gtk_idle_cb = gtk_idle_cb;
    iupdrvSetIdleFunction(NULL);
  }

  while (gtk_events_pending())
    gtk_main_iteration();

  if (old_gtk_idle_cb)
    iupdrvSetIdleFunction((Icallback)old_gtk_idle_cb);
}
