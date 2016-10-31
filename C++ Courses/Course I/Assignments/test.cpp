#include <iostream>

using namespace std;

void copy(int *a, const int *b)
    {
        *a = *b;
    }

int main()
{
    

    const int a = 5, b = 10;

    copy(&a, &b);
}
