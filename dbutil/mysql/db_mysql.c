// mysql 数据库操作
#include <stdio.h>
#include "mysql.h"
#include "log.h"

MYSQL   *connector = NULL;

char *
get_db_error()
{
    return  mysql_error(connector)
}

int 
db_mysql_open(char *addr, char *dbname, char *dbuser, char *dbpasswd)
{
    int ret;

    if (!mysql_init(connector))
    {
        log(ll_error, "fail to init mysql:%s", get_db_error());
    }

    // 设置编码方式
    mysql_options(connector, MYSQL_SET_CHARSET_NAME, "utf8");

    // 连接数据库 
    ret =  mysql_real_connect(connector, "localhost", dbuser, dbpasswd, dbname, 3306, NULL, 0);
    if (ret != 0)
    {
        log(ll_error, "Fail to connect mysql:%s", get_db_error());
        return -1;
    }

    // 设置字符集为utf-8
    mysql_set_charset_name(connector, "utf8")
    return 0;
}

void 
db_mysql_close()
{
    mysql_close(connector);
    connector = NULL;
}

void
db_mysql_check()
{
    db_mysql_select("select ")
}
// 查询操作 
int 
db_mysql_query(const char *sql, ...)
{
    int  ret;
    ret = mysql_query(connector, sql);
    if (ret != 0)
    {
        log(ll_error, "fail to query mysql:%s", mysql_error(&connector));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(&connector);
    if(result == NULL)
    {
        log(ll_error, "fail to store result:%s", mysql_error(&connector));
        return -1;
    }

    int numFields = 0;
    numFields = mysql_num_fields(result);

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {
        for(i = 0; i<numFields; i++)
        {
            ;
        }
    }

    mysql_free_result(result)

    return 0;
}

int 
db_mysql_insert(const char *sql, ...)
{

}