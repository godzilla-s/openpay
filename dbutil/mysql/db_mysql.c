// mysql 数据库操作
#include <stdio.h>
#include "mysql.h"

static dbutil_t dbutil;

int 
db_open(char *db_service, char *db_user, char db_passwd)
{
    mysql_init(&dbutil->mysql);

    // 设置编码方式
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");

    // 连接数据库 
    mysql_real_connect(&mysql, "localhost", "root", "root", "mfctest", 3306, NULL, 0);
    return 0;
}

void 
db_close()
{

}