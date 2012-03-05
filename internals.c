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

    sim_node* new_node = malloc(sizeof(struct sim_node));

    if (new_node == NULL)
    {
	printf("Could not allocate memory.\n");
	abort();
    }

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
    return;
}
