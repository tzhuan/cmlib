#ifndef __SCALING_H__
#define __SCALING_H__

namespace std {
	template<typename I, typename F>
	void scale(I &dst, const I &src, const F &filter)
	{
	}

	template<typename I, typename F>
	I scale(const I &src, const F &filter, int width, int height)
	{
		I dst(width, height);
		scale(dst, src, filter);
		return dst;
	}
}

#endif
