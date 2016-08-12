#include <iostream>

using namespace std;

// Node is a struc that has a value and a pointer that points to another instance of Node itself.
struct Node
{
	int value;
	struct Node* next; // pointer to point to struct Node itself.
};

int main()
{
	// Declare x to be a pointer pointing to the struct Node
	Node* x;
	Node* y;
	Node* z;

	// Initializing x to point to an instance of Node
	x = new Node;
	y = new Node;
	z = new Node;

	// Assigning a value to "value" in the struct pointed to by x
	x->value = 1;
	y->value = 2;
	z->value = 3;

	// Pointer "next" in x points to another instance of Node, i.e. y
	x->next = y;
	y->next = z;
	z->next = x;

	// Checking the intended implementation
	cout << "Variable 'value' in x: " << x->next->value << "\n";
	cout << "Variable 'value' in x: " << y->value;

	cin.get();
	return 0;
}