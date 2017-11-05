#ifndef _DB_LOAD_H
#define _DB_LOAD_H

// 接口配置
typedef struct db_serice_s {
    char    service_code[7];
    char    service_desc[100];
    int     pkg_id;
} db_service_t;

// 微服务配置
typedef struct db_trade_s {
    char    trade_code[7];
    char    trade_desc[100];
    int     pkg_id;
    char    chantype[5];
} db_trade_t;

typedef struct db_pkg_s {
    int     pkg_id;
    char    pkg_dir;
} db_pkg_t;

#endif
