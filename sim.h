#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dSFMT.h"

typedef struct peca1d
{
    // Automata parameters.
    double p;
    char rule;

    // Instance parameters.
    int width;
    int pos;
    int height;
    char* trace;

} peca1d;

void update(peca1d* x, dsfmt_t* dsfmt_p);

int* diff(peca1d* x, peca1d* y);

void print_peca1d(peca1d* x);

void run(peca1d* x, dsfmt_t* rng);
