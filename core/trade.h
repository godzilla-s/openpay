#ifndef _TRADE_H
#define _TRADE_H

typedef struct trade_s
{
    char    trade_code[7];
    char    trade_data[9];
    char    trade_time[7];
    char    trade_logno[7];
} trade_t;

#endif
