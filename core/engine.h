// 服务引擎

#ifndef _ENGINE_H
#define _ENGINE_H

#include "service.h"
#include "trade.h"
#include "pkg.h"

typedef struct engine_s
{
    service_t   services[EGN_MAX_SERVICE];
    trade_t     trades[EGN_MAX_TRADE];
    module_t    modules[EGN_MAX_MODULE];
    pkg_t       pkgs[EGN_MAX_SERVICE];
} engine_t;

#endif
