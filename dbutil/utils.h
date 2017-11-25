#ifndef _UTIL_H
#define _UTIL_H

typedef struct db_mem_s
{
    void    *mem;
    int     used;
    int     size;
} db_mem_t;

#endif
