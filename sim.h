#ifndef _SIM_H
#define _SIM_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "xalloc.h"
#include "dSFMT.h"

typedef struct sim_desc
{
    // Automata parameters.
    double p;
    char rule;

    // Instance parameters.
    int width;
    int height;

    // Initial value parameter.
    char* initial;

} sim_desc;

typedef struct simulation
{
    sim_desc desc;
    char* trace;
    int mt_seed;

} simulation;

simulation* run (sim_desc* x, int seed);
void simulation_free(simulation* s);

#endif
