#include "internals.h"

int sim_set (sim_context* context, sim_table* table, char* args);

int sim_run (sim_context* context, sim_table* table, char* args);

int sim_diff (sim_context* context, sim_table* table, char* args);

int sim_read (sim_context* context, sim_table* table);

int sim_write (sim_context* context, sim_table* table);
