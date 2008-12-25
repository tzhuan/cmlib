#include <exception>
#include <iostream>
#include <string>

#include "imageio/all.h"
#include "image.h"

using namespace std;
using namespace cmlib::image;

template<typename ImageType>
void read_test(const string& type_name);

template<typename ImageType>
void write_test(const string& type_name, const string& input);

const char*(suffix[]) = {
	"pgm", "ppm", "png", "jpg", "tiff", "bmp",
	"hdr", "exr", "flt", "pfm"};
const int suffix_size = sizeof(suffix)/sizeof(const char*);

const char*(type[]) = {"ByteImage", "FloatImage"};
const int type_size = sizeof(type)/sizeof(const char*);

const char*(channel[]) = {"1", "3", "4"};
const int channel_size = sizeof(channel)/sizeof(const char*);

int main(int argc, char *argv[])
{
	string input = "lena.ppm";
	if (argc == 1)
		cerr << "[use default test image: lena.ppm]" << endl;
	else if (argc == 2)
		input = argv[1];
	else {
		cerr << "usage: " << argv[0] << " image-file" << endl;
		return 1;
	}

	cerr << "begin of write test..." << endl;
	write_test<ByteImage1>("ByteImage1", input); cerr << endl;
	write_test<ByteImage3>("ByteImage3", input); cerr << endl;
	write_test<ByteImage4>("ByteImage4", input); cerr << endl;
	write_test<FloatImage1>("FloatImage1", input); cerr << endl;
	write_test<FloatImage3>("FloatImage3", input); cerr << endl;
	write_test<FloatImage4>("FloatImage4", input); cerr << endl;
	cerr << "end of write test..." << endl << endl;

	cerr << "begin of read test..." << endl;
	read_test<ByteImage1>("ByteImage1"); cerr << endl;
	read_test<ByteImage3>("ByteImage3"); cerr << endl;
	read_test<ByteImage4>("ByteImage4"); cerr << endl;
	read_test<FloatImage1>("FloatImage1"); cerr << endl;
	read_test<FloatImage3>("FloatImage3"); cerr << endl;
	read_test<FloatImage4>("FloatImage4"); cerr << endl;
	cerr << "end of read test..." << endl << endl;
	return 0;
}

template<typename ImageType>
void read_test(const string& type_name)
{
	ImageType image;
	cerr << "[test " << type_name << "]" << endl;

	for (int i = 0; i < suffix_size; ++i) {
		string input = type_name + "." + suffix[i];
		cerr << "...read from " << input << ": ";
		try {
			if (!read(image, input)) {
				cerr << "fail (fopen error)" << endl;
				continue;
			}
		} catch (exception &e) {
			cerr << "fail (" << e.what() << ")" << endl;
			continue;
		}
		cerr << "ok" << endl;

		string output = type_name + "_" + suffix[i] + ".png";
		cerr << "...write to " << output << ": ";
		try {
			if (!write(image, output)) {
				cerr << "fail (fopen error)" << endl;
				return;
			}
		} catch (exception &e) {
			cerr << "fail (" << e.what() << ")" << endl;
			return;
		}
		cerr << "ok" << endl;
	}
}

template<typename ImageType>
void write_test(const string& type_name, const string& input)
{
	ImageType image;
	cerr << "[test " << type_name << "]" << endl;
	cerr << "...read from " << input << ": ";
	try {
		if (!read(image, input)) {
			cerr << "fail (fopen error)" << endl;
			return;
		}
	} catch (exception &e) {
		cerr << "fail (" << e.what() << ")" << endl;
		return;
	}
	cerr << "ok" << endl;

	for (int i = 0; i < suffix_size; ++i) {
		string output = type_name + "." + suffix[i];
		cerr << "...write to " << output << ": ";
		try {
			if (!write(image, output)) {
				cerr << "fail (fopen error)" << endl;
				continue;
			}
		} catch (exception &e) {
			cerr << "fail (" << e.what() << ")" << endl;
			continue;
		}
		cerr << "ok" << endl;
	}
}
