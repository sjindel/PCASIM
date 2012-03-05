#include "sim.h"

simulation* run (sim_desc* x, int seed)
{
    // Some useful variables

    int w = x->width;
    int h = x->height;

    // Initialize simulation.

    simulation* s = xmalloc(sizeof(struct simulation));

    memcpy(&(s->desc),x,sizeof(sim_desc));

    s->desc.initial = xmalloc(w*sizeof(char));

    memcpy(s->desc.initial,x->initial,w*sizeof(char));

    s->mt_seed = seed;

    // Initialize trace

    char* trace = xcalloc(x->width*x->height, sizeof(char));

    s->trace = trace;

    // Copy initial conditions into first line of trace
    memcpy(trace,x->initial,w*sizeof(char));

    // Initialize MT.

    dsfmt_t dsfmt;
    dsfmt_init_gen_rand(&dsfmt,time(0));

    // Run the simulation

    for (int i = 1; i < h; i++)
    {
	// Declare temporary array.
	char tmp[w];

	// Update new array.

	for (int j = 0; j < w; j++)
	{
	    char new_state;
	    double z = dsfmt_genrand_open_open(&dsfmt);
	    if (z < (x->p))
	    {
		int q;
		if (j - 1 < 0)
		    q = j+w-1;
		else
		    q = j-1;

		char configuration =
		    (trace[(i-1)*w + q]  << 2)
		    + (trace[(i-1)*w*sizeof(char) + j]  << 1)
		    + (trace[(i-1)*w*sizeof(char) + ((j+1) % w)]  << 0);

		new_state = (x->rule >> configuration) & 1;
	    }
	    else
		new_state = trace[(i-1)*w + j];

	    tmp[j] = new_state;
	}

	// Copy new array into appropriate line of old array.
	memcpy(trace+i*w,tmp,w*sizeof(char));
    }

    return s;

}
