#include <iostream>

using namespace std;

int main()
{
	
	/* PROGRAM TO PRINT OUT SPACE RESERVED FOR VARIABLES */
	char c;
	short s;
	int i=7;
	unsigned int ui;
	unsigned long int ul;
	float f;
	double d;
	long double ld;
	cout << endl
		<< "The storage space for each variable type is:"
		<< endl;
	cout << endl << "char: \t\t\t%d bits", sizeof(c) * 8;  //  \t means tab 
	cout << endl << "short: \t\t\t%d bits", sizeof(s) * 8;
	cout << endl << "int: \t\t\t%d bits", sizeof(i) * 8;
	cout << endl << "unsigned int: \t\t%d bits", sizeof(ui) * 8;
	cout << endl << "unsigned long int: \t%d bits", sizeof(ul) * 8;
	cout << endl << "float: \t\t\t%d bits", sizeof(f) * 8;
	cout << endl << "double: \t\t%d bits", sizeof(d) * 8;
	cout << endl << "long double: \t\t%d bits", sizeof(ld) * 8;

	cin.get();
	return 0;
}