#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ll_debug        4
#define ll_warn         3
#define ll_info         2
#define ll_error        1

// 一次最多写入数据
#define MAX_LINE_SIZE    2048

void loginit(int level, char *path, char *outfile);

void rawlog(int level, const char *file, int line, char *fmt, ...);

#define V_INFO(args)  __FILE__,__LINE__

#define log(level, ...) rawlog(level, V_INFO(args), __VA_ARGS__)

#endif
