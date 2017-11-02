#ifndef _PKG_H
#define _PKG_H

// 消息报
typedef struct pkg_s
{
    char    pkg_type[5];
    char    direct[2];
    char    def_val[100];
} pkg_t

#endif
