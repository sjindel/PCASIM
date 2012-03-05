#include "pcasim.h"

int main(int argc, char** argv)
{
    // Initialize random number generator.

    dsfmt_t dsfmt;
    dsfmt_init_gen_rand(&dsfmt,time(0));

    // Initialize the PCA.

    pca1d pca;
    pca.width = 100;
    pca.height = 100;
    pca.p = 1;
    pca.rule = 110;
    pca.trace = calloc(100*100,sizeof(char));

    if (pca.trace == NULL)
    {
	printf("Could not allocate memory.");
	abort();
    }

    pca.pos = 1;
    pca.trace[pca.width - 1]=1;

    printf("Computing initial trace...\n");

    run(&pca,&dsfmt);

    // Main loop.

    // Clean up.

    free(pca.trace);

    return 0;
}
