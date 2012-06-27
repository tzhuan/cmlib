/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <iostream>
#include <sstream>

#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>
#include <cmlib/image.h>
#include <cmlib/video.h>

using namespace std;
using namespace cmlib::image;
using namespace cmlib::video;

int
main(int argc, char **argv)
{
	if (argc != 2) {
		cerr << "usage: " << argv[0] << " video-file" << endl;
		return 1;
	}

	VideoFileStream vfs(argv[1]);

	if (!vfs) {
		cerr << "open file fail" << endl;
		return 1;
	}

	// vfs.info(cerr);

	ByteImage4 img;
	/*
	while (true) {
		vfs >> img;
		if (vfs.is_keyframe()) {
			cerr << vfs.tell_index() << endl;
		}
		if (vfs.eof())
			break;
		else if (!vfs)
			vfs.clear();
	}
	*/

	// vfs.seekg(130);
	if (!vfs) {
		cerr << "seek fail" << endl;
		return 1;
	}

	for (int i = 0; i < 20; ++i) {
		cerr << "decode frame " << i << " ... ";
		vfs >> img;
		if (!vfs) {
			cerr << "fail." << endl;
			vfs.clear();
			continue;
		}
		stringstream ss;
		ss << "test-" << i << ".png";
		cerr << " write to " << ss.str() << " ... ";
		write(img, ss.str());
		cerr << "done." << endl;
	}

	return 0;
}
