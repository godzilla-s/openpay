#include <stdio.h>
#include "engine.h"
#include "common.h"

int 
engine_init()
{
    int     ret;
    engine_t *engine = malloc(sizeof(engine_t));
    if (engine == NULL)
    {
        log(ll_error, "fail to engine init");
        return -1;
    }

    ret = load_service(&engine->service);
    if (ret)
    {
        log(ll_error, "fail to load service: %s\n");
        return -1;
    }

    ret = load_trade(&engine->trades);
    if (ret)
    {
        log(ll_error, "fail to load trades: %s");
        return -1;
    }

    ret = load_pkg(&engine->pkg);
    if (ret)
    {
        log(ll_error, "fail to load pkg");
        return -1;
    }

    ret = load_flowchart(&engine->flowchart);
    if (ret)
    {
        log(ll_error, "fail to load flowchart")
    }
}