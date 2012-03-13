#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "dsfmt/dSFMT.h"
#include "diff.h"
#include "xalloc.h"

#ifndef __INTERNALS_H
#define __INTERNALS_H

// sim_node is the container for simulations that are put into the hashtable.

typedef struct sim_node sim_node;
struct sim_node
{
    simulation* s;
    sim_node* next;

};

// sim_table is the hashtable that will store the simulations.

typedef struct sim_table
{
    int n; // Number of simulations in the table.
    int h; // Size of the array.

    sim_node** array;

} sim_table;

// sim_context stores the environment variables of the interpreter

typedef struct sim_context
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

} sim_context;


int desc_cmp (sim_desc* x, sim_desc* y);
int desc_hash (sim_desc* s);
void table_add (simulation* s, sim_table* t);
simulation* table_find (sim_desc* d, char s, int seed, sim_table* t);
void table_balance(sim_table* t);
void table_free(sim_table* t);
sim_table* new_table();

#endif
