#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "dSFMT.h"

typedef struct sim_desc
{
    // Automata parameters.
    double p;
    char rule;

    // Instance parameters.
    int width;
    int pos;
    int height;

    char* initial;

} sim_desc;

typedef struct simulation
{
    sim_desc desc;
    char* trace;
    int mt_seed;

} simulation;

simulation* run (sim_desc* x, int seed);

//int* diff(simulation* x, simulation* y);
