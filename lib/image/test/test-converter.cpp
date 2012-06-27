/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <exception>
#include <iostream>
#include <string>

#include <cmlib/image.h>

using namespace std;
using namespace cmlib::image;

template<typename Type> string type_name();
template<> string type_name<Byte1>() { return "Byte1"; }
template<> string type_name<Byte3>() { return "Byte3"; }
template<> string type_name<Byte4>() { return "Byte4"; }
template<> string type_name<Short1>() { return "Short1"; }
template<> string type_name<Short3>() { return "Short3"; }
template<> string type_name<Short4>() { return "Short4"; }
template<> string type_name<Float1>() { return "Float1"; }
template<> string type_name<Float3>() { return "Float3"; }
template<> string type_name<Float4>() { return "Float4"; }
template<> string type_name<Double1>() { return "Double1"; }
template<> string type_name<Double3>() { return "Double3"; }
template<> string type_name<Double4>() { return "Double4"; }
template<> string type_name<ByteImage1>() { return "ByteImage1"; }
template<> string type_name<ByteImage3>() { return "ByteImage3"; }
template<> string type_name<ByteImage4>() { return "ByteImage4"; }
template<> string type_name<ShortImage1>() { return "ShortImage1"; }
template<> string type_name<ShortImage3>() { return "ShortImage3"; }
template<> string type_name<ShortImage4>() { return "ShortImage4"; }
template<> string type_name<FloatImage1>() { return "FloatImage1"; }
template<> string type_name<FloatImage3>() { return "FloatImage3"; }
template<> string type_name<FloatImage4>() { return "FloatImage4"; }
template<> string type_name<DoubleImage1>() { return "DoubleImage1"; }
template<> string type_name<DoubleImage3>() { return "DoubleImage3"; }
template<> string type_name<DoubleImage4>() { return "DoubleImage4"; }

template<typename From, typename To>
void test_default_convert()
{
	From from(TypeTrait<typename TypeTrait<From>::channel_type>::domain()/2);
	To to;
	default_convert(from, to);
	cout << type_name<From>() << " (" << output(from) << ") to " 
		<< type_name<To>() << " (" << output(to) << ")" << endl;
	to = default_convert<From, To>(from);
	cout << type_name<From>() << " (" << output(from) << ") to " 
		<< type_name<To>() << " (" << output(to) << ")" << endl;
}

template<typename From>
void test_default_convert_all()
{
	test_default_convert<From, Byte1>();
	test_default_convert<From, Byte3>();
	test_default_convert<From, Byte4>();
	test_default_convert<From, Short1>();
	test_default_convert<From, Short3>();
	test_default_convert<From, Short4>();
	test_default_convert<From, Float1>();
	test_default_convert<From, Float3>();
	test_default_convert<From, Float4>();
	test_default_convert<From, Double1>();
	test_default_convert<From, Double3>();
	test_default_convert<From, Double4>();
}

template<typename From, typename To>
void test_convert()
{
	From from(2, 1);
	from.fill(TypeTrait<typename TypeTrait<From>::channel_type>::domain()/2);
	To to(2, 1);
	convert(from, to);
	cout << type_name<From>() << " {" << output(from) << "} to " 
		<< type_name<To>() << " {" << output(to) << "}" << endl;
	to = convert<From, To>(from);
	cout << type_name<From>() << " {" << output(from) << "} to " 
		<< type_name<To>() << " {" << output(to) << "}" << endl;
}

template<typename From>
void test_convert_all()
{
	test_convert<From, ByteImage1>();
	test_convert<From, ByteImage3>();
	test_convert<From, ByteImage4>();
	test_convert<From, ShortImage1>();
	test_convert<From, ShortImage3>();
	test_convert<From, ShortImage4>();
	test_convert<From, FloatImage1>();
	test_convert<From, FloatImage3>();
	test_convert<From, FloatImage4>();
	test_convert<From, DoubleImage1>();
	test_convert<From, DoubleImage3>();
	test_convert<From, DoubleImage4>();
}
int main(int argc, char *argv[])
{
	test_default_convert_all<Byte1>();
	test_default_convert_all<Byte3>();
	test_default_convert_all<Byte4>();
	test_default_convert_all<Short1>();
	test_default_convert_all<Short3>();
	test_default_convert_all<Short4>();
	test_default_convert_all<Float1>();
	test_default_convert_all<Float3>();
	test_default_convert_all<Float4>();
	test_default_convert_all<Double1>();
	test_default_convert_all<Double3>();
	test_default_convert_all<Double4>();

	test_convert_all<ByteImage1>();
	test_convert_all<ByteImage3>();
	test_convert_all<ByteImage4>();
	test_convert_all<ShortImage1>();
	test_convert_all<ShortImage3>();
	test_convert_all<ShortImage4>();
	test_convert_all<FloatImage1>();
	test_convert_all<FloatImage3>();
	test_convert_all<FloatImage4>();
	test_convert_all<DoubleImage1>();
	test_convert_all<DoubleImage3>();
	test_convert_all<DoubleImage4>();
	return 0;
}
