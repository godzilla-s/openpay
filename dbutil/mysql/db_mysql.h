#ifndef _DB_MYSQL_H
#define _DB_MYSQL_H

#include "mysql.h"

typedef struct dbutil_s {
    MYSQL   mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
} dbutil_t;

#endif
