#ifndef _DB_MYSQL_H
#define _DB_MYSQL_H

#include "mysql.h"

typedef struct db_instance_s {
    MYSQL   mysql;
} db_instance_t;

typedef struct db_select_s
{
    MYSQL_RES    *result;
    MYSQL_STMT   *stmt; 
    MYSQL_FIELD  *fields;
    MYSQL_BIND   *bindParam;
} db_select_t;

#define  FLD_INT    1
#define  FLD_STR    2

db_instance_t * 
db_va_instance_new(char *host, int port, char *dbname, char *dbuser, char *dbpasswd);

void 
db_va_instance_close(db_instance_t *db);

#endif
