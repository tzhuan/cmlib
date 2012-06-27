/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <cstdlib>
#include <iostream>

#include "cmlib/image.h"

using namespace std;
using namespace cmlib::image;

// V1 op= V2 {{{
template<typename Type1, typename Type2>
void test_plus_assign(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} + {" << output(v2) << "} = {";
	v1 += v2;
	cout << output(v1) << "}" << endl;
}

template<typename Type1, typename Type2>
void test_minus_assign(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} - {" << output(v2) << "} = {";
	v1 -= v2;
	cout << output(v1) << "}" << endl;
}

template<typename Type1, typename Type2>
void test_multiply_assign(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} * {" << output(v2) << "} = {";
	v1 *= v2;
	cout << output(v1) << "}" << endl;
}

template<typename Type1, typename Type2>
void test_divide_assign(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} / {" << output(v2) << "} = {";
	v1 /= v2;
	cout << output(v1) << "}" << endl;
}

template<typename Type1, typename Type2>
void test_modulus_assign(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} % {" << output(v2) << "} = {";
	v1 %= v2;
	cout << output(v1) << "}" << endl;
}
// }}}

// V1 op V2 {{{
template<typename Type1, typename Type2>
void test_plus(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} + {" << output(v2) << "} = {" << output(v1+v2) << "}" << endl;
}

template<typename Type1, typename Type2>
void test_minus(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} - {" << output(v2) << "} = {" << output(v1-v2) << "}" << endl;
}

template<typename Type1, typename Type2>
void test_multiply(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} * {" << output(v2) << "} = {" << output(v1*v2) << "}" << endl;
}

template<typename Type1, typename Type2>
void test_divide(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} / {" << output(v2) << "} = {" << output(v1/v2) << "}" << endl;
}

template<typename Type1, typename Type2>
void test_modulus(Type1 v1, const Type2& v2)
{
	cout << "{" << output(v1) << "} % {" << output(v2) << "} = {" << output(v1%v2) << "}" << endl;
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

	FloatImage3 f3(3, 1);
	f3.fill(3.8);

	ByteImage3 b3(3, 1);
	b3.fill(7);

	ShortImage3 s3(3, 1);
	s3.fill(123);

	test_plus_assign(f3, b3);
	test_plus(f3, b3);
	test_plus(f3, 5);
	test_plus(f3, Byte3(2, 3, 4));
	test_plus(6, f3);
	test_plus(Byte3(2, 3, 4), f3);

	test_multiply_assign(f3, b3);
	test_multiply(f3, b3);
	test_multiply(f3, 5);
	test_multiply(f3, Byte3(2, 3, 4));
	test_multiply(6, f3);
	test_multiply(Byte3(2, 3, 4), f3);

	test_minus_assign(f3, b3);
	test_minus(f3, b3);
	test_minus(f3, 5);
	test_minus(f3, Byte3(2, 3, 4));
	test_minus(6, f3);
	test_minus(Byte3(2, 3, 4), f3);

	test_divide_assign(f3, b3);
	test_divide(f3, b3);
	test_divide(f3, 5);
	test_divide(f3, Byte3(2, 3, 4));
	test_divide(6, f3);
	test_divide(Byte3(2, 3, 4), f3);

	test_modulus_assign(s3, b3);
	test_modulus(s3, b3);
	test_modulus(s3, 5);
	test_modulus(s3, Byte3(2, 3, 4));
	test_modulus(6, s3);
	test_modulus(Byte3(2, 3, 4), s3);
	return 0;
}
