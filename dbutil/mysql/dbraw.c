// mysql 数据库操作
#include <stdio.h>
#include <string.h>
#include "dbraw.h"
//#include "log.h"

db_instance_t * 
db_va_instance_new(char *host, int port, char *dbname, char *dbuser, char *dbpasswd)
{
    int ret;
    db_instance_t *db = malloc(sizeof(db_instance_t));
printf("begin init\n");
    if (!mysql_init(&db->mysql))
    {
        //log(ll_error, "fail to init mysql:%s", get_db_error());
        printf("fail to init mysql");
        return NULL;
    }

    // 设置编码方式
    // mysql_options(db->mysql, MYSQL_SET_CHARSET_NAME, "utf8");
printf("begin connect\n");
    // 连接数据库 
    ret =  mysql_real_connect(&db->mysql, host, dbuser, dbpasswd, dbname, port, NULL, 0);
    if (!ret)
    {
        printf("Fail to connect mysql:%s\n", mysql_error(&db->mysql));
        return NULL;
    }

    // 设置字符集为utf-8
    // mysql_set_charset_name(db->mysql, "utf8");
    return db;
}

void 
db_va_instance_close(db_instance_t *db)
{
    mysql_close(&db->mysql);
    free(db);
}

/* 获取查询结果集 */
int 
db_va_open_select(db_instance_t *db, db_select_t *select, char *sql, ...)
{
    void        *ptr; 
    int         type;
    int         len;
    int         ret, i = 0; 
    va_list     vargs;
    int         paramCount = 0;
    MYSQL_BIND  *bindInParam = NULL;

    va_start(vargs, sql);

    if (db == NULL || select == NULL)
        return -1;

printf("stmt init begin\n");
    select->stmt = mysql_stmt_init(&db->mysql);
    if(select->stmt == NULL)
    {
        return -1;
    }

    printf("stmt prepare begin\n");
    ret = mysql_stmt_prepare(select->stmt, sql, strlen(sql));
    if (ret)
    {
        return -1;
    }

    paramCount = mysql_stmt_param_count(select->stmt);
    if(paramCount > 0)
        bindInParam = calloc(paramCount, sizeof(MYSQL_BIND));
    
    /* parse in params and set bind parameters */
    while(paramCount && (ptr = va_arg(vargs, void *) != NULL))
    {
        type = va_arg(vargs, int);
        len = va_arg(vargs, int);
printf("%d, %d\n", type, len);
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
    printf("mysql_stmt_bind_param begin\n");
    /* bind stmt parameters */
    ret = mysql_stmt_bind_param(select->stmt, bindInParam);
    if(ret)
    {
        return -1;
    }

    printf("mysql_stmt_execute\n");
    /* execute stmt */
    ret = mysql_stmt_execute(select->stmt);
    if(ret)
    {
        printf("fail to stmt execute:%s\n", mysql_stmt_error(select->stmt));
        return -1;
    }

    printf("mysql_stmt_result_metadata begin\n");
    select->result = mysql_stmt_result_metadata(select->stmt);
    if(select->result == NULL)
    {
        return -1;
    }

    printf("mysql_fetch_fields begin\n");
    select->fields = mysql_fetch_fields(select->result);
    if(select->fields == NULL)
    {
        return -1;
    }

    printf("mysql_num_fields begin\n");
    int colCount = mysql_num_fields(select->result);
    if(colCount > 0)
        select->bindParam = calloc(colCount, sizeof(MYSQL_BIND));
    if(select->bindParam == NULL)
    {
        return -1;
    }

    return 0;
}

/* 从select中获取数据 */ 
int 
db_va_select_fetch(db_select_t *select, ...)
{
    int             ret, i; 
    int             colCount = 0;
    int             type, len;
    void            *ptr;
    va_list         vargs;
    MYSQL_BIND      *bindOutParam =select->bindParam;

    va_start(vargs, select);
        
    if(bindOutParam == NULL)
    {
        return -1;
    }

    colCount = mysql_num_fields(select->result);
    my_bool  *is_null = calloc(colCount, sizeof(my_bool));
    my_bool  *error = calloc(colCount, sizeof(my_bool));
    unsigned long *length = calloc(colCount, sizeof(unsigned long));

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
        printf("fail to bind result:%s\n", mysql_stmt_error(select->stmt));
        return -1;
    }

    if (mysql_stmt_store_result(select->stmt))
    {
        return -1;
    }

    if(!mysql_stmt_fetch(select->stmt))
    {
        for(i=0; i<colCount; i++)
        {
            if(bindOutParam[i].buffer_type == MYSQL_TYPE_LONG)
            {
                if(is_null[i])
                    printf("NULL\t");
                else
                    printf("%d\t", *(int *)bindOutParam[i].buffer);
            }

            if(bindOutParam[i].buffer_type == MYSQL_TYPE_STRING)
            {
                if(is_null[i])
                    printf("NULL\t");
                else
                    printf("%s\t", (char *)bindOutParam[i].buffer);
            }
        }
        printf("\n");
    }
    else
    {
        printf("stmt fetch: %s\n", mysql_stmt_error(select->stmt));
    }

    free(bindOutParam);

    return 0;
}

/* 释放select */
void  
db_va_select_close(db_select_t *select)
{
    free(select->bindParam);
    mysql_free_result(select->result);
    mysql_stmt_close(select->stmt);
}

/* 执行insert,Update操作 */
int
db_va_execute(db_instance_t *db, char *sql, ...)
{
    void        *ptr;
    int         type;
    int         len;
    va_list     vargs;


    return 0;
}