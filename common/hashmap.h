#ifndef _HASH_MAP_H
#define _HASH_MAP_H

typedef map_node_s struct 
{
    char    key[33];
    void    *value;
} map_node_t;

typedef hashmap_s struct
{
    map_node_t *node[512];
} hashmap_t;

#endif
