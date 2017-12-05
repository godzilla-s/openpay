// mysql 数据库操作
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "dbmysql_va.h"

db_mem_t  *mem = NULL;

db_mem_t *mem_new(int size)
{
    db_mem_t  *mem = malloc(sizeof(db_mem_t));
    mem->pool = calloc(size, sizeof(char));
    mem->used = 0;
    mem->size = size;

    return mem;
}

void mem_free(db_mem_t *mem)
{
    free(mem->pool);
    free(mem);
    mem = NULL;
}

void *mem_alloc(db_mem_t *mem, int size)
{
    if(mem->size - mem->used < size + sizeof(int))
    {
        return NULL;
    }

    void *ptr = mem->pool + mem->used;
    mem->used += size + sizeof(int);
    return ptr + sizeof(int);
}

void mem_clean(db_mem_t *mem)
{
    memset(mem->pool, 0, mem->used);
    mem->used = 0;
}

db_instance_t * 
db_instance_new(char *host, int port, char *dbname, char *dbuser, char *dbpasswd)
{
    int ret;

    db_instance_t *db = malloc(sizeof(db_instance_t));
    if (db == NULL)
    {
        return NULL;
    }

    db->mysql = malloc(sizeof(MYSQL));
    // 初始化数据库
    if (!mysql_init(db->mysql))
    {
        free(db);
        return NULL;
    }

    // 连接数据库 
    db->mysql = mysql_real_connect(db->mysql, host, dbuser, dbpasswd, dbname, port, NULL, 0);
    if (!db->mysql)
    {
        free(db);
        return NULL;
    }

    mem = mem_new(MEM_BLOCK_SIZE);
    if(!mem)
    {
        mysql_close(db->mysql);
        free(db);
        return NULL;
    }

    return db;
}

void 
db_instance_close(db_instance_t *db)
{
    mysql_close(db->mysql);
    free(db->mysql);
    free(db);
    mem_free(mem);
    mem = NULL;
}

void 
db_select_init(db_select_t *select)
{
    select->stmt = NULL;
}

static int
va_make_params_to_bind(va_list vargs, MYSQL_BIND *bind)
{
    int     type, len, i=0;
    void    *ptr;

    for(i=0; (ptr = va_arg(vargs, void *)) != NULL; i++)
    {
        type = va_arg(vargs, int);
        len = va_arg(vargs, int);

         switch(type)
         {
             case FLD_INT:
                bind[i].buffer_type = MYSQL_TYPE_LONG;
                bind[i].buffer = (char *)ptr;
                bind[i].buffer_length = sizeof(long);
                break;
             case FLD_STR:
                bind[i].buffer_type = MYSQL_TYPE_STRING;
                bind[i].buffer = (char *)ptr;
                bind[i].buffer_length = len;
                break;
             default:
                printf("unknown fields type\n");
                return ERR_DB_INVALID_FIELD_TYPE;
         }
    }

    return 0;
}

/* 获取查询结果集 */
static int 
db_va_open_select(db_instance_t *db, db_select_t *select, char *sql, va_list vargs)
{
    void        *ptr; 
    int         type;
    int         len;
    int         ret, i; 
    //va_list     vargs;
    int         count = 0;
    MYSQL_BIND  *params = NULL;

    //va_start(vargs, sql);

    if (!db || !select || !mem)
        return -1;

    select->stmt = mysql_stmt_init(db->mysql);
    if(!select->stmt)
    {
        return -1;
    }

    if(mysql_stmt_prepare(select->stmt, sql, strlen(sql)))
    {
        return -1;
    }

    count = mysql_stmt_param_count(select->stmt);
    if(count > 0)
        params = mem_alloc(mem,count *  sizeof(MYSQL_BIND));
    
    /* parse in params and set bind parameters */
    ret = va_make_params_to_bind(vargs, params);
    if(ret)
    {
        return ret;
    }

    /* bind stmt parameters */
    ret = mysql_stmt_bind_param(select->stmt, params);
    if(ret)
    {
        return -1;
    }

    /* 执行statement */
    ret = mysql_stmt_execute(select->stmt);
    if(ret)
    {
        return -1;
    }

    /* */
    MYSQL_RES *result = mysql_stmt_result_metadata(select->stmt);
    if(result == NULL)
    {
        return -1;
    }

    /* 获取字段 */
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    if(fields == NULL)
    {
        return -1;
    }

    MYSQL_BIND  *bind;
    count = mysql_num_fields(result);
    if(count > 0)
    {
        bind = mem_alloc(mem, count * sizeof(MYSQL_BIND));
        if(!bind)
        {
            return -1;
        }
    }
 
    for(i=0; i<count; i++)
    {
        bind[i].buffer_type = fields[i].type;
        if(fields[i].type == MYSQL_TYPE_LONG)
        {
            bind[i].buffer_type = MYSQL_TYPE_LONG;
            bind[i].buffer = mem_alloc(mem, sizeof(long));
        }
        if(fields[i].type == MYSQL_TYPE_VAR_STRING)
        {
            bind[i].buffer_type = MYSQL_TYPE_STRING;
            bind[i].buffer = mem_alloc(mem, fields[i].length);
            bind[i].buffer_length = fields[i].length;
        }

        bind[i].is_null = (my_bool *)mem_alloc(mem, sizeof(my_bool));
        bind[i].error = (my_bool *)mem_alloc(mem, sizeof(my_bool));
        bind[i].length = (unsigned long *)mem_alloc(mem, sizeof(unsigned long));
    }

    if(mysql_stmt_bind_result(select->stmt, bind))
    {
        printf("fail to bind result:%s\n", mysql_stmt_error(select->stmt));
        return -1;
    }

    if (mysql_stmt_store_result(select->stmt))
    {
        return -1;
    }

    // free(params);
    return 0;
}

/* 从select中获取数据 */ 
static int 
db_va_select_fetch(db_select_t *select, va_list vargs)
{
    int             ret, i = 0; 
    int             colCount = 0;
    int             type, len;
    void            *ptr = NULL;
    //va_list         vargs;
    
    MYSQL_BIND      *out = select->stmt->bind;

    //va_start(vargs, select);

    if(!mysql_stmt_fetch(select->stmt))
    {
        while((ptr = va_arg(vargs, void *)) != NULL)
        {
            type = va_arg(vargs, int);
            len = va_arg(vargs, int);
            
            switch(type)
            {
                case FLD_INT:
                    //printf("-- %d\n", *(int *)out[i].buffer);
                    if(out[i].buffer_type != MYSQL_TYPE_LONG)
                    {
                        return ERR_DB_FIELD_TYPE_NOTMATCH;
                    } 
                    memcpy(ptr, out[i].buffer, sizeof(long));
                    //printf("** %d\n", *(int *)ptr);
                    break;
                case FLD_STR:
                    if(out[i].buffer_type != MYSQL_TYPE_STRING)
                        return ERR_DB_FIELD_TYPE_NOTMATCH;
                    memcpy(ptr, out[i].buffer, len);
                    break;
                case FLD_FLOAT:
                    if(out[i].buffer_type != MYSQL_TYPE_STRING)
                        return ERR_DB_FIELD_TYPE_NOTMATCH;
                    memcpy(ptr, out[i].buffer, sizeof(double));
                    break;
                default:
                    return ERR_DB_INVALID_FIELD_TYPE;
            }
            i++;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

/* 释放select */
void  
db_va_select_close(db_select_t *select)
{
    mem_clean(mem);
    mysql_stmt_close(select->stmt);
}

int 
db_open_select_all(db_instance_t *db, db_select_t *select, char *sql, ...)
{
    va_list vargs;
    va_start(vargs, sql);

    return db_va_open_select(db, select, sql, vargs);
}

int 
db_fetch_select(db_select_t *select, ...)
{
    va_list vargs;
    va_start(vargs, select);
    return db_va_select_fetch(select, vargs);
}

int 
db_open_select_one(db_instance_t *db, char *sql, ...)
{
    va_list vargs;
    va_start(vargs, sql);

    db_select_t select;
    db_select_init(&select);

    int ret = db_va_open_select(db, &select, sql, vargs);
    if(ret)
        return ret;

    int rowCount = mysql_stmt_num_rows(select.stmt);
    if(rowCount < 1)
        return ERR_DB_NOT_FOUND_RESULT;
    if(rowCount > 1)
        return ERR_DB_MULTI_RESULT_FOUND;
    
    ret =  db_va_select_fetch(&select, vargs);
    if(ret)
        return ret;
    db_va_select_close(&select);
    return 0;
}

/* 执行insert,Update操作 */
int
db_va_execute(db_instance_t *db, char *sql, ...)
{
    int       ret;
    va_list   vargs;
    va_start(vargs, sql);
    MYSQL_STMT  *stmt;
    MYSQL_BIND  *params;

    stmt = mysql_stmt_init(db->mysql);
    if(!stmt)
    {
        return -1;
    }

    if(mysql_stmt_prepare(stmt, sql, strlen(sql)))
    {
        return -1;
    }

    int count = mysql_stmt_param_count(stmt);
    if(count > 0)
        params = mem_alloc(mem, count *  sizeof(MYSQL_BIND));

    /* parse in params and set bind parameters */
    ret = va_make_params_to_bind(vargs, params);
    if(ret)
    {
        return ret;
    }

    /* bind stmt parameters */
    ret = mysql_stmt_bind_param(stmt, params);
    if(ret)
    {
        return -1;
    }

    ret = mysql_stmt_execute(stmt);
    if(ret)
    {
        return -1;
    }

    /* check stmt execute result */
    ret = mysql_stmt_affected_rows(stmt);
    if(ret)
    {
        return ret;
    }


    return 0;
}

//Test 
int main()
{
    const char* host = "localhost";  
    const char* user = "root";  
    const char* password = "123456";  
    const char* database = "mybase";  
    const int   port = 3306;  
    const char* socket = NULL;
    const int   flag = 0;

    db_instance_t *db = db_instance_new(host, port, database, user, password);
    if (!db)
    {
        printf("fail to new db\n");
        return -1;
    }
    printf("db_instance_new ok\n");

    db_select_t     select;
    int  ret;
    int  pid;
    char name[32] = {'\0'};
    int depno;
    char title[30] = {'\0'};


    ret = db_open_select_all(db, &select, "select * from person", NULL);
    if(ret)
    {
        printf("fail to open select\n");
        return -1;
    }

    printf("db va open select\n");

    while(1)
    {
        memset(name, 0, 32);
        memset(title, 0, 30);
        ret = db_fetch_select(&select, 
            &pid, FLD_INT, -1,
            name, FLD_STR, 30,
            &depno, FLD_INT, -1,
            title, FLD_STR, 30,
            NULL);
        if(ret)
        {
            break;
        }
        printf("db_va_select_fetch: %d, %s, %d, %s\n", pid, name, depno, title);
    }

    db_va_select_close(&select);

    int     userId;
    char    userName[31];
    char    userTitle[31];
    char    sex[11];

    ret = db_open_select_all(db, &select, "select * from user", NULL);
    if(ret)
    {
        printf("fail to select user\n");
        return -1;
    }

    int count = mysql_stmt_num_rows(select.stmt);
    printf("result count: %d\n", count);

    while(1)
    {
        memset(userName, 0, 31);
        memset(userTitle, 0, 31);
        memset(sex, 0, 11);

        ret = db_fetch_select(&select,
            &userId, FLD_INT, -1,
            userName, FLD_STR, 30,
            userTitle, FLD_STR, 30,
            sex, FLD_STR, 10,
            NULL);
        if(ret)
        {
            break;
        }
        printf("user: %d, %s, %s, %s\n", userId, userName, userTitle, sex);
    }
    db_va_select_close(&select);
/*
    id = 3;
    depno = 120;
    strncpy(name, "Frank", 6);
    strncpy(title, "MarketAnasys", 14);
    printf("db_va_execute \n");
    ret = db_va_execute(db, "insert into person values(?,?,?,?)", 
        &id, FLD_INT, -1, 
        name, FLD_STR, strlen(name),
        &depno, FLD_INT, -1,
        title, FLD_STR, strlen(title),
        NULL);
    printf("execute result: %d\n", ret);
*/

    db_instance_close(db); 

    return 0;
}