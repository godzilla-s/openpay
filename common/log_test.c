#include "log.h"

int main()
{
    log_init(ll_info, NULL, "test.log");

    log(ll_info, "Hello: %s", "abcdefgh");
    log(ll_error, "Hello: %d, %s", 200, "rfggh");

    return 0;
}