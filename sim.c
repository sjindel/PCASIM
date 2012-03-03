#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dSFMT.h"

typedef struct pca1d
{
    // Automata parameters.
    double p;
    char rule;

    // Instance parameters.
    int width;
    int pos;
    int height;
    char* trace;

} pca1d;

void update(pca1d* x, dsfmt_t* dsfmt_p)
{

    // Sanity check.
    if (x->pos >= x->height)
    {
	printf("Execution complete, nothing to do.\n");
	return;
    }

    // Declare new array.
    char t[x->width];

    int w = x->width;
    int p = x->pos;

    // Update new array.
    for (int i = 0; i < w; i++)
    {
	char new_state;
	double z = dsfmt_genrand_open_open(dsfmt_p);
	if (z < (x->p))
	{
	    int q;
	    if (i - 1 < 0)
		q = i-1+w;
	    else
		q = i-1;

	    char configuration =
		(x->trace[(p-1)*w + q]  << 2)
		+ (x->trace[(p-1)*w*sizeof(char) + i]  << 1)
		+ (x->trace[(p-1)*w*sizeof(char) + ((i+1) % w)]  << 0);

	    new_state = (x->rule >> configuration) & 1;
	}
	else
	{
	    new_state = x->trace[(p-1)*w + i];
	}

	t[i] = new_state;
    }

    // Copy new array into appropriate line of old array.

    for (int i = 0; i < w; i++)
	x->trace[p*w + i] = t[i];

    // Increment positition counter
    x->pos++;

}

// Requires that the height and width of x and y be equal, returns NULL otherwise.
int* diff(pca1d* x, pca1d* y)
{
    if (x->height != y->height)
	return NULL;

    if (x->width != y->width)
	return NULL;

    int h = x->height;
    int w = x->width;

    int* difference = calloc(h,sizeof(int));

    if (difference == NULL)
    {
	printf("Could not allocate memory.\n");
	abort();
    }

    for (int l = 0; l < h; l ++)
	for (int i = 0; i < w; i++)
	    if (x->trace[l*w + i] != y->trace[l*w + i])
		difference[l]++;

    return difference;
}

void print_pca1d(pca1d* x)
{
    for (int i = 0; i < x->height; i++)
    {
	for (int j = 0; j < x->width; j++)
	    printf("%d,",x->trace[i*x->width + j]);
	printf("\n");
    }
}

int main(int argc, char* argv[])
{
    // Initialize random number generator.
    dsfmt_t dsfmt;
    dsfmt_init_gen_rand(&dsfmt,time(0));

    pca1d test;
    sscanf(argv[1],"%lf",&test.p);
    test.rule = 30;
    test.width = 200;
    test.height = 100;
    test.trace = calloc(test.width*test.height,sizeof(char));
    if (test.trace == NULL)
    {
	printf("Couldn't allocate memory.\n");
	abort();
    }
    test.pos = 1;
    test.trace[test.width/2]=1;

    while (test.pos < test.height)
	update(&test,&dsfmt);

    print_pca1d(&test);

    return 1;
}
