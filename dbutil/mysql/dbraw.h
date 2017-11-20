#ifndef _DB_MYSQL_H
#define _DB_MYSQL_H

#include "mysql.h"

typedef struct db_instance_s {
    MYSQL   *mysql;
} db_instance_t;

#endif
