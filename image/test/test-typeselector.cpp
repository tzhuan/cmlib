#include <iostream>
#include <cmlib/image/ColorSelector.h>

using namespace std;
using namespace cmlib::image;

template<typename Type>
ostream& print(ostream&, Type);

#define PRINT(TYPE) \
	template<> \
	ostream& print<TYPE>(ostream& os, TYPE) \
	{ \
		os << #TYPE; \
	}

#define TEST(OS, TYPE1, TYPE2) \
	OS << #TYPE1 << " vs. " << #TYPE2 << ": "; \
	print<typename ColorSelector<TYPE1, TYPE2>::priority_type>(OS, 0); \
	OS << std::endl;

#define TESTS(OS, TYPE) \
	TEST(OS, TYPE, char); \
	TEST(OS, TYPE, unsigned char); \
	TEST(OS, TYPE, short); \
	TEST(OS, TYPE, unsigned short); \
	TEST(OS, TYPE, int); \
	TEST(OS, TYPE, unsigned int); \
	TEST(OS, TYPE, long); \
	TEST(OS, TYPE, unsigned long); \
	TEST(OS, TYPE, float); \
	TEST(OS, TYPE, double);

PRINT(char);
PRINT(unsigned char);
PRINT(short);
PRINT(unsigned short);
PRINT(int);
PRINT(unsigned int);
PRINT(long);
PRINT(unsigned long);
PRINT(float);
PRINT(double);

int main()
{
	TESTS(cout, char);
	TESTS(cout, unsigned char);
	TESTS(cout, short);
	TESTS(cout, unsigned short);
	TESTS(cout, int);
	TESTS(cout, unsigned int);
	TESTS(cout, long);
	TESTS(cout, unsigned long);
	TESTS(cout, float);
	TESTS(cout, double);
	return 0;
}
