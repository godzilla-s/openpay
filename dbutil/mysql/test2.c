#include <stdio.h>
#include <string.h>
#include "mysql.h"

#define FLD_INT     1
#define FLD_STR     2

int main()
{
    MYSQL   mysql; 
    const char* host = "localhost";  
    const char* user = "root";  
    const char* password = "root123456";  
    const char* database = "mybase";  
    const int   port = 3306;  

    if(!mysql_init(&mysql))
    {
        printf("fail to init mysql\n");
        return -1;
    }

    if(!mysql_real_connect(&mysql, host, user, password, database, port, socket, flag)) {  
        printf("fail to connect: %s\n", mysql_error(&mysql));
        return -1;  
    }

    int id;
    char name[30] = {'\0'};
    db_query(&mysql, "select * from person where id=?", 
            "male", FLD_STR, 4,
            NULL,
            name, FLD_STR, 30,
            NULL);
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

    MYSQL_STMT  *stmt = mysql_stmt_init(&mysql);
    if (stmt == NULL)
    {
        printf("fail to init stmt: %s\n", mysql_error(&mysql));
        return -1;
    }

    mysql_stmt_prepare(stmt, sql, strlen(sql));
    paramCount = mysql_stmt_param_count(stmt);
    
    MYSQL_BIND  *bindIn = calloc(paramCount, sizeof(MYSQL_BIND));

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

    int column_count= mysql_num_fields(prepare_meta_result);
    if(column_count <= 0)
    {
        printf("coulumn number should greater than 0\n");
        return -1;
    }

    MYSQL_BIND  *bindOut = calloc(column_count, sizeof(MYSQL_BIND));
    i=0;
    my_bool  *is_null = calloc(column_count, sizeof(my_bool));
    my_bool  *error = calloc(column_count, sizeof(my_bool));
    unsigned long *length = calloc(cloumn_count, sizeof(unsigned long));

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
            printf("\n");
        }
    }

    free(bindIn);

    free(bindOut);

    //mysql_stmt_free_result(stmt);

    mysql_free_result(prepare_meta_result);

    mysql_stmt_close(stmt);

    return 0;
}