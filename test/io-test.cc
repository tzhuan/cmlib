#include <iostream>
#include <string>
#include "Gil.h"

using namespace std;
using namespace Gil;

template<typename Type>
void test(
    const string& testname,
    const string& src, 
    const string& filename
    );

int main(int argc, char *argv[])
{
    if (argc != 2) {
	cerr << "usage: " << argv[0] << " image-file" << endl;
	return 1;
    }
    string src = argv[1];
    test<ByteImage1>("[Byte Image, 1 channel]", src, "byte1");
    cerr << endl;
    test<ByteImage3>("[Byte Image, 3 channels]", src, "byte3");
    cerr << endl;
    test<ByteImage4>("[Byte Image, 4 channels]", src, "byte4");
    cerr << endl;
    test<FloatImage1>("[Float Image, 1 channel]", src, "float1");
    cerr << endl;
    test<FloatImage3>("[Float Image, 3 channels]", src, "float3");
    cerr << endl;
    test<FloatImage4>("[Float Image, 4 channels]", src, "float4");
    cerr << endl;
    return 0;
}

template<typename Type>
void test(
    const string& testname,
    const string& src, 
    const string& filename 
    )
{
    Type img;
    cerr << testname << endl;
    if (!img.read(src)) {
	cerr << "error: " << ": read " << src << endl;
	return;
    }
    const char*(suffix[]) = {
	"pgm", "ppm", "bmp", "tga", "png", "jpg", "tif", 
	"hdr", "flt", "pfm", "uve", "dpx"};
    const int size = sizeof(suffix)/sizeof(const char*);
    for (int i = 0; i < size; ++i) {
	string wname = filename + "." + suffix[i];
	if (img.write(wname))
	    cerr << "write " << wname << " ok" << endl;
	else
	    cerr << "error: " << ": write " << wname << endl;
    }
}
