#ifndef _DATA_POOL_H
#define _DATA_POOL_H

typedef struct bucket_s {
    int   type;
    void  *data;
} bucket_t;

typedef struct datapool_s{
    int     bucketNum;
    int     bucketSize;
    void    *mem;
} datapool_t;

// 初始化数据池 
datapool_t *
datapool_init(int bucketSize, int bucketNum);

// 数据池销毁
void 
datapool_free(datapool_t **datapool);

// 清空数据池 
void 
datapool_clean(datapool_t *datapool);

int 
datapool_set_string(datapool_t *datapool, int index, char *data);

int 
datapool_set_long(datapool_t *datapool, int index, long data);

int 
datapool_set_double(datapool_t *datapool, int index, double data);

int 
datapool_get_string(datapool_t *datapool, int index, char *data, int size);

int 
datapool_get_long(datapool_t *datapool, int index, long *data);

int 
datapool_get_double(datapool_t *datapool, int index, double *data);


#endif