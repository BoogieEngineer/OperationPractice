#include <iostream>
#include <cstdint>

using namespace std;

class RGBA // All class members default to be private
{
	uint8_t m_red;
	uint8_t m_green;
	uint8_t m_blue;
	uint8_t m_alpha;

public:
	// Constructor prototype, taking 4 arguments:
	RGBA(int, int, int, int);
	// Function to print:
	void print()
	{
		cout << "Red = " << static_cast<int>(m_red)
			<< ", Green = " << static_cast<int>(m_green)
			<< ", Blue = " << static_cast<int>(m_blue)
			<< ", Alpha = " << static_cast<int>(m_alpha)
			<< "\n";
	}
};

// Constructor using member initialization list:
// (if args not given at function call, the call will use the defaults in the constructor declaration)
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
	RGBA crap(0, 44);
	teal.print();
	crap.print();

	cin.get();
	return 0;
}