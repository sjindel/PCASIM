#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "utils.h"

int main(int argc, char** argv)
{
    // Initialize a sim_desc.

    char* trace = calloc(100,sizeof(char));
    trace[99] = 1;

    sim_desc pca = {1, 110, 100, 100, trace};

    simulation* result = run(&pca, time(0));

    print_simulation(result);

    return 0;
}
