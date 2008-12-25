#include <iostream>
#include <stdexcept>

#include "imageio/all.h"
#include "imageio.h"
#include "image.h"

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
