#ifndef GIL_FILTERER_H 
#define GIL_FILTERER_H 

#include <cstddef>

#include "../gil.h"

namespace gil {

	template<class RealFilter, class DstImage>
	class Filter {
		public:
			Filter(RealFilter& filter): my_real_filter(filter)
			{
				// empty
			}

			template<class SrcImage>
			inline void operator ()(DstImage& dst, const SrcImage& src) const
			{
				my_real_filter.filter(dst, src);
			}

			template<
				class ProxyFilter, class ProxyDstImage, class ProxySrcImage
			>
			inline void operator ()(
				DstImage& dst,
				const ImageProxy<ProxyFilter, ProxyDstImage, ProxySrcImage>&
				src_proxy
			) const
			{
				ProxyDstImage tmp;
				(*this)(dst, src_proxy(tmp));
			}

			template<class SrcImage>
			inline ImageProxy<RealFilter, DstImage, SrcImage>
			operator ()(const SrcImage& src) const
			{
				return 
					ImageProxy< RealFilter, DstImage, SrcImage >
					(my_real_filter, src);
			}

		protected:

		private:
			RealFilter& my_real_filter;	
	};

}

#endif
