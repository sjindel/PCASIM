#include "diff.h"

/*
  Difference functions require that the height and width of x and y be equal,
  return NULL otherwise
*/

// Compute array of row-wise hamming distance.

int* diff_hamming(simulation* x, simulation* y)
{
    if (x->desc.height != y->desc.height)
	return NULL;

    if (x->desc.width != y->desc.width)
	return NULL;

    int h = x->desc.height;
    int w = x->desc.width;

    int* diff = xcalloc(h,sizeof(int));

    for (int l = 0; l < h; l++)
	for (int i = 0; i < w; i++)
	    if (x->trace[l*w + i] != y->trace[l*w + i])
		diff[l]++;

    return diff;
}

// Compute array of row-wise tally difference.

int* diff_tally(simulation* x, simulation* y)
{
    if (x->desc.height != y->desc.height)
	return NULL;

    if (x->desc.width != y->desc.width)
	return NULL;

    int h = x->desc.height;
    int w = x->desc.width;

    int* diff = xcalloc(h,sizeof(int));

    for (int l = 0; l < h; l++)
    {
	int xs;
	for (int i = 0; i < w; i++)
	    xs += x->trace[l*w + i];
	int ys;
	for (int i = 0; i < w; i++)
	    ys += y->trace[l*w + i];
	diff[l] = xs-ys.
    }

    return diff;
}
