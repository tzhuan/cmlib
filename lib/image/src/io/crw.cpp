/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <cstdio>
#include <cassert>

#include <cmlib/imageio/Exception.h>
#include <cmlib/imageio/io/crw.h>

extern "C" {
extern int ext_identify(FILE *f);
extern void ext_init(size_t *w, size_t *h, size_t *m);
extern void ext_read(unsigned short(*row_pointers)[4]);
}

using namespace cmlib::image;

namespace cmlib {
namespace image {
	void CrwReader::check(FILE *f)
	{
		assert(f);
		if (!ext_identify(f))
		throw InvalidFormat("unsupported format in CrwReader::check()");
	}

	void CrwReader::init()
	{
		ext_init(&my_width, &my_width, &my_meta_length);
	}

	void CrwReader::read(unsigned short (*row_pointers)[4])
	{
		ext_read(row_pointers);
	}

	void CrwReader::finish()
	{
	}
} // namespace image
} // namespace cmlib
