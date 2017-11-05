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

typedef struct mem_node_s 
{

} mem_node_t;

typedef struct mempool_s
{

} mempool_t;

#endif
