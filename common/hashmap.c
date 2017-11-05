#include <stdlib.h>
#include "hashmap.h"
#include "mempool.h"

hashmap_t *
hashmap_new(int bucketNum, mem_config_t *config)
{
    hashmap_t *map = malloc(sizeof(hashmap_t));
    if (config != NULL)
    {
        map->malloc = config->malloc;
        map->free - config->free;
        map->calloc = config->calloc;
    }
    else 
    {
        map->malloc = malloc;
        map->free = free;
        map->calloc = calloc;
    }

    map->buckets = map->calloc(bucketNum, sizeof(hash_node_t*));
    map->bucketNum = bucketNum;

    return map;
}

void 
hashmap_destroy(hashmap_t **map)
{
    ;
}

static uint64 
hash_string(const char *str, int len)
{
    uint64 seed;

    return seed;
}

static hash_node_t * 
hash_node_new(hashmap_t *map, const char *key, void *data)
{
    hash_node_t *node = map->malloc(sizeof(hash_node_t));
    node->key = key;
    node->data = data;

    node->next = NULL;

    return node;
}

