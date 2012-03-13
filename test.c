#include <stdio.h>
#include <stdlib.h>
#include "xalloc.h"
#include "internals.h"
#include "utils.h"
#include "commands.h"

int main(int argc, char** argv)
{
    sim_table* t = new_table();

    char* init = xcalloc(100,sizeof(char));
    init[50] = 1;

    sim_context* context = malloc(sizeof(struct sim_context));

    context->p = 1.0;
    context->width = 100;
    context->height = 100;
    context->rule = 110;
    context->initial = init;
    context->seed = time(0);

    sim_desc pca = {1.0, 110, 100, 100, init};

    sim_run(context,t);

    simulation* s = table_find(&pca,1,context->seed,t);

    if (s == NULL)
    {
	printf("Failure.\n");
	abort();
    }

    print_simulation(s);

    free(init);

    free(context);

    table_free(t);

    printf("All tests passed successfullly.\n");

    return 0;
}
