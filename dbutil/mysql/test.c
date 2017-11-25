#include "dbraw.h" 
#include <stdio.h>  

int main()
{
    const char* host = "localhost";  
    const char* user = "root";  
    const char* password = "root123456";  
    const char* database = "mybase";  
    const int   port = 3306;  
<<<<<<< HEAD
    const char* socket = NULL;  
    const int flag = 0;  
    const char* sql ;     
    int num_fields;  
    unsigned long * lengths;  
    int i;  
    //initialize the database   
    if(!mysql_init(&mysql) ) {  
        output_error(&mysql);  
    }  
    printf("mysql initialized successfully ! \n");  
    //connect to the database;  
    if(!mysql_real_connect(&mysql, host, user, password, database, port, socket, flag)) {  
        output_error(&mysql);  
    }  
    printf("mysql connect successfully! \n");  
    printf("\n\n\nthe content of the table data in the database test\n");  
    printf("-----------------------------------------------------------\n");  
    //do the select query on the database;  
    sql = "select * from person" ;  
    //printf("%d : %d/n", sizeof(sql), strlen(sql)); // 4：18 sizeof(sql):the size of point --(4); strlen(sql):  
    if( mysql_real_query(&mysql, sql, strlen(sql)) ){  
        output_error(&mysql);  
    }  
    //fetch the the result set of the query!          
    result = mysql_store_result(&mysql);  
    if(result) {  
        fields = mysql_fetch_fields(result);    // fetch the struct of result          
        num_fields = mysql_num_fields(result);  // fetch the number of result fields;  
          
        //lengths = mysql_fetch_lengths(result);      
        for(i=0; i<num_fields; i++) {  
            printf("%s\t", fields[i].name );  
        }  
        printf("\n");  
        while(row = mysql_fetch_row(result)) {  
            for(i=0; i<num_fields; i++) {  
                printf("%s \t",  row[i]);  
             }  
             printf("\n");  
        }  
        //release the result of set  for release the memory  
        mysql_free_result(result);    
          
    }  
    else {  
        output_error(&mysql);  
    }  
    printf("\n\n-----------------------------------------------------------\n");  
    //close the connetion to the database  
    mysql_close(&mysql);  
    return 0;  
}  
 static void output_error(MYSQL * mysql) {  
    fprintf(stderr, "errorno: %d \n", mysql_errno(mysql) );  
    fprintf(stderr, "error info: %s \n", mysql_error(mysql) );  
    exit(1);  
}  

// 参考： http://zetcode.com/db/mysqlc/ 
// 参考： http://ask.csdn.net/questions/242518

=======
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
>>>>>>> 9798e6b992f44982d607cf5aadeeaede7fba9106
