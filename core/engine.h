// 服务引擎

#ifndef _ENGINE_H
#define _ENGINE_H

typedef engine_s struct
{
    service_t   services[EGN_MAX_SERVICE];
    trade_t     trades[EGN_MAX_TRADE];
    module_t    modules[EGN_MAX_MODULE];
} engine_t;

#endif
