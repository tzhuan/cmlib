#ifndef FILTERER_H 
#define FILTERER_H 

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
			void operator ()(DstImage& dst, const SrcImage& src) const
			{
				my_real_filter.filter(dst, src);
			}

			template<
				class ProxyFilter, class ProxyDstImage, class ProxySrcImage
			>
			void operator ()(
				DstImage& dst,
				const ImageProxy<ProxyFilter, ProxyDstImage, ProxySrcImage>&
				src_proxy
			) const
			{
				ProxyDstImage tmp;
				return (*this)(dst, src_proxy(tmp));
			}

			template<class SrcImage>
			ImageProxy<RealFilter, DstImage, SrcImage>
			operator ()(const SrcImage& src) const
			{
				return 
					ImageProxy< RealFilter, DstImage, SrcImage >
					(my_real_filter, src);
			}

			template<
				class ProxyFilter, class ProxyDstImage, class ProxySrcImage
			>
			ImageProxy<RealFilter, DstImage, ProxyDstImage>
			operator ()(
				const ImageProxy<ProxyFilter, ProxyDstImage, ProxySrcImage>&
				src_proxy
			) const
			{
				ProxyDstImage tmp;
				return 
					ImageProxy< RealFilter, DstImage, ProxyDstImage >
					( my_real_filter, src_proxy(tmp) );
			}

		protected:

		private:
			RealFilter& my_real_filter;	
	};

}

#endif
