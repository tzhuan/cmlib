#include <exception>
#include <iostream>
#include <string>

#include <cmlib/image.h>
#include <cmlib/dip.h>

using namespace std;
using namespace cmlib::image;
using namespace cmlib::dip;

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

template<typename Type>
void test_colorspace1()
{
	Type src(10, 10), dst(10, 10);
	cout << type_name<Type>() << endl;
	rgb_to_xyz(src, dst);
	dst = rgb_to_xyz<Type, Type>(src);

	rgb_to_ycbcr(src, dst);
	dst = rgb_to_ycbcr<Type, Type>(src);

	rgb_to_hsv(src, dst);
	dst = rgb_to_hsv<Type, Type>(src);

	hsv_to_rgb(src, dst);
	dst = hsv_to_rgb<Type, Type>(src);

	rgb_to_hsl(src, dst);
	dst = rgb_to_hsl<Type, Type>(src);

	rgb_to_lab(src, dst);
	dst = rgb_to_lab<Type, Type>(src);

	rgb_to_luv(src, dst);
	dst = rgb_to_luv<Type, Type>(src);

	xyz_to_rgb(src, dst);
	dst = xyz_to_rgb<Type, Type>(src);

	ycbcr_to_rgb(src, dst);
	dst = ycbcr_to_rgb<Type, Type>(src);
}

template<typename Src, typename Dst>
void test_colorspace2()
{
	Src src(10, 10);
	Dst dst(10, 10);
	cout << type_name<Src>() << " to " << type_name<Dst>() << endl;
	rgb_to_gray(src, dst);
	dst = rgb_to_gray<Src, Dst>(src);
}

int main(int argc, char *argv[])
{
	test_colorspace1<ByteImage3>();
	test_colorspace1<ByteImage4>();
	test_colorspace1<ShortImage3>();
	test_colorspace1<ShortImage4>();
	test_colorspace1<FloatImage3>();
	test_colorspace1<FloatImage4>();
	test_colorspace1<DoubleImage3>();
	test_colorspace1<DoubleImage4>();

	test_colorspace2<ByteImage3, ByteImage1>();
	test_colorspace2<ByteImage4, ByteImage1>();
	test_colorspace2<ShortImage3, ShortImage1>();
	test_colorspace2<ShortImage4, ShortImage1>();
	test_colorspace2<FloatImage3, FloatImage1>();
	test_colorspace2<FloatImage4, FloatImage1>();
	test_colorspace2<DoubleImage3, DoubleImage1>();
	test_colorspace2<DoubleImage4, DoubleImage1>();
	return 0;
}
