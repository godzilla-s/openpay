#include "mempool.h"
#include <stdio.h>

#define CAPACITY_   sizeof(int)

mempool_t *
mempool_new(int size)
{
    mempool_t *mempool = malloc(sizeof(sizeof(mempool_t)));
    mempool->allocs = calloc(1, size);
    mempool->size = size;
    mempool->used = 0;
    return mempool;
}

void 
mempool_clean(mempool_t *pool)
{
    memset(pool->allocs, 0, pool->size);
    pool->used = 0;
}

void 
mempool_destroy(mempool_t *pool)
{
    #if __linux__
    free(pool->allocs);
    #else
    // TODO 
    #endif
    pool->allocs = NULL;
    pool=NULL;
}

void *
mempool_alloc(mempool_t *pool, int size)
{
    int rest;
    if (!pool->allocs)
    {
        return NULL;
    }
    
    rest = pool->size - pool->used;
    if (rest < size)
    {
        // 重新使用或者重新分配
    }
    void *ptr = pool->allocs + pool->used;
    memcpy(ptr, &size, sizeof(int));
    pool->used = pool->used + size + sizeof(int);
    return ptr + sizeof(int);
}

void 
mempool_free(mempool_t *pool, void *ptr)
{
    void *read = ptr + sizeof(int);
    memset(read, 0, sizeof(int)); 
}