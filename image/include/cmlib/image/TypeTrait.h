#ifndef CMLIB_IMAGE_TYPETRAIT_H
#define CMLIB_IMAGE_TYPETRAIT_H

/** @file TypeTrait.h
 *  @brief This file contains the basic TypeTrait in cmlib::image. 
 *
 *  @author littleshan, tzhuan
 */

#include <cstddef>
#include <limits>

#include <cmlib/common.h>

namespace cmlib {
namespace image {

	//@{
	/**
	 *  @defgroup type_tags Type Tags
	 *  These are empty types, used to distinguish different types.
	 */
	/// Marking integer type.
	struct IntegerTag {};
	/// Marking	floating point type.
	struct FloatingPointTag {};
	/// Marking Color type.
	struct ColorTag {};
	/// Marking Image type.
	struct ImageTag {};
	//@}

	/** @brief clamp the value to be between lower and upper
	 *
	 *  @param value the value to be clamp
	 *  @param lower minimum value
	 *  @param upper maximum value
	 */
	template <typename Type>
	Type clamp(Type value, Type lower, Type upper){
		return std::min( std::max(value, lower), upper );
	}

	/** @class TypeTrait
	 *  @brief TypeTrait is used to get the type properties.
	 *
	 *  @tparam T the type to be get properties
	 */
	template <typename Type>
	class TypeTrait { // {{{
	public:
		typedef Type type;
		typedef Type channel_type;
		typedef Type color_type;
		typedef float extended_type; ///< for pixel value arithmetic
		typedef float math_type; ///< for pixel value arithmetic
		typedef float output_type; ///< for pixel value output
		typedef IntegerTag type_category;

		// XXX deprecated
		typedef float ExtendedType; ///< for pixel value arithmetic
		typedef float MathType; ///< for pixel value arithmetic
		typedef float OutputType; ///< for pixel value output

		/** @brief min value
		 */
		static Type min()
		{
			return std::numeric_limits<Type>::min();
		}

		/** @brief max value
		 */
		static Type max()
		{
			return std::numeric_limits<Type>::max();
		}

		/** @brief domain of the Type, max() - min()
		 */
		static Type domain()
		{
			return max() - min();
		}

		static Type zero() 
		{ 
			return 0; 
		}

		// XXX deprecated
		static Type transparent() 
		{ 
			return zero(); 
		}

		// XXX deprecated
		static Type opaque() 
		{
			return max();
		}

		static Type mix(Type a, Type b, Type w)
		{
			return ( a * (max()-w) + b * (w-min()) ) / domain();
		}
	}; // }}}

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_TYPETRAIT_H
