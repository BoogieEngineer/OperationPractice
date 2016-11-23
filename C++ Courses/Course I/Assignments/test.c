#include <stdio.h>
#include <stdlib.h>

#define Elements(A) (sizeof(A)/sizeof(*(A)))

int main(void)
{
    int ax;
    scanf("%d", &ax);
    ungetc('a', stdin);
    scanf("%x", &ax);
    printf("%x", ax);

    return(EXIT_SUCCESS);
}
