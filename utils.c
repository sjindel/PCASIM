#include "utils.h"

void print_simulation (simulation* s)
{
    for (int i = 0; i < s->desc.height; i++)
    {
	for (int j = 0; j < s->desc.width; j++)
	    printf("%d",s->trace[i*s->desc.width + j]);
	printf("\n");
    }
}
