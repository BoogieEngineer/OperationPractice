#include <iostream>
using namespace std;

int main()
{
    int x; // define variable that will take value from user-input
    cout << "Please provide an integer: "; //prompting the user to provide input
    cin >> x; // read user input

    cout << x << " decimal = " << oct << x << " octal = " << hex << x << " hexadecimal"; // print the results

    cin.get();
    cin.get();
    return 0;
}