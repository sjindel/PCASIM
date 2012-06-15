#include "xalloc.h"

void *xmalloc(size_t size)
{
    void *p = malloc(size);
    if (p == NULL) {
        printf("Could not allocate memory.\n");
        abort();
    }
    return p;
}

void *xcalloc(size_t n, size_t size)
{
    void *p = calloc(n, size);
    if (p == NULL) {
        printf("Could not allocate memory.\n");
        abort();
    }
    return p;
}
