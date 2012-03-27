#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "internals.h"
#include "utils.h"
#include "pgmlib.h"
#include "malloc.h"
#include "assert.h"
#include "diff.h"

int sim_set (sim_context* context, sim_table* table, char* args);

int sim_run (sim_context* context, sim_table* table);

int sim_diff_hamm (sim_context* context, sim_table* table, char* args);

int sim_diff_tally (sim_context* context, sim_table* table, char* args);

int sim_read (sim_context* context, sim_table* table);

int sim_disp (sim_context* context, sim_table* table, char* args);

int sim_print (sim_context* context, sim_table* table);

int sim_write (sim_table* table);

int sim_show (sim_context* context);

int sim_stat (sim_table* table);

#endif
