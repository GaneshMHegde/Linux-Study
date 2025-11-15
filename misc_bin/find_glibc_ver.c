#include <stdio.h>

// Manually declare the external function
extern const char *gnu_get_libc_version(void);

int main() {
    printf("glibc version: %s\n", gnu_get_libc_version());
    return 0;
}

