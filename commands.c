#include "commands.h"
#define FILE_NAME_BUF_SIZE 128
#define BYTES_PER_PIXEL 8

// Set a context variable.

int sim_set (sim_context* context, sim_table* table, char* args)
{
    int l = strlen(args);
    char var[l];
    char val[l];

    if (sscanf(args,"%s %s",var,val) == 2)
    {
	if (strcmp(var,"p") == 0)
	{
	    double p;
	    if (sscanf(val,"%lf",&p) == 1)
	    {
		context->p = p;
		return 0;
	    }
	    printf("Invalid value for parameter `p'.\n");
	    return 1;
	}
	if (strcmp(var,"rule") == 0)
	{
	    int rule;
	    if (sscanf(val,"%d",&rule) == 1)
	    {
		if ((rule <= 255) && (rule >= 0))
		{
		    context->rule = rule;
		    return 0;
		}
		printf("Parameter `rule' must be in range [0,255].\n");
		return 1;
	    }
	    printf("Invalid value for parameter `rule'.\n");
	    return 1;
	}
	if (strcmp(var,"width") == 0)
	{
	    int width;
	    if (sscanf(val,"%d",&width) == 1)
	    {
		context->width = width;
		return 0;
	    }
	    printf("Invalid value for parameter `width'.\n");
	    return 1;
	}
	if (strcmp(var,"height") == 0)
	{
	    int height;
	    if (sscanf(val,"%d",&height) == 1)
	    {
		context->height = height;
		return 0;
	    }
	    printf("Invalid value for parameter `height'.\n");
	    return 1;
	}
	// TODO: Put in case for initial value parameter.
	if (strcmp(var,"seed") == 0)
	{
	    int seed;
	    if (sscanf(val,"%d",&seed) == 1)
	    {
		context->seed = seed;
		return 0;
	    }
	    printf("Invalid value for parameter `seed'.\n");
	    return 1;
	}
    }
    printf("Invalid arguments to `set'.\n");
    return 1;
}

// Run a simulation of the current context.

int sim_run (sim_context* context, sim_table* table)
{

    sim_desc d;
    d.p = context->p;
    d.rule = context->rule;
    d.width = context->width;
    d.height = context->height;
    d.initial = context->initial;

    simulation* sim = run(&d,context->seed);

    table_add(sim,table);


    return 0;
}

int sim_disp(sim_context* context, sim_table* table)
{
    /*
      Write an image corresponding to the simulation trace
      in an appropriate output file and send it to eog.
    */

    sim_desc d = {context->p, context->rule, context->width, context->height,
		  context->initial};

    simulation* s = table_find(&d,1,context->seed,table);

    if (s == NULL)
    {
	printf("Context simulation has not been run.\n");
	return 1;
    }

    char buffer[FILE_NAME_BUF_SIZE];

    sprintf(buffer,"/tmp/%lf.%d.%d.%d.%d.trace.csv",
	    d.p, d.rule, d.width, d.height, s->mt_seed);

    FILE* f = fopen(buffer,"w");

    if (!f)
    {
	printf("Could not open output file.\n");
	return 1;
    }

    write_simulation_csv(s,f);

    fclose(f);

    int x = fork();
    if (x == 0)
	execl("display_trace",buffer);

    return 0;
}

// Write simulations in the hash table to disk
// (in the current working directory).

int sim_write(sim_table* table)
{
    for (int i = 0; i < table->h; i++)
    {
	sim_node* c = table->array[i];

	while(c != NULL)
	{
	    // Write the simulation in this node to a file.

	    sim_desc d = c->s->desc;

	    char buffer[FILE_NAME_BUF_SIZE];

	    sprintf(buffer,"%lf.%d.%d.%d.%d.trace.csv",
		    d.p, d.rule, d.width, d.height, c->s->mt_seed);

	    FILE* f = fopen(buffer,"w");

	    if (f == NULL)
	    {
		printf("Error: could not open output file.\n");
		return 1;
	    }

	    write_simulation_csv(c->s,f);

	    fclose(f);

	    c = c->next;

	}

    }

    return 0;
}

// Print out the current environment variables.

int sim_show (sim_context* context)
{
    printf("p: %lf\nwidth: %d\nheight: %d\nseed: %d\n",
	   context->p,context->width,context->height,context->seed);
    return 0;
}

// Print out statistics of the table.

int sim_stat (sim_table* table)
{
    // Print out the number of elements in each row.

    for(int i = 0; i < table->h; i++)
    {
	int s = 0;
	sim_node* c = table->array[i];
	while (c != NULL)
	{
	    s++;
	    c = c->next;
	}
	printf("Row %d has %d elements.\n",i,s);
    }

    printf("Table stats:\n");
    printf("Elements: %d\n",table->n);
    printf("Size: %d\n",table->h);

    return 0;
}
