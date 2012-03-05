#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "utils.h"

int main(int argc, char** argv)
{
    // Initialize a sim_desc.

    char* init = calloc(100,sizeof(char));
    init[99] = 1;

    sim_desc pca = {1, 110, 100, 100, init};

    simulation* result = run(&pca, time(0));

    print_simulation(result);

    free(init);

    free(result->trace);

    free(result);

    return 0;
}
