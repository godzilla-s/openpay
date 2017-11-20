// mysql 数据库操作
#include <stdio.h>
#include "dbraw.h"
#include "log.h"

db_instance_t * 
db_handle_new(char *host, int port, char *dbname, char *dbuser, char *dbpasswd)
{
    int ret;
    db_instance_t *db = malloc(sizeof(db_instance_t));

    if (!mysql_init(db->mysql))
    {
        log(ll_error, "fail to init mysql:%s", get_db_error());
    }

    // 设置编码方式
    mysql_options(db->mysql, MYSQL_SET_CHARSET_NAME, "utf8");

    // 连接数据库 
    ret =  mysql_real_connect(db->mysql, host, dbuser, dbpasswd, dbname, port, NULL, 0);
    if (ret != 0)
    {
        log(ll_error, "Fail to connect mysql:%s", get_db_error());
        return -1;
    }

    // 设置字符集为utf-8
    mysql_set_charset_name(db->mysql, "utf8")
    return 0;
}

void 
db_instance_close(db_instance_t *db)
{
    mysql_close(mysql);
    db->mysql = NULL;
    free(db);
}

// 查询操作 
static int 
db_raw_query(const char *sql)
{
    int  ret;
    ret = mysql_real_query(mysql, sql, strlen(sql));
    if (ret != 0)
    {
        log(ll_error, "fail to query mysql:%s", mysql_error(mysql));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(mysql);
    if(result == NULL)
    {
        log(ll_error, "fail to store result:%s", mysql_error(mysql));
        return -1;
    }

    return 0;
}

int 
db_query(const char *sql, ...)
{
    va_arg  *list;
}