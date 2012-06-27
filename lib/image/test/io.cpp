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
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>

using namespace std;
using namespace cmlib::image;

template<typename ImageType>
void read_test(const char *type_name, const char *dir);

template<typename ImageType>
void write_test(const char *type_name, const char *input, const char *dir);

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
	// string input = "lena.ppm";
	if (argc != 3) {
		cerr << "usage: " << argv[0] << " test-image test-dir" << endl;
		return 1;
	}

	const char *input = argv[1];
	const char *dir = argv[2];

	cerr << "begin of write() test..." << endl;
	write_test<ByteImage1>("ByteImage1", input, dir); cerr << endl;
	write_test<ByteImage3>("ByteImage3", input, dir); cerr << endl;
	write_test<ByteImage4>("ByteImage4", input, dir); cerr << endl;
	write_test<FloatImage1>("FloatImage1", input, dir); cerr << endl;
	write_test<FloatImage3>("FloatImage3", input, dir); cerr << endl;
	write_test<FloatImage4>("FloatImage4", input, dir); cerr << endl;
	cerr << "end of write() test..." << endl << endl;

	cerr << "begin of read() test..." << endl;
	read_test<ByteImage1>("ByteImage1", dir); cerr << endl;
	read_test<ByteImage3>("ByteImage3", dir); cerr << endl;
	read_test<ByteImage4>("ByteImage4", dir); cerr << endl;
	read_test<FloatImage1>("FloatImage1", dir); cerr << endl;
	read_test<FloatImage3>("FloatImage3", dir); cerr << endl;
	read_test<FloatImage4>("FloatImage4", dir); cerr << endl;
	cerr << "end of read() test..." << endl << endl;
	return 0;
}

template<typename ImageType>
void read_test(const char *type_name, const char *dir)
{
	ImageType image;
	cerr << "type: " << type_name << endl;

	for (int i = 0; i < suffix_size; ++i) {
		string input = string(dir) + "/" + type_name + "." + suffix[i];
		cerr << "\tread from " << input << ": ";
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

		string output = string(dir) + "/" + type_name + "_" + suffix[i] + ".png";
		cerr << "\twrite to " << output << ": ";
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
void write_test(const char *type_name, const char *input, const char *dir)
{
	ImageType image;
	cerr << "type: " << type_name << endl;
	cerr << "\tread from " << input << ": ";
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
		string output = string(dir) + "/" + type_name + "." + suffix[i];
		cerr << "\twrite to " << output << ": ";
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
