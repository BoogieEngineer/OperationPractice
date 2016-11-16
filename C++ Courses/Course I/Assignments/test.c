#include <stdio.h>
#include <stdlib.h>

#define Elements(A) (sizeof(A)/sizeof(*(A)))

int main(void)
{

    static char *p[] =
{"now's the", "how now brown",
"my letter is in the mail"};
printf("%s %s %s", &*(p+0)[6], &p[1][8], &p[2][3]);

    return(EXIT_SUCCESS);
}
