#ifndef SCALING_H
#define SCALING_H

#include "Filterer.h"

namespace std {
	template<typename I, typename F>
	void scale(I &dst, const I &src, const F &filter)
	{
		filter(dst, src, filter);
	}

	template<typename I, typename F>
	I scale(const I &src, const F &filter, size_t width, size_t height)
	{
		I dst(width, height);
		scale(dst, src, filter);
		return dst;
	}
}

#endif
