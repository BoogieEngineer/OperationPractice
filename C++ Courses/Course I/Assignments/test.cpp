#include <iostream>
#include <string>

using namespace std;

int main()
{
    string buf("Length");
buf += " of" + " of this is: ";
string buf2("Strange characters" + 8, "Strange characters" + 18);
cout << buf << buf2.size() << ' ' << buf2;

    return 0;
}
