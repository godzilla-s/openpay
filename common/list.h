// 单向链表
#ifndef _LIST_H
#define _LIST_H

#include "mempool.h"

// 单向链表
typedef struct list_node_s 
{
    int  type;
    void *data;
    struct list_node_s *next;
    struct list_node_s *prev;
} list_node_t;

typedef struct list_s
{
    list_node_t *head;
    list_node_t *tail;
    int  size;
    mem_config_t *config;
} list_t;

#endif