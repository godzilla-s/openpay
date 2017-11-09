#ifndef _DB_MYSQL_H
#define _DB_MYSQL_H

#include "mysql.h"

typedef struct dbutil_mysql_s {
    MYSQL   mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
} dbutil_mysql_t;

#endif
