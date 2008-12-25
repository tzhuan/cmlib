#ifndef CMLIB_IMAGE_TYPE_H
#define CMLIB_IMAGE_TYPE_H

/** @file Type.h
 *  @brief This file contains the basic type definition in cmlib::image.
 *
 *  @author littleshan, tzhuan
 */

#include <cstddef>
#include <limits>

#include "TypeTrait.h"

namespace cmlib {
namespace image {

	// primitive scalar type
	typedef unsigned char Byte1;
	typedef unsigned short Short1;
	typedef float Float1;
	typedef double Double1;

	template<> 
	inline Float1 TypeTrait<Float1>::min()
	{
		return 0.0f;
	}

	template<> 
	inline Float1 TypeTrait<Float1>::max()
	{
		return 1.0f;
	}
	
	template<> 
	inline Double1 TypeTrait<Double1>::min()
	{
		return 0.0;
	}

	template<> 
	inline Double1 TypeTrait<Double1>::max()
	{
		return 1.0;
	}

	// XXX deprecated
	template <> inline Float1 TypeTrait<Float1>::opaque() 
	{
		return TypeTrait<Float1>::max();
	}

	// XXX deprecated
	template <> inline Double1 TypeTrait<Double1>::opaque()
	{ 
		return TypeTrait<Double1>::max();
	}

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_TYPE_H
