#include <cstdlib>
#include <iostream>

#include "cmlib/image/Color.h"

using namespace std;
using namespace cmlib::image;

// V1 op= V2 {{{
template<typename Type1, typename Type2>
void test_plus_assign(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") + (" << output(v2) << ") = (";
	v1 += v2;
	cout << output(v1) << ")" << endl;
}

template<typename Type1, typename Type2>
void test_minus_assign(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") - (" << output(v2) << ") = (";
	v1 -= v2;
	cout << output(v1) << ")" << endl;
}

template<typename Type1, typename Type2>
void test_multiply_assign(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") * (" << output(v2) << ") = (";
	v1 *= v2;
	cout << output(v1) << ")" << endl;
}

template<typename Type1, typename Type2>
void test_divide_assign(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") / (" << output(v2) << ") = (";
	v1 /= v2;
	cout << output(v1) << ")" << endl;
}

template<typename Type1, typename Type2>
void test_modulus_assign(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") % (" << output(v2) << ") = (";
	v1 %= v2;
	cout << output(v1) << ")" << endl;
}
// }}}

// V1 op V2 {{{
template<typename Type1, typename Type2>
void test_plus(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") + (" << output(v2) << ") = (" << output(v1+v2) << ")" << endl;
}

template<typename Type1, typename Type2>
void test_minus(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") - (" << output(v2) << ") = (" << output(v1-v2) << ")" << endl;
}

template<typename Type1, typename Type2>
void test_multiply(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") * (" << output(v2) << ") = (" << output(v1*v2) << ")" << endl;
}

template<typename Type1, typename Type2>
void test_divide(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") / (" << output(v2) << ") = (" << output(v1/v2) << ")" << endl;
}

template<typename Type1, typename Type2>
void test_modulus(Type1 v1, const Type2& v2)
{
	cout << "(" << output(v1) << ") % (" << output(v2) << ") = (" << output(v1%v2) << ")" << endl;
}
// }}}

template<typename Type>
Type random()
{
	double r = static_cast<double>(rand())/RAND_MAX;
	return r*cmlib::image::TypeTrait<Type>::domain();
}

int main()
{
	srand(time(0));
	Byte1 b1 = random<Byte1>();
	Byte3 b3(random<Byte1>(), random<Byte1>(), random<Byte1>());
	Byte4 b4(random<Byte1>(), random<Byte1>(), random<Byte1>(), random<Byte1>());
	Short1 s1 = random<Short1>();
	Short3 s3(random<Short1>(), random<Short1>(), random<Short1>());
	Short4 s4(random<Short1>(), random<Short1>(), random<Short1>(), random<Short1>());
	Float1 f1 = random<Float1>();
	Float3 f3(random<Float1>(), random<Float1>(), random<Float1>());
	Float4 f4(random<Float1>(), random<Float1>(), random<Float1>(), random<Float1>());

	test_plus_assign(f3, b3);
	test_minus_assign(f3, b3);
	test_multiply_assign(f3, b3);
	test_divide_assign(f3, b3);
	test_modulus_assign(s3, b3);

	test_plus_assign(f3, random<Short1>());
	test_minus_assign(f3, random<Short1>());
	test_multiply_assign(f3, random<Short1>());
	test_divide_assign(f3, random<Short1>());
	test_modulus_assign(s3, random<Short1>());

	test_plus(f3, b3);
	test_minus(f3, b3);
	test_multiply(f3, b3);
	test_divide(f3, b3);
	test_modulus(s3, b3);

	test_plus(f3, random<Short1>());
	test_minus(f3, random<Short1>());
	test_multiply(f3, random<Short1>());
	test_divide(f3, random<Short1>());
	test_modulus(s3, random<Short1>());

	test_plus(random<Short1>(), f3);
	test_minus(random<Short1>(), f3);
	test_multiply(random<Short1>(), f3);
	test_divide(random<Short1>(), f3);
	test_modulus(random<Short1>(), s3);
	return 0;
}
