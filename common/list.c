#include "list.h"

list_t *
list_new(mem_config_t *config)
{
    list_t *list = NULL;
    if (config != nil )
        list = config->calloc(1, sizeof(list_t));
    else
        list = calloc(1, sizeof(list_t));
    if (list == NULL)
        return NULL;
    list->head = list->tail = NULL;
    list->config = config;
    return list;
}

static list_node_t *
list_node_new(list_t *list, void *data)
{

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
    }
    return node;
}

