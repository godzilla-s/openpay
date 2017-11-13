#include "mempool.h"
#include <stdio.h>

int main()
{
    mempool_t *pool = mempool_new(0x500);

    char *a = mempool_alloc(pool, 100);
    strcpy(a, "abcdefgthth");

    char *b = mempool_alloc(pool, 200);
    strcpy(b, "dasf4f4grg");

    struct test {
        int     a;
        char    b[20];
        int     c;
    };

    struct test *p = mempool_alloc(pool, sizeof(struct test));
    p->a = 100;
    strcpy(p->b, "hello");
    p->c = 200;
    
    mempool_free(pool, a);
    mempool_free(pool, b);

    mempool_destroy(pool);

    return 0;
}