#include "internals.h"

int hash (simulation* s)
{
    int desc_hash =
	s->desc.p *
	s->desc.rule *
	s->desc.width *
	s->desc.height *
	s->desc.width;

    return desc_hash * s->mt_seed;
}
