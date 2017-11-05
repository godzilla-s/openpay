#include <stdio.h>
#include "log.h"
#include "common.h"

struct loginfo_t{
    int     fd;
    FILE    *fp;
    int     level;
    int     istat;                  //是否初始化
    int     fstat;                  //文件是否打开
    char    logfile[65];
    int     ymd;
    struct  tm  *pm;
    char    *homed;
};

char *loglev[4] =
{
    [0] = "ERROR",
    [1] = "INFO",
    [2] = "WARN",
    [3] = "DEBUG"
};

static struct loginfo_t loginfo;
static char  wstream[MAX_LINE_SIZE];

static int offset(const char *s)
{
    int i = 0;
    while(*s++ != '\0')
        i++;
    return i;
} 

void getdatetime()
{
    time_t  tms;
    int             ymd;

    time(&tms);
    loginfo.pm = localtime(&tms);

    ymd = (loginfo.pm->tm_year) * 10000 + (loginfo.pm->tm_mon + 1) * 100 + loginfo.pm->tm_mday;

    if(ymd != loginfo.ymd)
    {
            loginfo.ymd = ymd;
            if(loginfo.fp != NULL)
            {
                fflush(loginfo.fp);
                    fclose(loginfo.fp);
                    loginfo.fstat = -1;
                    loginfo.fp = NULL;
            }
    }

    return ;
}

void gethomepath()
{
    if(loginfo.homed == NULL)
        loginfo.homed = (char *)getenv("HOME");
}

void openlog()
{
    if(loginfo.logfile[0] == '\0'){
            if(logdircheck("/log") != 0){
                    loginfo.fstat = -1;
                    return ;
            }
            sprintf(loginfo.logfile, "%s/log/LOG.%04d%02d%02d.log", (char *)getenv("HOME"), loginfo.pm->tm_year+1900, 
                                loginfo.pm->tm_mon+1, loginfo.pm->tm_mday);
    }

    //fprintf(stderr, "log_file: %s\n", loginfo.logfile);

    loginfo.fp = fopen(loginfo.logfile, "a+");
    if(loginfo.fp == NULL){
        loginfo.fstat = -1;  /* open fail */
        return ;
    }

    loginfo.fstat = 1; /* open ok */
}

int logdircheck(char *path)
{
    int             i = 0;
    char    dir[65] = {'\0'};

    if(loginfo.homed == NULL)
        loginfo.homed = (char *)getenv("HOME");

    strcpy(dir, loginfo.homed);

    i = offset(dir);
    while(*path != '\0'){
        if(*path == '/'){
            dir[i++] = *path++;
        }
        while(*path != '/' && *path != '\0')
            dir[i++] = *path++;

        if(access(dir, 0) != 0){
            if(mkdir(dir, 0755) != 0){
                fprintf(stderr, "Create log file path fail\n");
                return -1;
            }
        }
    }
    return 0;
}

void loginit(int level, char *path, char *outfile)
{
    int offset;

    memset(&loginfo, 0, sizeof(loginfo));

    if(level > ll_debug || level < ll_error)
                loginfo.level = ll_error;

    gethomepath();

    if(path == NULL){
        offset = sprintf(loginfo.logfile, "%s/log", loginfo.homed);
    }
    else{
        offset = sprintf(loginfo.logfile, "%s%s", loginfo.homed, path);
    }

    if(logdircheck(path) != 0)
        return ;

    getdatetime();

    if(outfile == NULL){
        sprintf(loginfo.logfile + offset, "/LOG.%04d%02d%02d.log", loginfo.pm->tm_year+1900, loginfo.pm->tm_mon+1, 
                loginfo.pm->tm_mday);
    }
    else{
        sprintf(loginfo.logfile + offset, "/%s.%04d%02d%02d.log", outfile, loginfo.pm->tm_year+1900, loginfo.pm->tm_mon+1,
                loginfo.pm->tm_mday);
    }

    loginfo.level = level;

    loginfo.istat = 1;

    return ;
}

void rawlog(int level, const char *file, int line, char *fmt, ...)
{
    char    *pstr = null;
    if(level > ll_debug || level < ll_error)
        return ;

    if(loginfo.istat == 0)
        loginfo.level = level;

    if(loginfo.istat == 1 && level > loginfo.level)
        return ;

    getdatetime();

    if(loginfo.fp == null)
        openlog();

    if(loginfo.fstat == -1)
        return ;

    memset(wstream, 0 , MAX_LINE_SIZE);

    pstr = wstream;

    sprintf(pstr, "[%04d/%02d/%02d %02d:%02d:%02d][%d][%s][%s:%d] ", loginfo.pm->tm_year+1900, loginfo.pm->tm_mon+1, 
            loginfo.pm->tm_mday, loginfo.pm->tm_hour, loginfo.pm->tm_min, loginfo.pm->tm_sec, getpid(), loglev[loginfo.level-1], 
            file, line);

    pstr = pstr + offset(pstr);

    va_list argvs;
    va_start(argvs, fmt);
    vsnprintf(pstr, MAX_LINE_SIZE, fmt, argvs);
    va_end(argvs);

    fprintf(loginfo.fp, "%s\n", wstream);

    fflush(loginfo.fp);

    return ;
}