#include <stdio.h>

int main(void) {
    int x, y;
    
    printf("x = "); fflush(stdout);
    scanf("%d", &x);

    printf("y = "); fflush(stdout);
    scanf("%d", &y);

    while (x != y) {
        if (x < y)
            y -= x;
        else
            x -= y;
    }

    printf("ggt = %d\n", y);

    return 0;
}

