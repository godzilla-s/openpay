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

/* 获取查询结果集 */
int 
db_va_open_select(db_instance_t *db, db_select_t *select, char *sql, ...)
{
    void        *ptr; 
    int         type;
    int         len;
    int         ret, i; 
    va_list     vargs;
    int         paramCount = 0;
    MYSQL_BIND  *bindInParam = NULL;

    va_start(vargs, sql);

    if (db == NULL || db->mysql == NULL)
        return -1;

    if (select == NULL)
        return -1;

    select->stmt = mysql_stmt_init(mysql);
    if(select->stmt == NULL)
    {
        return -1;
    }

    ret = mysql_stmt_prepare(stmt, sql, strlen(sql)
    if (ret)
    {
        return -1;
    }

    paramCount = mysql_stmt_param_count(select->stmt);
    if(paramCount > 0)
        bindInParam = calloc(paramCount, sizeof(MYSQL_BIND));
    
    /* parse in params and set bind parameters */
    while(paramCount && (ptr = va_arg(vargs, void *) != NULL)
    {
        type = va_arg(vargs, int);
        len = va_arg(vargs, int);

        if(type == FLD_INT)
        {
            bindInParam[i].buffer_type = MYSQL_TYPE_LONG;
            bindInParam[i].buffer = (char *)ptr;
            bindInParam[i].buffer_length = sizeof(long);
        }

        if(type == FLD_STR)
        {
            bindInParam[i].buffer_type = MYSQL_TYPE_STRING;
            bindInParam[i].buffer = (char *)ptr;
            bindInParam[i].buffer_length = len;
        }

        i++;
    }

    /* bind stmt parameters */
    ret = mysql_stmt_bind_param(select->stmt, bindInParam);
    if(ret)
    {
        return -1;
    }

    ret = mysql_stmt_execute(select->stmt);
    if(ret)
    {
        return -1;
    }

    select->result = mysql_stmt_result_metadata(select->stmt);
    if(select->result == NULL)
    {
        return -1;
    }

    select->fields = mysql_fetch_fields(select->result);
    if(select->fields == NULL)
    {
        return -1;
    }

    free(bindInParam);

    return 0;
}

/* 从select中获取数据 */ 
int 
db_va_select_fetch(db_select_t *select, ...)
{
    int             ret, i; 
    int             colCount = 0;
    int             type, len;
    va_list         vargs;
    MYSQL_BIND      *bindOutParam = NULL;

    va_start(vargs, select);
    colCount = mysql_num_fields(select->result);
    if(colCount > 0)
        bindOutParam = calloc(colCount, sizeof(MYSQL_BIND));
        
    if(bindOutParam == NULL)
    {
        return -1;
    }

    while((ptr = va_arg(vargs, void *)) != NULL)
    {
        type = va_arg(vargs, int);
        len = va_arg(vargs, int);

        if (type == FLD_INT)
        {
            bindOutParam[i].buffer_type = MYSQL_TYPE_LONG;
            bindOutParam[i].buffer = (char *)ptr;
            bindOutParam[i].is_null = &is_null[i];
            bindOutParam[i].length = &length[i];
            bindOutParam[i].error = &error[i];
        }

        if(type == FLD_STR)
        {
            bindOutParam[i].buffer_type = MYSQL_TYPE_STRING;
            bindOutParam[i].buffer = (char *)ptr;
            bindOutParam[i].is_null = &is_null[i];
            bindOutParam[i].buffer_length = len;
            bindOutParam[i].length = &length[i];
            bindOutParam[i].error = &error[i];
        }
        i++;
    }

    if(mysql_stmt_bind_result(select->stmt, bindOutParam))
    {
        printf("fail to bind result:%s\n", mysql_stmt_error(stmt));
        return -1;
    }

    if (mysql_stmt_store_result(select->stmt))
    {
        return -1;
    }

    free(bindOutParam);

    return 0;
}

/* 释放select */
void  
db_va_select_close(db_select_t *select)
{
    mysql_free_result(select->result);
    mysql_stmt_close(select->stmt);
}

/* 执行insert,Update操作 */
int
db_va_execute(db_instance_t *db, char *sql, ...)
{
    return 0;
}