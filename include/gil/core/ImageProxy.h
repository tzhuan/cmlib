#ifndef IMAGE_PROXY_H
#define IMAGE_PROXY_H

namespace gil {

	template<class Filter, class DstImage, class SrcImage>
	class ImageProxy {
		public:
			ImageProxy(const Filter& filter, const SrcImage& src):
				my_filter(filter), my_src(src)
			{
				// empty
			}

			inline DstImage& operator ()(DstImage& dst) const
			{
				my_filter(dst, my_src);
				return dst;
			}

		protected:
		private:
			const Filter& my_filter;
			const SrcImage& my_src;
	};
}

#endif
