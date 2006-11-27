#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <Filterer.h>

namespace gil {

	template<typename I, typename K>
	void convolute(I &dst, const I &src, const K kernel)
	{
		filter(dst, src, kernel);
	}

	template<typename I, typename K>
	I convolute(const I &src, const K kernel)
	{
		I dst(src.width(), src.height());
		convolute(dst, src, kernel);
		return dst;
	}
}

#endif
