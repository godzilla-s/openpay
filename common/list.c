#include <stdio.h>
#include "list.h"
#include "mempool.h"

list_t *
list_new(int type, mem_config_t *config)
{
    list_t *list = NULL;
    if (config != NULL) {
        list->malloc = config->malloc;
        list->free = config->free;
        list->calloc = config->calloc;
    }
    list->head = list->tail = NULL;
    list->type = type;
    return list;
}

static list_node_t *
list_node_new(list_t *list, void *data, int size)
{
    list_node_t *node = list->malloc(sizeof(list_node_t));
    memcpy(node->data, data, size);
    node->prev = node->next = NULL;
    return node;
}

static list_node_t *
list_node_get(list_t *list, int index)
{
    if (index > list->size)
        return NULL;
    list_node_t *node = list->head;
    while(index>=0)
    {
        node = node->next;
        --index;
    }
    return node;
}

static int 
list_node_insert(list_t *list, list_node_t *node, int position)
{
    list_node_t *temp = list_node_get(list, position);
    if (temp == NULL)
        return -1;
    
    return 0;
}

static int 
list_node_delete(list_t *list, int position)
{
    return 0;
}

// API 
int list_push(list_t *list, void *data)
{
    return list_node_insert(list, list_node_new(list, data, 10), 0);
}

int list_pop(list_t *list, void *data)
{
    list_node_t *node = list_node_get(list, 0);
    if (node == NULL)
        return 1;
    memcpy(node->data, data, 10);
    list->free(node);
    node = NULL;
    return 0;
}

int list_insert(list_t *list, void *data, int position)
{
    return list_node_insert(list, list_node_new(list, data, 10), position);
}

int list_delete(list_t *list, int position)
{
    return list_node_delete(list, position);
}

void list_destroy(list_t **list)
{
    ;
}