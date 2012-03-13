#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "sim.h"

void print_simulation (simulation* s);

void write_simulation_plain (simulation* s, FILE* f);

void write_simulation_csv (simulation* s, FILE* f);

#endif
