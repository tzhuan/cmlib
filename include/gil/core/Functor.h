#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <cstddef>
#include "ImageProxy.h"

namespace gil {

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
}

#endif
