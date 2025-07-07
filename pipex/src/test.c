
#define _GNU_SOURCE
#include <malloc.h>
#include <stdio.h>
int main(void) {
    char *p = malloc(100);
    malloc_info(0, stderr);
    free(p);
    return 0;
}
