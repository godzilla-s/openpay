// 单向链表
#ifndef _LIST_H
#define _LIST_H

// 单向链表
typedef list_node_s struct 
{
    int  type;
    void *data;
    list_node_t *next;
    list_node_t *prev;
} list_node_t;

typedef list_s struct
{
    list_node_t *head;
    list_node_t *tail;
    int  size;
    mem_config_t *config;
} list_t;

#endif