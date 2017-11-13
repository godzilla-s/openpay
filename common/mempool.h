#ifndef _MEMPOOL_H
#define _MEMPOOL_H

#include "common.h"

typedef struct mem_config_s
{
    void* (*malloc)(uint32 size);
    void (*free)(void *ptr);
    void* (*calloc)(uint32 count, uint32 size);
    void* (*realloc)(void *ptr, uint32 size);
} mem_config_t;

typedef struct mempool_s
{
    void    *allocs;
    int     size;
    int     used; 
} mempool_t;

#endif
