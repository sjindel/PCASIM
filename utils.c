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

void write_simulation (simulation* s, FILE* f)
{
    for (int i = 0; i < s->desc.height; i++)
    {
	for (int j = 0; j < s->desc.width; j++)
	    fprintf(f,"%d",s->trace[i*s->desc.width + j]);
	fprintf(f,"\n");
    }
}
