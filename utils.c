#include "utils.h"

void print_simulation(simulation * s)
{
    for (int i = 0; i < s->desc.height; i++) {
        for (int j = 0; j < s->desc.width; j++)
            printf("%d", s->trace[i * s->desc.width + j]);
        printf("\n");
    }
}

void write_simulation_plain(simulation * s, FILE * f)
{
    for (int i = 0; i < s->desc.height; i++) {
        for (int j = 0; j < s->desc.width; j++)
            fprintf(f, "%d", s->trace[i * s->desc.width + j]);
        fprintf(f, "\n");
    }
}

void write_simulation_csv(simulation * s, FILE * f)
{
    for (int i = 0; i < s->desc.height; i++) {
        for (int j = 0; j < s->desc.width; j++) {
            if (j == s->desc.width - 1)
                fprintf(f, "%d", s->trace[i * s->desc.width + j]);
            else
                fprintf(f, "%d,", s->trace[i * s->desc.width + j]);
        }
        fprintf(f, "\n");
    }
}

char *eat_whitespace_front(char *x)
{
    while ((*x == ' ') || (*x == '\t') || (*x == '\n'))
        x++;
    return x;
}

void eat_whitespace_back(char *x)
{
    // Remove trailing whitespace.
    char *end = x + strlen(x);
    end--;
    while ((*end == ' ') || (*end == '\t') || (*end == '\n'))
        end--;
    *(end + 1) = '\0';
}
