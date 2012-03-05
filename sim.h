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

void update(pca1d* x, dsfmt_t* dsfmt_p);

int* diff(pca1d* x, pca1d* y);

void print_pca1d(pca1d* x);

void run(pca1d* x, dsfmt_t* rng);
