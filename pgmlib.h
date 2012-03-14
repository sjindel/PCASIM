/* Header file for pgmlib.c. */

#ifndef __PGMLIB_H
#define __PGMLIB_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct pgm {
    int w;
    int h;
    int max;
    int* pData;
} pgm;

void LoadPGM( char* filename, pgm* pPgm );

void WritePGM( char* filename, pgm* pPgm );

#endif
