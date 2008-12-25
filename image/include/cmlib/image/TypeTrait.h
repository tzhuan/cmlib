#ifndef CMLIB_IMAGE_TYPETRAIT_H
#define CMLIB_IMAGE_TYPETRAIT_H

/** @file TypeTrait.h
 *  @brief This file contains the basic TypeTrait in cmlib::image. 
 *
 *  @author littleshan, tzhuan
 */

#include <cstddef>
#include <limits>

// set DLLAPI if we're using VC
#ifdef _MSC_VER
	#pragma warning(disable: 4996)

	#ifdef BUILD_IMAGE_DLL
		#define DLLAPI __declspec(dllexport)
	#elif defined(LINK_IMAGE_DLL)
		#define DLLAPI __declspec(dllimport)
	#else
		#define DLLAPI
	#endif // BUILD_IMAGE_DLL
#else
	#define DLLAPI
#endif // _MSC_VER

namespace cmlib {
namespace image {

	/** @brief clamp the value to be between lower and upper
	 *
	 *  @param value the value to be clamp
	 *  @param lower minimum value
	 *  @param upper maximum value
	 */
	template <typename T>
	T clamp(T value, T lower, T upper){
		return std::min( std::max(value, lower), upper );
	}

	/** @class TypeTrait
	 *  @brief TypeTrait is used to get the type properties.
	 *
	 *  @tparam T the type to be get properties
	 */
	template <typename T>
	class TypeTrait {
	public:
		typedef float ExtendedType; ///< for pixel value arithmetic
		typedef float MathType; ///< for pixel value arithmetic
		typedef float OutputType; ///< for pixel value output

		/** @brief min value
		 */
		static T min()
		{
			return std::numeric_limits<T>::min();
		}

		/** @brief max value
		 */
		static T max()
		{
			return std::numeric_limits<T>::max();
		}

		/** @brief domain of the Type, max() - min()
		 */
		static T domain()
		{
			return max() - min();
		}

		static T zero() 
		{ 
			return 0; 
		}

		// XXX deprecated
		static T transparent() 
		{ 
			return zero(); 
		}

		// XXX deprecated
		static T opaque() 
		{
			return max();
		}

		static T mix(T a, T b, T w)
		{
			return ( a * (max()-w) + b * (w-min()) ) / domain();
		}
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_TYPETRAIT_H
