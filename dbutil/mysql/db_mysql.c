// mysql 数据库操作
#include <stdio.h>
#include "mysql.h"
#include "log.h"

MYSQL   connector;

int 
db_mysql_open(char *db_service, char *db_user, char db_passwd)
{
    int ret;

    mysql_init(&connector);

    // 设置编码方式
    mysql_options(&connector, MYSQL_SET_CHARSET_NAME, "utf8");

    // 连接数据库 
    ret =  mysql_real_connect(&connector, "localhost", "root", "root", "mfctest", 3306, NULL, 0);
    if (ret != 0)
    {
        log(ll_error, "Fail to connect mysql - %s", mysql_error(&dbutil->mysql));
        return -1;
    }

    // 设置字符集为utf-8
    mysql_set_charset_name(&connector, "utf8")
    return 0;
}

void 
db_mysql_close()
{
    mysql_close(&connector)
}

// 查询操作 
int 
db_mysql_select(const char *sql, ...)
{
    int  ret;
    ret = mysql_real_query(&connector, sql, strlen(sql));
    if (ret != 0)
    {
        log(ll_error, "fail to query mysql - %s", mysql_error(&connector));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(&connector);
    if(result == NULL)
    {
        log(ll_error, "fail to store result - %s", mysql_error(&connector));
        return -1;
    }

    return 0;
}

int
db_result_fetch(MYSQL_RES *result)
{
    int fieldNum = 0;

    fieldNum = mysql_num_fields(result);

    return 0;
}

int 
db_mysql_insert(const char *sql, ...)
{

}