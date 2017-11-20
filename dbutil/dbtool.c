// dbtool 自动生成数据库操作的工具
#include <stdio.h>
#include <unistd.h>

char *key_word[] = {
    "action",
    "table",
    "read_by",
    "order_by",
    "order_by_desc",
    "group_by",
    "add",
    "update_by",
    "function"
};

char *data_type[] = {
    "int",
    "str",
    "double"
};

typedef struct tblfield {
    char    type[8];
    char    name[33];
    int     length;
};

int 
read_file(char *fileName, char *stream)
{
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp != NULL)
    {
        return -1;
    }
}

int function_name()
{
    
}

int 
main(int args, char *argv[])
{
    char  opt = '\0';
    char *fileName = NULL;
    char *outfile = NULL;

    while((opt = getopt(args, argv, "f:o:")) != -1)
    {
        switch(opt)
        {
            case 'f':
                fileName = optarg;
            case 'o':
                outfile = optarg;
        }
    }

    return 0;
}
// dbtool -f db_server.act -o