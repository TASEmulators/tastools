/*---------------------------------------------------------------------------*
 |              PDFlib - A library for generating PDF on the fly             |
 +---------------------------------------------------------------------------+
 | Copyright (c) 1997-2006 Thomas Merz and PDFlib GmbH. All rights reserved. |
 +---------------------------------------------------------------------------+
 |                                                                           |
 |    This software is subject to the PDFlib license. It is NOT in the       |
 |    public domain. Extended versions and commercial licenses are           |
 |    available, please check http://www.pdflib.com.                         |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* $Id: pc_contain.c,v 1.1 2008/10/17 06:10:43 scuri Exp $
 *
 * PDFlib generic container classes
 *
 */

#include "pc_util.h"
#include "pc_contain.h"


/**************************** avl tree class ****************************/

typedef struct avl_node_s avl_node;

struct avl_node_s
{
    const char *name;
    int		balance;
    avl_node *	left;
    avl_node *	right;
};

#undef	COMMENT
#ifdef	COMMENT

before single rotate right; after insertion of X.

                       root(-2)

                    /            \

               lc(-1)           +------+
                                | rc   |
              /      \          |      |
                                |      |
        +------+    +------+    |      |
        | llc  |    | rlc  |    | n    |
        |      |    |      |    +------+
        |      |    |      |
        |      |    |      |
        | n    |    | n    |
        +------+    +------+

            |

            X


after single rotate right.

                  lc(0)

               /        \

        +------+          root(0)
        | llc  |
        |      |        /         \
        |      |
        |      |    +------+    +------+
        | n    |    | rlc  |    | rc   |
        +------+    |      |    |      |
                    |      |    |      |
            |       |      |    |      |
                    | n    |    | n    |
            X       +------+    +------+



before double rotate right; after insertion of X/Y.

                             root(-2)

                       /                  \

                lc(+1)                      +------+
                                            | rc   |
              /        \                    |      |
                                            |      |
        +------+         rlc(-1/+1)         |      |
        | llc  |                            |      |
        |      |        /         \         |      |
        |      |                            |      |
        |      |    +------+    +------+    | n    |
        |      |    | lrlc |    | rrlc |    +------+
        |      |    |      |    |      |
        |      |    |      |    |      |
        | n    |    | n-1  |    | n-1  |
        +------+    +------+    +------+

                        |           |

			X           Y


after double rotate right:

                          rlc(0)

                      /            \

              lc(0/-1)               root(+1/0)

              /      \                /      \

        +------+    +------+    +------+    +------+
        | llc  |    | lrlc |    | rrlc |    | rc   |
        |      |    |      |    |      |    |      |
        |      |    |      |    |      |    |      |
        |      |    | n-1  |    | n-1  |    |      |
        |      |    +------+    +------+    |      |
        |      |                            |      |
        |      |        |           |       |      |
        | n    |                            | n    |
        +------+        X           Y       +------+


avl_node *
avl_insert(avl_node *root, const char *name, pdc_bool *change_parent_balance)
{
    pdc_bool change_balance = pdc_false;

    if (root == 0)
    {
	avl_node *result = (avl_node *) malloc(sizeof (avl_node));

	result->name = name;
	result->balance = 0;
	result->left = (avl_node *) 0;
	result->right = (avl_node *) 0;
	*change_parent_balance = pdc_true;
	return result;
    }

    if (strcmp(name, root->name) < 0)
    {
	root->left = avl_insert(root->left, name, &change_balance);

	if (change_balance && --root->balance < 0)
	{
	    if (root->balance == -1)
	    {
		*change_parent_balance = pdc_true;
	    }
	    else				/* root->balance == -2 */
	    {
		avl_node *lc = root->left;	/* left child */
		avl_node *rlc = lc->right;	/* right of left child */

		if (lc->balance == -1)		/* single rotate right */
		{
		    root->left = rlc;
		    lc->right = root;
		    lc->balance = root->balance = 0;
		    return lc;
		}
		else				/* double rotate right */
		{
		    root->balance = (rlc->balance == -1) ? +1 : 0;
		    lc->balance   = (rlc->balance == +1) ? -1 : 0;
		    rlc->balance  = 0;
		    lc->right	= rlc->left;
		    rlc->left	= lc;
		    root->left	= rlc->right;
		    rlc->right	= root;
		    return rlc;
		}
	    }
	}
    }
    else
    {
	root->right = avl_insert(root->right, name, &change_balance);

	if (change_balance && ++root->balance > 0)
	{
	    if (root->balance == +1)
	    {
		*change_parent_balance = pdc_true;
	    }
	    else				/* root->balance == +2 */
	    {
		avl_node *rc = root->right;	/* right child */
		avl_node *lrc = rc->left;	/* left of right child */

		if (rc->balance == +1)		/* single rotate left */
		{
		    root->right = lrc;
		    rc->left = root;
		    rc->balance = root->balance = 0;
		    return rc;
		}
		else				/* double rotate left */
		{
		    root->balance = (lrc->balance == +1) ? -1 : 0;
		    rc->balance   = (lrc->balance == -1) ? +1 : 0;
		    lrc->balance  = 0;
		    rc->left	= lrc->right;
		    lrc->right	= rc;
		    root->right	= lrc->left;
		    lrc->left	= root;
		    return lrc;
		}
	    }
	}
    }

    return root;
}

#endif	/* COMMENT */

/***************************** vector class *****************************/

struct pdc_vtr_s
{
    pdc_core *	pdc;

    pdc_ced	ced;		/* container entry descriptor		*/
    void *	context;	/* client context			*/

    char **	ctab;		/* chunk table				*/
    int		ctab_size;	/* current # of slots			*/
    int		ctab_incr;
    int		chunk_size;	/* # of items per chunk			*/
    int		size;		/* current # of items total		*/
};


static const pdc_vtr_parms vtr_dflt_parms =
{
    0,				/* init_size				*/
    100,			/* chunk_size				*/
    10				/* ctab_incr				*/
};

void
pdc_vtr_dflt_parms(pdc_vtr_parms *vp)
{
    *vp = vtr_dflt_parms;
}


static void
pdc_vtr_grow_ctab(pdc_vtr *v, int new_size)
{
    static const char fn[] = "pdc_vtr_grow_ctab";

    int i;

    v->ctab = (char **)
	pdc_realloc(v->pdc, v->ctab, (size_t) (new_size * sizeof (char *)), fn);

    for (i = v->ctab_size; i < new_size; ++i)
	v->ctab[i] = (char *) 0;

    v->ctab_size = new_size;
} /* pdc_vtr_grow_ctab */


pdc_vtr *
pdc_vtr_new(
    pdc_core *pdc,
    const pdc_ced *ced,
    void *context,
    const pdc_vtr_parms *parms)
{
    static const char fn[] = "pdc_vtr_new";

    pdc_vtr *v = (pdc_vtr *) pdc_malloc(pdc, sizeof (pdc_vtr), fn);

    if (!parms)
	parms = &vtr_dflt_parms;

    v->pdc = pdc;
    v->ced = *ced;
    v->context = context ? context : pdc;

    v->ctab = (char **) 0;
    v->ctab_size = 0;
    v->ctab_incr = parms->ctab_incr;
    v->chunk_size = parms->chunk_size;
    v->size = 0;

    if (parms->init_size != 0)
    {
	PDC_TRY (pdc)
	{
	    pdc_vtr_resize(v, parms->init_size);
	}
	PDC_CATCH (pdc)
	{
	    pdc_vtr_delete(v);
	    PDC_RETHROW(pdc);
	}
    }

    return v;
} /* pdc_vtr_new */


void
pdc_vtr_delete(pdc_vtr *v)
{
    int cs = v->chunk_size;
    int i;

    if (v->size != 0 && v->ced.release)
    {
	for (i = 0; i < v->size; ++i)
	{
	    v->ced.release(v->context, (void *)
		&v->ctab[i / cs][(i % cs) * v->ced.size]);
	}
    }

    for (i = 0; i < v->ctab_size && v->ctab[i] != (char *) 0; ++i)
    {
	pdc_free(v->pdc, v->ctab[i]);
    }

    if (v->ctab)
	pdc_free(v->pdc, v->ctab);

    pdc_free(v->pdc, v);
} /* pdc_vtr_delete */


int
pdc_vtr_size(const pdc_vtr *v)
{
    return (int) v->size;
} /* pdc_vtr_size */


void
pdc_vtr_resize(pdc_vtr *v, int new_size)
{
    static const char fn[] = "pdc_vtr_resize";

    int cs = v->chunk_size;

    PDC_ASSERT(v->pdc, 0 <= new_size);

    if (new_size < v->size)
    {
	if (!v->ced.release)
	{
	    v->size = new_size;
	}
	else
	{
	    do
	    {
		--v->size;

		v->ced.release(v->context, (void *)
		    &v->ctab[v->size / cs][(v->size % cs) * v->ced.size]);
	    } while (new_size < v->size);
	}

	/* TODO: free chunks if possible? */
    }
    else if (new_size > v->size)
    {
	int curr_slot = v->size / cs;
	int new_ctsize = (new_size + cs - 1) / cs;
	int i;

	if (v->ctab_size < new_ctsize)
	    pdc_vtr_grow_ctab(v, new_ctsize);

	for (i = curr_slot; i < new_ctsize; ++i)
	{
	    if (v->ctab[i] == (char *) 0)
	    {
		v->ctab[i] = (char *)
		    pdc_malloc(v->pdc, (size_t) (cs * v->ced.size), fn);
	    }
	}

	if (v->ced.reclaim)
	{
	    for (i = v->size; i < new_size; ++i)
	    {
		v->ced.reclaim((void *) &v->ctab[i/cs][(i%cs) * v->ced.size]);
	    }
	}

	v->size = new_size;
    }
} /* pdc_vtr_resize */


void *
pdc__vtr_at(const pdc_vtr *v, int idx)
{
    static const char fn[] = "pdc__vtr_at";

    int cs = v->chunk_size;

    if (idx < 0 || v->size <= idx)
	pdc_error(v->pdc, PDC_E_INT_ARRIDX,
	    pdc_errprintf(v->pdc, "%d", idx), fn, 0, 0);
	    /* TODO: "%u" */

    return (void *) (&v->ctab[idx / cs][(idx % cs) * v->ced.size]);
} /* pdc__vtr_at */


#if 0
const void *
pdc__vtr_at_c(const pdc_vtr *v, int idx)
{
    static const char fn[] = "pdc__vtr_at_c";

    int cs = v->chunk_size;

    if (idx < 0 || v->size <= idx)
	pdc_error(v->pdc, PDC_E_INT_ARRIDX,
	    pdc_errprintf(v->pdc, "%d", idx), fn, 0, 0);
	    /* TODO: "%u" */

    return (const void *) (&v->ctab[idx / cs][(idx % cs) * v->ced.size]);
} /* pdc__vtr_at_c */
#endif


void *
pdc__vtr_top(const pdc_vtr *v)
{
    int cs = v->chunk_size;
    int idx;

    if (v->size == 0)
	return (void *) 0;

    idx = v->size - 1;
    return (void *) (&v->ctab[idx / cs][(idx % cs) * v->ced.size]);
} /* pdc__vtr_top */


#if 0
const void *
pdc__vtr_top_c(const pdc_vtr *v)
{
    int cs = v->chunk_size;
    int idx;

    if (v->size == 0)
	return (void *) 0;

    idx = v->size - 1;
    return (const void *) (&v->ctab[idx / cs][(idx % cs) * v->ced.size]);
} /* pdc__vtr_top_c */
#endif


void *
pdc__vtr_push(pdc_vtr *v)
{
    static char fn[] = "pdc__vtr_push";

    int cs = v->chunk_size;
    int idx = v->size;
    int slot = idx / cs;
    char *target;

    if (v->ctab_size <= slot)
	pdc_vtr_grow_ctab(v, v->ctab_size + v->ctab_incr);

    if (v->ctab[slot] == (char *) 0)
    {
	v->ctab[slot] = (char *)
	    pdc_malloc(v->pdc, (size_t) (cs * v->ced.size), fn);
    }

    ++v->size;
    target = &v->ctab[slot][(idx % cs) * v->ced.size];

    if (v->ced.reclaim)
    {
	v->ced.reclaim((void *) target);
    }

    return (void *) target;
} /* pdc__vtr_push */


void
pdc_vtr_pop(pdc_vtr *v)
{
    static char fn[] = "pdc_vtr_pop";

    int cs = v->chunk_size;

    if (v->size == 0)
	pdc_error(v->pdc, PDC_E_INT_STACK_UNDER, fn, 0, 0, 0);

    --v->size;

    if (v->ced.release)
    {
	v->ced.release(v->context, (void *)
	    &v->ctab[v->size / cs][(v->size % cs) * v->ced.size]);
    }
} /* pdc_vtr_pop */
