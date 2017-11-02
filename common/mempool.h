#ifndef _MEMPOOL_H
#define _MEMPOOL_H

typedef struct mem_config_s
{
    void (*alloc)(int size);
    void (*free)(void *alloc);
    void (*calloc)(int block, int size);
} mem_config_t;

typedef struct mem_node_s 
{

} mem_node_t;

typedef struct mempool_s
{

} mempool_t;

#endif
