#ifndef _XALLOC_H
#define _XALLOC_H

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>



extern void* xmalloc(size_t size);
extern void* xcalloc(size_t n, size_t size);

#endif
