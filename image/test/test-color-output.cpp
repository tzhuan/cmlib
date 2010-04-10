#include <iostream>
#include <cmlib/image.h>

using namespace std;
using namespace cmlib::image;

int main()
{
	Byte1 b1(1);
	Byte3 b3(2, 3, 4);
	Byte4 b4(5, 6, 7, 8);
	Float1 f1(9.1);
	Float3 f3(11.12, 13.14, 15.16);
	Float4 f4(17.18, 19.2, 20.21, 22.23);
	cout << output(b1) << endl;
	cout << output(b3) << endl;
	cout << output(b4) << endl;
	cout << output(f1) << endl;
	cout << output(f3) << endl;
	cout << output(f4) << endl;
	return 0;
}
