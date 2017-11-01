#include "datapool.h"

datapool_t *
datapool_init(int dataSize, int elementSize)
{
    datapool_t *dpool = malloc(sizeof(datapool_t));
    dpool->size = dataSize;
}

static int 
datapool_set_data(datapool_t *datapool, int index, void *data, size_t len)
{
    memcpy((void *)datapool->mempool + index * datapool->elementSize, data, len);
}

static int 
datapool_get_data(datapool_t *datapool, int index, void *data, size_t len)
{
    memcpy(data, (void *)datapool->mempool + index , len);
}

int 
datapool_set_string(datapool_t *datapool, int index, char *data)
{
    return datapool_set_data(datapool, index, data, data, len(data));
}

int 
datapool_set_long(datapool_t *datapool, int index, long data)
{
    return datapool_set_data(datapool, index, data, sizeof(long));
}

int 
datapool_set_double(datapool_t *datapool, int index, double data)
{
    return datapool_set_data(datapool, index, data, sizeof(double));
}

int 
datapool_get_string(datapool_t *datapool, int index, char *data, size_t size)
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


