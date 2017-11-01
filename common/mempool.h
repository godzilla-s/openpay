#ifndef _MEMPOOL_H
#define _MEMPOOL_H

typedef mem_config_s struct
{
    void *alloc(int size);
    void free(void *alloc);
    void calloc(int block, int size);
} mem_config_t;

typedef mem_node_s struct 
{

} mem_node_t;

typedef mempool_s struct
{

} mempool_t;

#endif
