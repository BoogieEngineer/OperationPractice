#include <iostream>

using namespace std;

int main()
{
    int a = 5;

    int *x = &a;

    cout << x << "\n";
    cout << *x << "\n";

    return 0;
}
