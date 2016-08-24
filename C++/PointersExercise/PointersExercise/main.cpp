#include <iostream>

using namespace std;

void AddFive(int* Number)
{
	*Number = *Number + 5;
}

int main()
{
	int nMyNumber = 18;

	printf("Original number: %d\n", nMyNumber);
	AddFive(&nMyNumber); // if not using a pointer in the AddFive function, nMyNumber won't change
	printf("New number: %d\n", nMyNumber);

	// Alternatively:
	int *pMyNumber;
	pMyNumber = new int(18);

	printf("Original number: %d\n", *pMyNumber);
	AddFive(pMyNumber);
	printf("New number: %d\n", *pMyNumber);

	cin.get();
	return 0;
}