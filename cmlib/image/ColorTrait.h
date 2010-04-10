#ifndef CMLIB_IMAGE_COLORTRAIT_H
#define CMLIB_IMAGE_COLORTRAIT_H

/** @file ColorTrait.h
 *  @brief This file contains the basic type definition and TypeTrait
 *  in cmlib::image. 
 *
 *  @author littleshan, tzhuan
 */

#include "TypeTrait.h"

namespace cmlib {
namespace image {

	template <typename T>
	struct ColorTrait {
		typedef T BaseType;
		typedef typename TypeTrait<T>::ExtendedType ExtendedColor;
		static size_t channels() { return 1; }
		static T& select_channel(T& color, size_t) 
		{ 
			return color; 
		}
		static const T& select_channel(const T& color, size_t) 
		{ 
			return color;
		}
		enum { Channels = 1 };
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_COLORTRAIT_H
