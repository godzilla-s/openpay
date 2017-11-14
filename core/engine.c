#include <stdio.h>
#include "engine.h"
#include "common.h"

int 
engine_init()
{
    engine_t *engine = malloc(sizeof(engine_t));
    if (engine == NULL)
    {
        log(ll_error, "fail to engine init");
        return -1;
    }

}