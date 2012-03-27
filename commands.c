#include "commands.h"
#define FILE_NAME_BUF_SIZE 128
#define TOTAL_BUF_SIZE 256
#define BYTES_PER_PIXEL 8
#define BASE_NUM 0x30

// Set a context variable.

int sim_set (sim_context* context, sim_table* table, char* args)
{
    int l = strlen(args);
    char var[l];
    char val[l];
    char seq[l];

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
		context->initial = realloc(context->initial,
					   context->width*sizeof(char));

		if (!context->initial)
		{
		    printf("Could not allocate memory.\n");
		    return 1;
		}

		for (int i = 0; i < context->width; i++)
		    context->initial[i] = 0;

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
    if (sscanf(args,"%s %s %s",var,val,seq) == 3)
    {
	if (strcmp(var,"initial") == 0)
	{
	    int len = strlen(seq);

	    // Wipe current initial buffer.

	    for (int i = 0; i < context->width; i++)
		context->initial[i] = 0;

	    if (strcmp(val,"left") == 0)
	    {
		for (int i = 0; (i < len) && (i < context->width); i++)
		    context->initial[i] = seq[i] - BASE_NUM;
		return 0;
	    }
	    else if (strcmp(val,"right") == 0)
	    {
		int start_val = 0;

		if ((context->width - len) > start_val)
		    start_val = context->width - len;

		for (int i = start_val; i < context->width; i++)
		    context->initial[i] = seq[i-start_val] - BASE_NUM;
		return 0;
	    }
	    else if (strcmp(val,"center") == 0)
	    {
		int start_val = 0;

		if ((context->width - len) > start_val)
		    start_val = (context->width - len)/2;

		for (int i = start_val; i < (context->width+len)/2; i++)
		    context->initial[i] = seq[i-start_val] - BASE_NUM;
		return 0;
	    }
	    else if (strcmp(val,"periodic") == 0)
	    {
		for (int i = 0; i < context->width; i++)
		    context->initial[i] = seq[i%len] - BASE_NUM;
		return 0;
	    }
	    printf("Invalid values for parameters for `initial'.\n");
	    return 1;
	}
    }
    printf("Invalid arguments to `set'.\n");
    return 1;
}

// Run a simulation of the current context.

int sim_run (sim_context* context, sim_table* table)
{

    sim_desc d = {context->p, context->rule, context->width, context->height,
		  context->initial};

    simulation* sim = run(&d,context->seed);

    table_add(sim,table);

    context->seed++;

    return 0;
}

int sim_disp(sim_context* context, sim_table* table, char* args)
{
    /*
      Write an image corresponding to the simulation trace
      in an appropriate output file and send it to eog.
    */

    int scale = 1;

    sscanf(args,"%d",&scale);

    sim_desc d = {context->p, context->rule, context->width, context->height,
		  context->initial};

    simulation* s = table_find(&d,1,context->seed,table);

    if (s == NULL)
    {
	printf("Context simulation has not been run.\n");
	return 1;
    }

    char buffer[FILE_NAME_BUF_SIZE];

    sprintf(buffer,"%lf.%d.%d.%d.%d.trace.pgm",
	    d.p, d.rule, d.width, d.height, s->mt_seed);

    int* image = xcalloc(d.width*scale*d.height*scale,sizeof(int));

    for (int i = 0; i < d.height*scale; i++)
	for (int j = 0; j < d.width*scale; j++)
	    image[i*d.width*scale + j] =
		(1 - s->trace[(i/scale)*d.width + (j/scale)]) * 255;

//		    printf("%d",image[(i*scale + k)*d.width + j*scale + l]/255);


    pgm p = {d.width*scale,d.height*scale,255,image};

    WritePGM(buffer,&p);

    free(image);

    // Open the file in eog.

    char buf[TOTAL_BUF_SIZE];

    sprintf(buf,"eog %s &",buffer);

    system(buf);

    return 0;
}

int sim_print(sim_context* context, sim_table* table)
{
    sim_desc d = {context->p,context->rule,context->width,context->height,
		  context->initial};

    simulation* s = table_find(&d,1,context->seed,table);

    if (s == NULL)
    {
	printf("Context simulation has not been run.\n");
	return 1;
    }

    print_simulation(s);

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

// Render all the pictures.
int sim_render(sim_table* table)
{
    for (int i = 0; i < table->h; i++)
    {
	sim_node* c = table->array[i];

	while(c != NULL)
	{
	    // Write the simulation in this node to a file.

	    sim_desc d = c->s->desc;

	    int scale = 1;
	    if (1000/d.width > scale)
		scale = 1000/d.width;

	    char buffer[FILE_NAME_BUF_SIZE];

	    sprintf(buffer,"%lf.%d.%d.%d.%d.trace.pgm",
		    d.p, d.rule, d.width, d.height, c->s->mt_seed);

	    int* image = xcalloc(d.width*scale*d.height*scale,sizeof(int));

	    for (int i = 0; i < d.height*scale; i++)
		for (int j = 0; j < d.width*scale; j++)
		    image[i*d.width*scale + j] =
			(1 - c->s->trace[(i/scale)*d.width + (j/scale)]) * 255;

	//		    printf("%d",image[(i*scale + k)*d.width + j*scale + l]/255);


	    pgm p = {d.width*scale,d.height*scale,255,image};

	    WritePGM(buffer,&p);

	    free(image);

	    c = c->next;

	}

    }

    return 0;
}

// Write a hamming difference of two simulations

int sim_diff_hamm(sim_context* context, sim_table* table, char* args)
{
    double fix_p;
    int fix_width, fix_height, fix_seed;
    char fix_rule;

    if (sscanf(args,"[ %lf , %d , %d , %d , %d ]",
	       &fix_p,&fix_rule,&fix_width,&fix_height,&fix_seed)
	!= 5)
    {
	printf("Invalid arguments to the hamming difference.\n");
	return 1;
    }

    sim_desc fix_desc = {fix_p,fix_rule,fix_width,fix_height,context->initial};

    simulation* fix;
    if (fix_seed)
	fix = table_find(&fix_desc,1,fix_seed,table);
    else
	fix = table_find(&fix_desc,0,0,table);

    if (!fix)
    {
	printf("Reference simulation not run.\n");
	return 1;
    }

    sim_desc con_desc = {context->p, context->rule, context->width,
			 context->height, context->initial};

    simulation* con;
    if (fix_seed)
	con = table_find(&con_desc,1,context->seed,table);
    else
	con = table_find(&con_desc,0,0,table);

    if (!con)
    {
	sim_run(context, table);
	con = table_find(&con_desc,0,0,table);
    }

    assert(con);
    assert(fix);

    if ((fix_desc.width != con_desc.width) || (fix_desc.height != con_desc.height))
    {
	printf("Error: can not diff simulations of different dimensions.\n");
	return 1;
    }

    int* difference = diff_hamming(con,fix);

    char buffer[FILE_NAME_BUF_SIZE];
    if (!fix_seed)
	sprintf(buffer,"%lf.%d.%d.%d.v.%lf.%d.%d.%d.csv",
		fix_desc.p, fix_desc.rule, fix_desc.width, fix_desc.height,
		con_desc.p, con_desc.rule, con_desc.width, con_desc.height);
    else
	sprintf(buffer,"%lf.%d.%d.%d.%d.v.%lf.%d.%d.%d.%d.csv",
		fix_desc.p, fix_desc.rule, fix_desc.width, fix_desc.height,
		fix_seed, con_desc.p, con_desc.rule, con_desc.width,
		con_desc.height, context->seed);

    FILE* f = fopen(buffer,"w");

    if (!f)
    {
	printf("Could not open output file for diff.\n");
	return 1;
    }

    for (int i = 0; i < fix_desc.height; i++)
	fprintf(f,"%d\n",difference[i]);

    fclose(f);

    return 0;
}

int sim_diff_tally(sim_context* context, sim_table* table, char* args)
{
    double fix_p;
    int fix_width, fix_height, fix_seed;
    char fix_rule;

    if (sscanf(args,"[ %lf , %d , %d , %d , %d ]",
	       &fix_p,&fix_rule,&fix_width,&fix_height,&fix_seed)
	!= 5)
    {
	printf("Invalid arguments to the tally difference.\n");
	return 1;
    }

    sim_desc fix_desc = {fix_p,fix_rule,fix_width,fix_height,context->initial};

    simulation* fix;
    if (fix_seed)
	fix = table_find(&fix_desc,1,fix_seed,table);
    else
	fix = table_find(&fix_desc,0,0,table);

    if (!fix)
    {
	printf("Reference simulation not run.\n");
	return 1;
    }

    sim_desc con_desc = {context->p, context->rule, context->width,
			 context->height, context->initial};

    simulation* con;
    if (fix_seed)
	con = table_find(&con_desc,1,context->seed,table);
    else
	con = table_find(&con_desc,0,0,table);

    if (!con)
    {
	sim_run(context, table);
	con = table_find(&con_desc,0,0,table);
    }

    assert(con);
    assert(fix);

    if ((fix_desc.width != con_desc.width) || (fix_desc.height != con_desc.height))
    {
	printf("Error: can not diff simulations of different dimensions.\n");
	return 1;
    }

    int* difference = diff_tally(con,fix);

    char buffer[FILE_NAME_BUF_SIZE];
    if (!fix_seed)
	sprintf(buffer,"%lf.%d.%d.%d.v.%lf.%d.%d.%d.csv",
		fix_desc.p, fix_desc.rule, fix_desc.width, fix_desc.height,
		con_desc.p, con_desc.rule, con_desc.width, con_desc.height);
    else
	sprintf(buffer,"%lf.%d.%d.%d.%d.v.%lf.%d.%d.%d.%d.csv",
		fix_desc.p, fix_desc.rule, fix_desc.width, fix_desc.height,
		fix_seed, con_desc.p, con_desc.rule, con_desc.width,
		con_desc.height, context->seed);

    FILE* f = fopen(buffer,"w");

    if (!f)
    {
	printf("Could not open output file for diff.\n");
	return 1;
    }

    for (int i = 0; i < fix_desc.height; i++)
	fprintf(f,"%d\n",difference[i]);

    fclose(f);

    return 0;
}

// Print out the current environment variables.

int sim_show (sim_context* context)
{
    printf("p: %lf\nwidth: %d\nheight: %d\nseed: %d\ninitial:\n",
	   context->p,context->width,context->height,context->seed);
    for (int i = 0; i < context->width; i++)
	printf("%d",context->initial[i]);
    printf("\n");
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
