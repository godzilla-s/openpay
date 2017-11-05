#include <string.h>
#include "datapool.h"

datapool_t *
datapool_init(int bucketSize, int bucketNum)
{
    datapool_t *datapool = malloc(sizeof(datapool_t));
    datapool->mem = calloc(1, bucketSize * bucketNum);

    return datapool;
}

void 
datapool_free(datapool_t **datapool)
{
    free((*datapool)->mem);
    free(*datapool);
    *datapool = NULL;
}

void 
datapool_clean(datapool_t *datapool)
{
    memset(datapool->mem, 0, datapool->bucketNum * datapool->bucketSize);
}

static int 
datapool_set_data(datapool_t *datapool, int index, void *data, int size)
{
    if (index > datapool->bucketNum)
        return -1;
    void *buffer = datapool->mem + (index-1) * datapool->bucketSize;
    memcpy(buffer, data, size);
    return 0;
}

static int 
datapool_get_data(datapool_t *datapool, int index, void *data, int size)
{
    if (index > datapool->bucketNum)
        return -1;

    void *buffer = datapool->mem + (index-1) * datapool->bucketSize;
    memcpy(data, buffer, size);
    return 0;
}

int 
datapool_set_string(datapool_t *datapool, int index, char *data)
{
    return datapool_set_data(datapool, index, data, strlen(data));
}

int 
datapool_set_long(datapool_t *datapool, int index, long data)
{
    return datapool_set_data(datapool, index, data, sizeof(long));
}

int 
datapool_set_double(datapool_t *datapool, int index, double data)
{
    return datapool_set_data(datapool, index, &data, sizeof(double));
}

int 
datapool_get_string(datapool_t *datapool, int index, char *data, int size)
{
    return datapool_get_data(datapool, index, (void *)data, size);
}

int 
datapool_get_long(datapool_t *datapool, int index, long *data)
{
    return datapool_get_data(datapool, index, (void *)data, sizeof(long));
}

int 
datapool_get_double(datapool_t *datapool, int index, double *data)
{
    return datapool_get_data(datapool, index, data, sizeof(double));
}


