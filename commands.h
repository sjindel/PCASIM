#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "internals.h"

int sim_set (sim_context* context, sim_table* table, char* args);

int sim_run (sim_context* context, sim_table* table);

int sim_diff (sim_context* context, sim_table* table, char* args);

int sim_read (sim_context* context, sim_table* table);

int sim_write (sim_context* context, sim_table* table);

int sim_show (sim_context* context);

int sim_stat (sim_table* table);

#endif
