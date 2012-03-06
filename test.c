#include <stdio.h>
#include <stdlib.h>
#include "xalloc.h"
#include "internals.h"

int main(int argc, char** argv)
{
    // Initialize a sim_desc.

    char* init = xcalloc(100,sizeof(char));
    init[99] = 1;

    sim_desc pca = {1, 110, 100, 100, init};

    // Run a simulation.

    simulation* result = run(&pca, time(0));
    printf("%p\n",result);

    // Initialize a sim_table.

    sim_table* t = new_table();

    add(result,t);

    simulation* x = find(&pca,t);

    printf("%p\n",x);

    return 0;
}
