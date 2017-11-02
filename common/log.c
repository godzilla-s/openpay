#include <stdio.h>
#include "log.h"

struct log_file 
{
    char    file_name[32];
    int     level;
};

static struct log_file;

// 日志初始化 
void 
log_init(int level, char *path, char *logfile, int mask)
{

}


static void 
log_write(int level, char *path, char *file_name, int mask)
{

}