#ifndef GIL_NULL_FILTER_H
#define GIL_NULL_FILTER_H

#include "Filter.h"

namespace gil {

	template<class DstImage>
	class NullFilter: public Filter<NullFilter<DstImage>, DstImage> {
		friend class Filter<NullFilter<DstImage>, DstImage>;
		public:
			NullFilter(): Filter<NullFilter<DstImage>, DstImage>(*this)
			{
				// empty
			}
		protected:
			template<class SrcImage>
			void filter(DstImage& dst, const SrcImage& src) const
			{
				dst = src;
			}
	};

}

#endif
