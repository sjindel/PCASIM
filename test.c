#include <stdio.h>
#include <stdlib.h>
#include "xalloc.h"
#include "internals.h"

int main(int argc, char** argv)
{
    sim_table* t = new_table();

    char* init = xcalloc(100,sizeof(char));
    sim_desc pca = {1, 110, 100, 100, init};
    init[99] = 1;

    for (int i = 0; i < 100; i++)
    {

	// Run a simulation.

	simulation* result = run(&pca, i);

	table_add(result,t);

    }

    if (!table_find(&pca,1,50,t))
    {
	printf("Failure.\n");
	abort();
    }

    free(init);

    table_free(t);

    printf("All tests passed successfullly.\n");

    return 0;
}
