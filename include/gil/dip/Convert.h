#ifndef CONVERT_H
#define CONVERT_H

#include "Filter.h"

namespace gil {

	template<class DstImage, class Converter>
	class Convert: public Filter<Convert<DstImage, Converter>, DstImage> {
		friend class Filter<Convert<DstImage, Converter>, DstImage>;
		public:
			Convert<DstImage, Converter>(): 
				Filter<Convert<DstImage, Converter>, DstImage>(*this),
				my_converter(Converter())
			{
				// empty
			}

		protected:
			template<class SrcImage>
			void filter(DstImage& dst, const SrcImage& src) const
			{
				dst.resize(src.width(), src.height());
				for (size_t y = 0; y < src.height(); ++y)
					for (size_t x = 0; x < src.width(); ++x)
						dst(x, y) = my_converter(src(x, y));
			}

		private:
			const Converter my_converter;

	};

}

#endif
