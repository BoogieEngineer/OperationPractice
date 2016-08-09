#include <iostream>
#include <cstdint>

using namespace std;

class RGBA
{
	int m_red;
	int m_green;
	int m_blue;
	int m_alpha;

public:
	RGBA(int, int, int, int);
	void print()
	{
		cout << "Red = " << m_red << ", Green = " << m_green << ", Blue = " << m_blue << ", Alpha = " << m_alpha << "\n";
	}
};

RGBA::RGBA(int r=0, int g=0, int b=0, int a=255):
    m_red(r),
	m_green(g),
	m_blue(b),
	m_alpha(a)
{
}

int main()
{
	RGBA teal(0, 127, 127);
	teal.print();

	cin.get();
	return 0;
}