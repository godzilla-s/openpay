#ifndef _DB_MYSQL_H
#define _DB_MYSQL_H

#include "mysql.h"

typedef struct db_instance_s {
    MYSQL   *mysql;
} db_instance_t;

typedef struct db_select_s
{
    MYSQL_RES    *result;
    MYSQL_STMT   *stmt; 
    MYSQL_FIELD  *fields;
} db_select_t;


#endif
