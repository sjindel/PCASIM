#include "pcasim.h"

char* eat_whitespace_front(char* x)
{
    while ((*x == ' ') || (*x == '\t') || (*x == '\n')) x++;
    return x;
}

void eat_whitespace_back(char* x)
{
    // Remove trailing whitespace.
    char* end = x + strlen(x);
    end--;
    while ((*end == ' ') || (*end == '\t') || (*end == '\n')) end--;
    *(end+1) = '\0';
}

// Parse and interpret the line.

int interpret(char* line, sim_context* context, sim_table* table)
{
    char* i = line;

    // First eat leading/trailing whitespace

    while ((*i == ' ') || (*i == '\t') || (*i == '\n')) i++;

    // Next check for loop definition

    if (*i == '(')
    {
        // Process loop.

        // First find the end paren and set it to null.

        char* rest = ++i;

        while (*rest != ')')
        {
            if (*rest == '\0')
            {
                printf("Malformed loop syntax.\n");
                return 1;
            }
            rest++;
        }

        *(rest++) = '\0';

        char* loop = i;

        /*
          Now, `loop' (and `i') point(s) to a NULL-terminated string containing the
          loop definition (without parens), and `rest' points to the rest of
          the stuff that needs to be evaluated.

          We now parse the pair "key:start_value,end_value,interval" in the loop definition
          (seperated by semicolons) and perform the appropriate loop.
        */

        char* key = loop;

        while(*loop != ':')
        {
            if (*loop == '\0')
            {
                printf("Malformed loop syntax.\n");
                return 1;
            }
            loop++;
        }

        *(loop++) = '\0';

	loop = eat_whitespace_front(loop);

        char* start_value = loop;

        while(*loop != ',')
        {
            if (*loop == '\0')
            {
                printf("Malformed loop syntax.\n");
                return 1;
            }
            loop++;
        }

        *(loop++) = '\0';

	loop = eat_whitespace_front(loop);

        char* end_value = loop;

        while(*loop != ',')
        {
            if (*loop == '\0')
            {
                printf("Malformed loop syntax.\n");
                return 1;
            }
            loop++;
        }

        *(loop++) = '\0';

	loop = eat_whitespace_front(loop);

        char* interval = loop;

	// Remove trailing whitespace
	eat_whitespace_back(key);
	eat_whitespace_back(start_value);
	eat_whitespace_back(end_value);
	eat_whitespace_back(interval);

	/*
	// Print out some debugging info.

	printf("key is '%s'\n",key);
	printf("start_value is '%s'\n",start_value);
	printf("end_value is '%s'\n",end_value);
	printf("interval is '%s'\n",interval);
	*/

        // Determine which variable is being iterated over.
	if (strcmp(key,"p") == 0)
	{
	    // Back up the old variable to restore after loop.
	    double old_p = context->p;

	    double p_start_value = 0;
	    double p_end_value = 0;
	    double p_interval = 0;

	    if (sscanf(start_value,"%lf",&p_start_value) != 1)
	    {
		printf("Invalid start value for variable `p'.");
		return 1;
	    }
	    if (sscanf(end_value,"%lf",&p_end_value) != 1)
	    {
		printf("Invalid end value for variable `p'.");
		return 1;
	    }
	    if (sscanf(interval,"%lf",&p_interval) != 1)
	    {
		printf("Invalid interval for variable `p'.");
		return 1;
	    }


	    // Everything is OK so far, time to begin loop.

	    for(double p = p_start_value; p < p_end_value; p+=p_interval)
	    {
		context->p = p;
		if(interpret(rest,context,table)) break;
	    }

	    // Restore old value of p
	    context->p = old_p;


	    // Done.
	    return 0;

	}
	else if (strcmp(key,"rule") == 0)
	{
	    // Back up the old variable to restore after loop.
	    char old_rule = context->rule;

	    char rule_start_value = 0;
	    char rule_end_value = 0;
	    char rule_interval = 0;

	    if (sscanf(start_value,"%d",&rule_start_value) != 1)
	    {
		printf("Invalid start value for variable `rule'.");
		return 1;
	    }
	    if (sscanf(end_value,"%d",&rule_end_value) != 1)
	    {
		printf("Invalid end value for variable `rule'.");
		return 1;
	    }
	    if (sscanf(interval,"%d",&rule_interval) != 1)
	    {
		printf("Invalid interval for variable `rule'.");
		return 1;
	    }


	    // Everything is OK so far, time to begin loop.

	    for(char rule = rule_start_value; rule < rule_end_value; rule+=rule_interval)
	    {
		context->rule = rule;
		if(interpret(rest,context,table)) break;
	    }

	    // Restore old value of rule
	    context->rule = old_rule;


	    // Done.
	    return 0;
	}
	else if (strcmp(key,"width") == 0)
	{
	    // Back up the old variable to restore after loop.
	    int old_width = context->width;

	    int width_start_value = 0;
	    int width_end_value = 0;
	    int width_interval = 0;

	    if (sscanf(start_value,"%d",&width_start_value) != 1)
	    {
		printf("Invalid start value for variable `width'.");
		return 1;
	    }
	    if (sscanf(end_value,"%d",&width_end_value) != 1)
	    {
		printf("Invalid end value for variable `width'.");
		return 1;
	    }
	    if (sscanf(interval,"%d",&width_interval) != 1)
	    {
		printf("Invalid interval for variable `width'.");
		return 1;
	    }


	    // Everything is OK so far, time to begin loop.

	    for(int width = width_start_value; width < width_end_value; width+=width_interval)
	    {
		context->width = width;
		if(interpret(rest,context,table)) break;
	    }

	    // Restore old value of width
	    context->width = old_width;


	    // Done.
	    return 0;

	}
	else if (strcmp(key,"height") == 0)
	{
	    // Back up the old variable to restore after loop.
	    int old_height = context->height;

	    int height_start_value = 0;
	    int height_end_value = 0;
	    int height_interval = 0;

	    if (sscanf(start_value,"%d",&height_start_value) != 1)
	    {
		printf("Invalid start value for variable `height'.");
		return 1;
	    }
	    if (sscanf(end_value,"%d",&height_end_value) != 1)
	    {
		printf("Invalid end value for variable `height'.");
		return 1;
	    }
	    if (sscanf(interval,"%d",&height_interval) != 1)
	    {
		printf("Invalid interval for variable `height'.");
		return 1;
	    }


	    // Everything is OK so far, time to begin loop.

	    for(int height = height_start_value; height < height_end_value; height+=height_interval)
	    {
		context->height = height;
		if(interpret(rest,context,table)) break;
	    }

	    // Restore old value of height
	    context->height = old_height;


	    // Done.
	    return 0;

	}
	else if (strcmp(key,"seed") == 0)
	{
	    // Back up the old variable to restore after loop.
	    int old_seed = context->seed;

	    int seed_start_value = 0;
	    int seed_end_value = 0;
	    int seed_interval = 0;

	    if (sscanf(start_value,"%d",&seed_start_value) != 1)
	    {
		printf("Invalid start value for variable `seed'.");
		return 1;
	    }
	    if (sscanf(end_value,"%d",&seed_end_value) != 1)
	    {
		printf("Invalid end value for variable `seed'.");
		return 1;
	    }
	    if (sscanf(interval,"%d",&seed_interval) != 1)
	    {
		printf("Invalid interval for variable `seed'.");
		return 1;
	    }


	    // Everything is OK so far, time to begin loop.

	    for(int seed = seed_start_value; seed < seed_end_value; seed+=seed_interval)
	    {
		context->seed = seed;
		if(interpret(rest,context,table)) break;
	    }

	    // Restore old value of seed
	    context->seed = old_seed;


	    // Done.
	    return 0;
	}
	else
	{
	    printf("Invalid environment variable to iterate over.\n");
	    return 1;
	}

    }
    else
    {
	char* command = i;

	// Read until whitespace found.
	while ((*i != ' ') && (*i != '\t') && (*i != '\n') && (*i != '\0')) i++;

	*(i++) = '\0';

	char* args = i;

	if (strcmp(command,"set") == 0)
	    return sim_set(context,table,args);
	else if (strcmp(command,"run") == 0)
	    return sim_run(context,table);
	else if (strcmp(command,"write") == 0)
	    return sim_write(table);
	else if (strcmp(command,"disp") == 0)
	    return sim_disp(context,table,args);
	else if (strcmp(command,"print") == 0)
	    return sim_print(context,table);
	else if (strcmp(command,"show") == 0)
	    return sim_show(context);
	else if (strcmp(command,"stat") == 0)
	    return sim_stat(table);
	else if (strcmp(command,"quit") == 0)
	    return -1;
	else if (strcmp(command,"exit") == 0)
	    return -1;
	else
	{
	    printf("Invalid command.\n");
	    return 1;
	}
    }

    printf("This code should never be reached.\n");
    return 1;
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
    context->initial[99] = 1;
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

        if (interpret(line,context,table) == -1) break;

        free(line);
    }

    // Clean up.
    table_free(table);
    free(context->initial);
    free(context);

    return 0;
}
