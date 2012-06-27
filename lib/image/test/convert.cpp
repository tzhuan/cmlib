/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <iostream>
#include <stdexcept>

#include <cmlib/image.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>

using namespace std;
using namespace cmlib;
using namespace cmlib::image;

int main(int argc, char **argv)
{
	if (argc != 3) {
		cerr << "usage: " << argv[0] << " [input-file] [output-file]" << endl;
		return 1;
	}

	const char *fin = argv[1];
	const char *fout = argv[2];

	try {
		FloatImage4 image;

		if (!read(image, fin)) {
			cerr << "read error" << endl;
			return 1;
		}
		if (!write(image, fout)) {
			cerr << "write error" << endl;
			return 1;
		}
	} catch (exception &e) {
		cerr << e.what() << endl;
		return 1;
	} catch ( ... ) {
		cerr << "unkown error " << endl;
		return 2;
	}

	return 0;
}
