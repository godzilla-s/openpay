// 栈的使用 
#ifndef _STACK_H
#define _STACK_H

typedef struct stack_node_s{
    void *data;
    struct stack_node_s *next;
} stack_node_t;

typedef struct stack_s{
    stack_node_t *head;
} stack_t;

#endif