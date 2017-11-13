// mysql 数据库操作
#include <stdio.h>
#include "mysql.h"
#include "log.h"

static dbutil_mysql_t dbutil;

int 
db_open(char *db_service, char *db_user, char db_passwd)
{
    int ret 
    mysql_init(&dbutil->mysql);

    // 设置编码方式
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");

    // 连接数据库 
    ret =  mysql_real_connect(&mysql, "localhost", "root", "root", "mfctest", 3306, NULL, 0);
    if (ret != 0)
    {
        log(ll_error, "Fail to connect mysql - %s", mysql_error(&dbutil->mysql));
        return -1;
    }

    // 设置字符集为utf-8
    mysql_set_charset_name(&mysql, "utf8")
    return 0;
}

void 
db_close()
{
    mysql_close(&dbutil->mysql)
}

// 查询操作 
int 
db_select(const char *sql, ...)
{
    int  ret;
    ret = mysql_real_query(&dbutil->mysql, sql, strlen(sql));
    if (ret != 0)
    {
        log(ll_error, "fail to query mysql - %s", mysql_error(&dbutil->mysql));
        return -1;
    }


}

int 
db_insert(const char *sql, ...)
{

}