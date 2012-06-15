#include "sim.h"
#include "utils.h"

char norm(char x)
{
    if (x > 1)
        return x - 2;
    return x;
}

simulation *run(sim_desc * x, int seed)
{
    // Some useful variables

    int w = x->width;
    int h = x->height;

    // Initialize simulation.

    simulation *s = xmalloc(sizeof(struct simulation));

    memcpy(&(s->desc), x, sizeof(sim_desc));

    s->desc.initial = xmalloc(w * sizeof(char));

    memcpy(s->desc.initial, x->initial, w * sizeof(char));

    s->mt_seed = seed;

    // Initialize trace

    char *trace = xcalloc(x->width * x->height, sizeof(char));

    s->trace = trace;

    // Copy initial conditions into first line of trace
    memcpy(trace, x->initial, w * sizeof(char));

    // Initialize MT.

    dsfmt_t dsfmt;
    dsfmt_init_gen_rand(&dsfmt, seed);

    // Run the simulation

    for (int i = 1; i < h; i++) {

        // Declare temporary array.

        char tmp[w];

        // Update new array.

        for (int j = 0; j < w; j++) {
            char new_state;
            double z = dsfmt_genrand_open_open(&dsfmt);
            int q;
            if (j - 1 < 0)
                q = j + w - 1;
            else
                q = j - 1;

            char configuration = (norm(trace[(i - 1) * w + q]) << 2)
                + (norm(trace[(i - 1) * w * sizeof(char) + j]) << 1)
                +
                (norm(trace[(i - 1) * w * sizeof(char) + ((j + 1) % w)]) << 0);

            new_state = (x->rule >> configuration) & 1;
            if (z >= (x->p)) {
                new_state = norm(trace[(i - 1) * w + j]) +
                    2 * abs(new_state - norm(trace[(i - 1) * w + j]));
            }

            tmp[j] = new_state;
        }

        // Copy new array into appropriate line of old array.
        memcpy(trace + i * w, tmp, w * sizeof(char));
    }

    return s;

}

void simulation_free(simulation * s)
{
    free(s->desc.initial);
    free(s->trace);
    free(s);
}
