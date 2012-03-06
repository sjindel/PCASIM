#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dSFMT.h"
#include "diff.h"
#include "xalloc.h"

// sim_node is the container for simulations that are put into the hashtable.

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

// context stores the environment variables of the interpreter

typedef struct context
{

    // Automata parameters.
    double p;
    char rule;

    // Instance parameters.
    int width;
    int height;

    // Initial value parameter.
    char* initial;

    // Seed parameter.
    int seed;

} context;


int desc_cmp (sim_desc* x, sim_desc* y);
int desc_hash (sim_desc* s);
void add (simulation* s, sim_table* t);
simulation* find (sim_desc* d, sim_table* t);
void balance(sim_table* t);
void table_free(sim_table* t);
sim_table* new_table();
