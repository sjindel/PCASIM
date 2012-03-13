#ifndef __PCASIM_H
#define __PCASIM_H

#include <stdio.h>
#include <stdlib.h>
#include "sim.h"
#include "internals.h"
#include "commands.h"
#include "readline/readline.h"
#include "readline/history.h"

int interpret(char* line, sim_context* context, sim_table* table);

int main(int argc, char** argv);

#endif
