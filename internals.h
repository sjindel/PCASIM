#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dSFMT.h"
#include "language.h"

typedef struct sim_node
{
    simulation* s;
    struct sim_node* next;
} sim_node;

// sim_table is the hashtable that will store the simulations.
typedef struct sim_table
{
    int n; // Number of simulations in the table.
    int h; // Size of the array.

    sim_node* array;

} sim_table;
