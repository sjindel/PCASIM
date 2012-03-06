#include <stdio.h>
#include <stdlib.h>
#include "xalloc.h"
#include "internals.h"

int main(int argc, char** argv)
{
    sim_table* t = new_table();

    char* init = xcalloc(100,sizeof(char));

    for (int i = 0; i < 100; i++)
    {
	printf("Adding simulation %d.\n",i);
	for (int j = 0; j <= i; j++)
	    init[j]=1;

	sim_desc pca = {1, 110, 100, 100, init};

	// Run a simulation.

	simulation* result = run(&pca, time(0));

	add(result,t);

    }

    free(init);

    table_free(t);

    return 0;
}
