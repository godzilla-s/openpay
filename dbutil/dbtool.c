// dbtool 自动生成数据库操作的工具
#include <stdio.h>
#include <unistd.h>

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

    while((opt = getopt(args, argv, "f:o")) != -1)
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