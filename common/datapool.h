#ifndef _DATA_POOL_H
#define _DATA_POOL_H

typedef data_element_s struct {
    
} data_element_t;

typedef datapool_s struct {
    int     bucketNum;
    int     bucketSize;
    void    *mem;
} datapool_t;

#endif