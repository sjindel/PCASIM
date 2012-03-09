#include "commands.h"

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
	    printf("Invalid value for parameter `p'.");
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
		printf("Parameter `rule' must be in range [0,255].");
		return 1;
	    }
	    printf("Invalid value for parameter `rule'.");
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
	    printf("Invalid value for parameter `width'.");
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
	    printf("Invalid value for parameter `height'.");
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
	    printf("Invalid value for parameter `seed'.");
	    return 1;
	}
    }
    printf("Invalid arguments to `set'.");
    return 1;
}

int sim_run (sim_context* context, sim_table* table, char* args)
{
    sim_desc d;
    d.p = context->p;
    d.rule = context->rule;
    d.width = context->width;

    // TODO: Allow for other initial values.

    char initial[d.width];
    d.initial = initial;
    d.initial[d.width/2] = 1;

    table_add(run(&d,context->seed),table);

    return 0;
}
