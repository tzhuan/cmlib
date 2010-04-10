#ifndef CMLIB_IMAGE_TYPE_H
#define CMLIB_IMAGE_TYPE_H

/** @file Type.h
 *  @brief This file contains the basic type definition in cmlib::image.
 *
 *  @author littleshan, tzhuan
 */

#include <cstddef>
#include <limits>

#include <cmlib/image/TypeTrait.h>

namespace cmlib {
namespace image {

	// primitive scalar type
	typedef unsigned char Byte1;
	typedef unsigned short Short1;
	typedef float Float1;
	typedef double Double1;

	// forward definitions
	template <typename Type, size_t Channel> class Color;
	template<typename, template<typename> class> class Image;
	template <typename ImageType> class SubImage;
	template <typename ImageType> class SliceImage;

	/** @class TypeTrait<Float1>
	 *  @brief Float1 specialization of TypeTrait
	 */
	template <>
	class TypeTrait<Float1> { // {{{
	public:
		typedef Float1 type;
		typedef Float1 channel_type;
		typedef Float1 color_type;
		typedef float extended_type; ///< for pixel value arithmetic
		typedef float math_type; ///< for pixel value arithmetic
		typedef float output_type; ///< for pixel value output
		typedef FloatingPointTag type_category;

		// XXX deprecated
		typedef float ExtendedType;
		typedef float MathType;
		typedef float OutputType;

		static type min()
		{
			return 0.0f;
		}

		static type max()
		{
			return 1.0f;
		}

		static type domain()
		{
			return max() - min();
		}

		static type zero() 
		{ 
			return 0.0f; 
		}

		// XXX deprecated
		static type transparent() 
		{ 
			return zero(); 
		}

		// XXX deprecated
		static type opaque() 
		{
			return max();
		}

		static type mix(type a, type b, type w)
		{
			return ( a * (max()-w) + b * (w-min()) ) / domain();
		}
	}; // }}}

	/** @class TypeTrait<Double1>
	 *  @brief Double1 specialization of TypeTrait
	 */
	template <>
	class TypeTrait<Double1> { // {{{
	public:
		typedef Double1 type;
		typedef Double1 channel_type;
		typedef Double1 color_type;
		typedef double extended_type; ///< for pixel value arithmetic
		typedef double math_type; ///< for pixel value arithmetic
		typedef double output_type; ///< for pixel value output
		typedef FloatingPointTag type_category;

		// XXX deprecated
		typedef double ExtendedType;
		typedef double MathType;
		typedef double OutputType;

		static type min()
		{
			return 0.0;
		}

		static type max()
		{
			return 1.0;
		}

		static type domain()
		{
			return max() - min();
		}

		static type zero() 
		{ 
			return 0.0; 
		}

		// XXX deprecated
		static type transparent() 
		{ 
			return zero(); 
		}

		// XXX deprecated
		static type opaque() 
		{
			return max();
		}

		static type mix(type a, type b, type w)
		{
			return ( a * (max()-w) + b * (w-min()) ) / domain();
		}
	}; // }}}

	/** @class TypeTrait< Color<Type, Channel> >
	 *  @brief Color<Type, Channel> specialization of TypeTrait
	 */
	template <typename Type, size_t Channel>
	class TypeTrait< Color<Type, Channel> > { // {{{
	public:
		typedef Color<Type, Channel> type;
		typedef Type channel_type;
		typedef Color<Type, Channel> color_type;
		typedef ColorTag type_category;
	}; // }}}

	/** @class TypeTrait< Image<Color, Allocator> >
	 *  @brief Image<Color, Allocator> specialization of TypeTrait
	 */
	template <typename ColorType, template<typename> class Allocator>
	class TypeTrait< Image<ColorType, Allocator> > { // {{{
	public:
		typedef Image<ColorType, Allocator> type;
		typedef typename TypeTrait<ColorType>::channel_type channel_type;
		typedef ColorType color_type;
		typedef ImageTag type_category;
	}; // }}}

	/** @class TypeTrait< SubImage<Image> >
	 *  @brief SubImage<Image> specialization of TypeTrait
	 */
	template <typename ImageType>
	class TypeTrait< SubImage<ImageType> > { // {{{
	public:
		typedef SubImage<ImageType> type;
		typedef typename TypeTrait<ImageType>::channel_type channel_type;
		typedef typename TypeTrait<ImageType>::color_type color_type;
		typedef ImageTag type_category;
	}; // }}}

	/** @class TypeTrait< SliceImage<Image> >
	 *  @brief SliceImage<Image> specialization of TypeTrait
	 */
	template <typename ImageType>
	class TypeTrait< SliceImage<ImageType> > { // {{{
	public:
		typedef SliceImage<ImageType> type;
		typedef typename TypeTrait<ImageType>::channel_type channel_type;
		typedef typename TypeTrait<ImageType>::color_type color_type;
		typedef ImageTag type_category;
	}; // }}}

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_TYPE_H
