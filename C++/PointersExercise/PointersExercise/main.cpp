#include <iostream>

using namespace std;

//======================================
//DOING: Add 5 to a number, using a pointer.
//void AddFive(int* Number)
//{
//	*Number = *Number + 5;
//}
//
//int main()
//{
//	int nMyNumber = 18;
//
//	printf("Original number: %d\n", nMyNumber);
//	AddFive(&nMyNumber); // if not using a pointer in the AddFive function, nMyNumber won't change
//	printf("New number: %d\n", nMyNumber);
//
//	// Alternatively:
//	int *pMyNumber;
//	pMyNumber = new int(18);
//
//	printf("Original number: %d\n", *pMyNumber);
//	AddFive(pMyNumber);
//	printf("New number: %d\n", *pMyNumber);
//
//	cin.get();
//	return 0;
//}
//======================================
//DOING: Pointer to a class. Then access class members through the pointer
//Also have class inheritance.
class Polygon
{
public:
	int m_width;
	int m_height;

	void set_values(int a, int b)
	{
		m_width = a;
		m_height = b;
	}
};

class Rectangle: public Polygon
{
public:
	int area()
	{
		return m_width * m_height;
	}
};

class Triangle: public Polygon
{
public:
	int area()
	{
		return m_width * m_height / 2;
	}
};

int main()
{
	Rectangle * p_rect;
	p_rect = new Rectangle;
	Triangle * p_trgl;
	p_trgl = new Triangle;

	p_rect->set_values(4, 5);
	p_trgl->set_values(4, 5);

	cout << "Rectangle's area: " << p_rect->area() << endl;
	cout << "Triangle's area: " << p_trgl->area() << endl;

	cin.get();
	return 0;
}