#include <stdio.h>
#include <string.h>
#include "mysql.h"

#define FLD_INT     1
#define FLD_STR     2

int db_query(MYSQL *mysql, char *sql, ...);

int main()
{
    MYSQL   mysql; 
    const char* host = "localhost";  
    const char* user = "root";  
    const char* password = "root123456";  
    const char* database = "mybase";  
    const int   port = 3306;  
    const char* socket = NULL;
    const int flag = 0;

    if(!mysql_init(&mysql))
    {
        printf("fail to init mysql\n");
        return -1;
    }

    printf("mysql init ok\n");

    if(!mysql_real_connect(&mysql, host, user, password, database, port, socket, flag)) {  
        printf("fail to connect: %s\n", mysql_error(&mysql));
        return -1;  
    }

    printf("mysql connect ok\n");

    int id;
    char name[30] = {'\0'};
    char sex[10] = "male";
    int age;
    int limit = 34;
    char nsex[10] = {'\0'};

    db_query(&mysql, "select name, sex, age from person where sex=? and age>?", 
            sex, FLD_STR, 4,
            &limit, FLD_INT, -1,
            NULL,
            name, FLD_STR, 30,
            nsex, FLD_STR, 10,
            &age, FLD_INT, -1,
            NULL);
    printf("%s, %d, %s\n", name, age, nsex);

    mysql_close(&mysql);

    return 0;   
}

int db_query(MYSQL *mysql, char *sql, ...)
{
    va_list  vargs;
    void     *ptr; 
    int       type;
    int       len;
    int       i = 0;
    int       paramCount = 0;
    va_start(vargs, sql);

    MYSQL_STMT  *stmt = mysql_stmt_init(mysql);
    if (stmt == NULL)
    {
        printf("fail to init stmt: %s\n", mysql_error(mysql));
        return -1;
    }

    printf("mysql stmt init ok\n");

    mysql_stmt_prepare(stmt, sql, strlen(sql));

    printf("mysql stmt prepare ok\n");

    paramCount = mysql_stmt_param_count(stmt);
    
    MYSQL_BIND  *bindIn = calloc(paramCount, sizeof(MYSQL_BIND));

    printf("bind in params: %d\n", paramCount);

    while(paramCount && (ptr = va_arg(vargs, void *)) != NULL)
    {
        type = va_arg(vargs, int);
        len = va_arg(vargs, int);

        if (type == FLD_INT)
        {
            bindIn[i].buffer_type = MYSQL_TYPE_LONG;
            bindIn[i].buffer = (char *)ptr; 
            bindIn[i].buffer_length = sizeof(long);
        }

        if (type == FLD_STR)
        {
            bindIn[i].buffer_type = MYSQL_TYPE_STRING;
            bindIn[i].buffer = (char *)ptr;
            bindIn[i].buffer_length = len;
        }

        i++;
    }

    if(mysql_stmt_bind_param(stmt, bindIn))
    {
        printf("fail to stmt bind params: %s\n", mysql_stmt_error(stmt));
        return -1;
    }

    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "fail to stmt execute %s\n", mysql_stmt_error(stmt));
        return -1;
    }

    MYSQL_RES *prepare_meta_result = mysql_stmt_result_metadata(stmt);
    if(!prepare_meta_result)
    {
        printf("fail to get stmt result metadata:%s\n", mysql_stmt_error(stmt));
        return -1;
    }

    MYSQL_FIELD *fields = mysql_fetch_fields(prepare_meta_result);

    int column_count= mysql_num_fields(prepare_meta_result);
    if(column_count <= 0)
    {
        printf("coulumn number should greater than 0\n");
        return -1;
    }

    // 打印字段fields 
    for (i=0; i<column_count; i++)
    {
        printf("%s ", fields[i].name);
    }
    printf("\n");

    MYSQL_BIND  *bindOut = calloc(column_count, sizeof(MYSQL_BIND));
    my_bool  *is_null = calloc(column_count, sizeof(my_bool));
    my_bool  *error = calloc(column_count, sizeof(my_bool));
    unsigned long *length = calloc(column_count, sizeof(unsigned long));

    printf("bind out params: %d\n", column_count);

    i=0;
    while((ptr = va_arg(vargs, void *)) != NULL)
    {
        type = va_arg(vargs, int);
        len = va_arg(vargs, int);

        if (type == FLD_INT)
        {
            bindOut[i].buffer_type = MYSQL_TYPE_LONG;
            bindOut[i].buffer = (char *)ptr;
            bindOut[i].is_null = &is_null[i];
            bindOut[i].length = &length[i];
            bindOut[i].error = &error[i];
        }

        if(type == FLD_STR)
        {
            bindOut[i].buffer_type = MYSQL_TYPE_STRING;
            bindOut[i].buffer = (char *)ptr;
            bindOut[i].is_null = &is_null[i];
            bindOut[i].buffer_length = len;
            bindOut[i].length = &length[i];
            bindOut[i].error = &error[i];
        }
        i++;
    }

    if(mysql_stmt_bind_result(stmt, bindOut))
    {
        printf("fail to bind result:%s\n", mysql_stmt_error(stmt));
        return -1;
    }

    if (mysql_stmt_store_result(stmt))
    {
        fprintf(stderr, " mysql_stmt_store_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }

    while(!mysql_stmt_fetch(stmt))
    {
        for(i=0; i<column_count; i++)
        {
            if(bindOut[i].buffer_type == MYSQL_TYPE_LONG)
            {
                if(is_null[i])
                    printf("NULL\t");
                else
                    printf("%d\t", *(int *)bindOut[i].buffer);
            }

            if(bindOut[i].buffer_type == MYSQL_TYPE_STRING)
            {
                if(is_null[i])
                    printf("NULL\t");
                else
                    printf("%s\t", (char *)bindOut[i].buffer);
            }
        }
        printf("\n");
    }

    free(bindIn);

    free(bindOut);

    //mysql_stmt_free_result(stmt);

    mysql_free_result(prepare_meta_result);

    mysql_stmt_close(stmt);

    return 0;
}

typedef struct db_select_s
{
    MYSQL_RES    *result;
    MYSQL_STMT   *stmt; 
    MYSQL_FIELD  *fields;
} db_select_t;

int db_select_open_by(db_select_t *select)
{
    return 0;
}

int db_select_fetch(db_select_t *select, void *data)
{
    return 0;
}

int db_select_close(db_select_t *select)
{
    mysql_free_result(select->result);
    mysql_stmt_close(select->stmt); 
    return 0;
}
