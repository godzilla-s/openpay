#ifndef _DB_MYSQL_H
#define _DB_MYSQL_H

#include "mysql.h"

typedef struct db_instance_s {
    MYSQL       *mysql;
} db_instance_t;

typedef struct db_select_s
{
    MYSQL_STMT   *stmt; 
} db_select_t;

typedef struct db_mem_s
{
    void    *pool;
    int     used;
    int     size;
} db_mem_t;

#define MEM_BLOCK_SIZE 8096

#define FLD_INT     1
#define FLD_STR     2
#define FLD_FLOAT   3


#define ERR_DB_FIELD_TYPE_NOTMATCH      -10
#define ERR_DB_INVALID_FIELD_TYPE       -11

#endif
