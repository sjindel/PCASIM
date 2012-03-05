#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include "dSFMT.h"
#include "diff.h"

struct sim_node
{
    simulation* s;
    struct sim_node* next;

};
typedef struct sim_node sim_node;

// sim_table is the hashtable that will store the simulations.
typedef struct sim_table
{
    int n; // Number of simulations in the table.
    int h; // Size of the array.

    sim_node** array;

} sim_table;


int desc_cmp (sim_desc* x, sim_desc* y);
int desc_hash (sim_desc* s);
void add (simulation* s, sim_table* t);
simulation* find (sim_desc* d, sim_table* t);
void balance(sim_table* t);
