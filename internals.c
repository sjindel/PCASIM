#include "internals.h"

int desc_cmp (sim_desc* x, sim_desc* y)
{
    if (x->p != y->p)
	return 0;
    if (x->rule != y->rule)
	return 0;
    if (x->width != y->width)
	return 0;
    if (x->height != y->height)
	return 0;
    if (memcmp(x->initial,y->initial,x->width))
	return 1;
    return 0;
}

int desc_hash (sim_desc* s)
{
    return
	s->p *
	s->rule *
	s->width *
	s->height *
	s->width;
}

void add (simulation* s, sim_table* t)
{
    int hash = desc_hash(&(s->desc));

    sim_node* new_node = xmalloc(sizeof(struct sim_node));

    new_node->s = s;
    new_node->next = t->array[hash % t->h];
    t->array[hash % t->h] = new_node;
    t->n++;

    balance(t);

    return;
}

simulation* find (sim_desc* d, sim_table* t)
{
    int hash = desc_hash(d);

    sim_node* c = t->array[hash % t->h];

    while (c != NULL)
    {
	if (desc_cmp(d,&(c->s->desc)))
	    return c->s;
	c = c->next;
    }

    return NULL;
}

void balance(sim_table* t)
{
    int load = t->n/t->h;

    if (load > 1)
    {
	// Time to expand and rehash the table.

	sim_node** new_array = xcalloc(2*t->h,sizeof(sim_node*));

	// Iterate through the current hash table, adding the elements
	// in it to the new table.

	for (int i = 0; i < t->h; i++)
	{
	    sim_node* c = t->array[i];
	    while(c != NULL)
	    {
		// TODO: add simulation in c to the new table.

		sim_node* new_node = xmalloc(sizeof(sim_node*));
		new_node->s = c->s;

		int hash = desc_hash(&(c->s->desc));

		new_node->next = new_array[hash % ((t->h)*2)];
		new_array[hash % ((t->h)*2)] = new_node;
	    }
	}

	// Free old table

	for (int i = 0; i < t->h; i++)
	{
	    sim_node* c = t->array[i];

	    while(c != NULL)
	    {
		sim_node* tmp = c->next;
		free(c);
		c = tmp;
	    }
	}

	// Move new table into main structure

	t->array = new_array;
	t->h = 2*t->h;
    }
}
