#include <iostream>
#include <string>
#include <bitset>

using namespace std;

int main()
{
    int a = 7;
    long b = 1 << a;
    cout << bitset<8>(b) << "\n" << bitset<8>(128);
    return 0;
}
