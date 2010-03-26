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

	ByteImage3 img;
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

	for (int i = 0; i < 10; ++i) {
		vfs >> img;
		if (!vfs) {
			cerr << "fail, i: " << i << endl;
			vfs.clear();
		}
		stringstream ss;
		ss << "test-" << i << ".png";
		write(img, ss.str());
	}

	return 0;
}
