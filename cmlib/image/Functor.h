/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_IMAGE_FUNCTOR_H
#define CMLIB_IMAGE_FUNCTOR_H

#include <cstddef>
#include <cmlib/image/ImageProxy.h>

namespace cmlib {
namespace image {

	template<class RealFunctor, class DstImage>
	class Functor {
		public:
			Functor(RealFunctor& functor): my_real_functor(functor)
			{
				// empty
			}

			template<class SrcImage>
			inline void operator ()(DstImage& dst, const SrcImage& src) const
			{
				my_real_functor.function(dst, src);
			}

			template<
				class ProxyFunctor, class ProxyDstImage, class ProxySrcImage
			>
			inline void operator ()(
				DstImage& dst,
				const ImageProxy<ProxyFunctor, ProxyDstImage, ProxySrcImage>&
				src_proxy
			) const
			{
				ProxyDstImage tmp;
				(*this)(dst, src_proxy(tmp));
			}

			template<class SrcImage>
			inline ImageProxy<RealFunctor, DstImage, SrcImage>
			operator ()(const SrcImage& src) const
			{
				return 
					ImageProxy< RealFunctor, DstImage, SrcImage >
					(my_real_functor, src);
			}

		protected:

		private:
			RealFunctor& my_real_functor;	
	};
} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_FUNCTOR_H
