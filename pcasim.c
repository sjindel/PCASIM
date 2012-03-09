#include "pcasim.h"

// Parse and interpret the line.

int interpret(char* line, sim_context* context, sim_table* table)
{
    // First check for leading loop definition.
    int l = strlen(line);
    char* loop = malloc(l*sizeof(char));
    char* command = malloc(l*sizeof(char));
    char* args = malloc(l*sizeof(char));

    // Clean up.
    free(loop);
    free(command);
    free(args);
    return 0;
}

int main(int argc, char** argv)
{
    // Print startup message.

    printf("Welcome to PCASIM v0.0.1.\n");

    // Initialize context.

    printf("Initializing context...\n");

    sim_context* context = malloc(sizeof(struct sim_context));
    context->p = 1;
    context->rule = 110;
    context->width = 100;
    context->height = 100;
    context->initial = calloc(100,sizeof(char));
    context->initial[0] = 1;
    context->seed = time(0);

    // Initialize hashtable.

    printf("Initializing table...\n");

    sim_table* table = new_table();

    // Main loop.

    while(1)
    {
	char* line = readline("> ");
	if (line == NULL)
	{
	    printf("\n");
	    break;
	}

	interpret(line,context,table);

	free(line);
    }

    // Clean up.
    table_free(table);
    free(context->initial);
    free(context);

    return 0;
}
