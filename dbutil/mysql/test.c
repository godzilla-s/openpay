#include "dbraw.h" 
#include <stdio.h>  

int main()
{
    const char* host = "localhost";  
    const char* user = "root";  
    const char* password = "root123456";  
    const char* database = "mybase";  
    const int   port = 3306;  
    const char* socket = NULL;
    const int flag = 0;
    int     ret;
printf("db_va_instance_new\n");
    //db_va_instance_new(char *host, int port, char *dbname, char *dbuser, char *dbpasswd)
    db_instance_t *db = db_va_instance_new(host, port, database, user, password);
    if (db == NULL)
    {
        printf("fail to new db instance\n");
        return -1;
    }

    printf("db_va_instance_new ok\n");

    db_select_t     select;
    char cond[] = "male";
    char name[32] = {'\0'};
    char sex[10] = {'\0'};
    char remark[50] = {'\0'};
    int  age;

    ret = db_va_open_select(db, &select, "select * from person where sex=?", 
        cond, FLD_STR, 5,
        NULL
        );
    if (ret)
    {
        printf("fail to open select\n");
        return -1;
    }

    printf("db_va_open_select ok\n");

    ret = db_va_select_fetch(&select, 
        name, FLD_STR, 30,
        sex, FLD_STR, 10,
        &age, FLD_INT, -1,
        remark, FLD_STR, 30,
        NULL);
    if(ret)
    {
        printf("fail to select fetch\n");
        return -1;
    }
    printf("%s, %d, %s, %s\n", name, age, sex, remark);


    db_va_select_close(&select);
    db_va_instance_close(db);

    return 0;
}