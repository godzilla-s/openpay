#ifndef _HASH_MAP_H
#define _HASH_MAP_H

typedef struct hash_node_s 
{
    char    key[33];
    void    *value;

    struct hash_node_s *next;
} hash_node_t;

typedef struct hashmap_s
{
    hash_node_t **buckets;
    int   bucketNum; 

    void *(*malloc)(int size);
    void (*free)(void *alloc);
    void *(*calloc)(int block, int blockSize);
} hashmap_t;

#endif
