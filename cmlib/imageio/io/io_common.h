/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_IMAGE_IO_COMMON_H
#define CMLIB_IMAGE_IO_COMMON_H

namespace cmlib {
namespace image {

	template<typename T, bool = false>
	struct ByteReverser {
		static bool is_little_endian()
		{
			int i = 1;
			return ( *((char*)&i) == 1 );
		}

		void operator()(T&) {}
	};

	template<typename T>
	struct ByteReverser <T, true> {
		void operator()(T &data)
		{
			char *cp = (char*)&data;
			switch(sizeof(T)) {
				case 1:
					return;
				case 2:
					std::swap(cp[0], cp[1]);
					return;
				case 4:
					std::swap(cp[0], cp[3]);
					std::swap(cp[1], cp[2]);
					return;
				case 8:
					std::swap(cp[0], cp[7]);
					std::swap(cp[1], cp[6]);
					std::swap(cp[2], cp[5]);
					std::swap(cp[3], cp[4]);
					return;
				default:
					for (size_t i = 0; i < sizeof(T)/2; ++i)
						std::swap(cp[i], cp[sizeof(T)-i-1]);
					return;
			}
		}
	};

} // namesapce image
} // namespace cmlib

#endif // CMLIB_IMAGEIO_COMMON_H
