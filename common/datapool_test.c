#include "datapool.h"
#include <stdio.h>

int main()
{
    datapool_t *datapool = datapool_init(512, 512);
    if (datapool == NULL)
    {
        printf("fail to get init datapool");
        return -1;
    }

    datapool_set_string(datapool, 1, "hello");
    datapool_set_long(datapool, 23, 1000);
    datapool_set_double(datapool, 32, 340.99);

    char str[100] = {'\0'};
    long lval;
    double dval;

    datapool_get_string(datapool, 1, str, 100);
    datapool_get_long(datapool, 23, &lval);
    datapool_get_double(datapool,32, &dval);

    datapool_free(&datapool);
    
    return 0;
}