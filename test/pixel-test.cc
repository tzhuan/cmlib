#include <iostream>
#include <string>
#include "Gil.h"

using namespace std;
using namespace Gil;

template<typename Type, typename IType, typename PType>
void test();

int main(int argc, char *argv[])
{
    test<unsigned char, ByteImage1, BytePixel1>();
    test<unsigned char, ByteImage3, BytePixel3>();
    test<unsigned char, ByteImage4, BytePixel4>();
    test<float, FloatImage1, FloatPixel1>();
    test<float, FloatImage3, FloatPixel3>();
    test<float, FloatImage4, FloatPixel4>();
    return 0;
}

template<typename Type, typename IType, typename PType>
void test()
{
    IType img(200, 100);
    PType pixel(static_cast<Type>(0));
    for (int h = 0; h < img.height(); ++h)
	for (int w = 0; w < img.width(); ++w)
	    img[w][h] = pixel;
}
