#ifndef FILTERER_H 
#define FILTERER_H 

#include <cstddef>

#include "../gil.h"

namespace gil {
	/*
	struct TrueType {};
	struct FalseType {};

	template<typename T>
	struct FiltererTrait
	{
		typedef FalseType Separable;
	};


	template<typename To, typename From, typename F>
	inline 
	void 
	filter(To &dst, const From &src, const F &filterer, const TrueType&)
	{
		To tmp(dst);
		filter(tmp, src, filterer.x(), true);
		filter(dst, tmp, filterer.y(), true);
	}

	template<typename To, typename From, typename F>
	inline 
	void 
	filter(To &dst, const From &src, const F &filterer, const FalseType&)
	{
		filter(dst, src, filterer, true);
	}
	*/

	template<typename To, typename From, typename F>
	inline void filter_impl(To &dst, const From &src, const F &filterer)
	{
		for (size_t h = 0; h < dst.height(); ++h)
			for (size_t w = 0; w < dst.width(); ++w)
				dst(w, h) = filterer(src, w, h);
	}

	template<typename To, typename From, typename F>
	inline 
	void 
	filter_(To &dst, const From &src, const F &filterer)
	{
		if (F::separable()) {
			To tmp(dst);
			filter_impl(tmp, src, filterer.x());
			filter_impl(dst, tmp, filterer.y());
		} else {
			filter_impl(dst, src, filterer);
		}
	}

	template<typename To, typename From, typename F>
	inline 
	void 
	filter(To &dst, const From &src, const F &filterer)
	{
		filter_(dst, src, filterer);
	}

	template<typename To, typename From, typename F>
	inline
	const 
	ImageProxy<
		void (*)(To&, const From&, const F&, size_t, size_t),
		To,
		From,
		F,
		size_t,
		size_t
	>
	filter(const From &src, const F &filterer, size_t width, size_t height)
	{
		return 
			ImageProxy<
				void (*)(To&, const From&, const F&, size_t, size_t),
				To,
				From,
				F,
				size_t,
				size_t
			>(
				&(filter_<To, From, F>),
				src, 
				filterer, 
				width, 
				height
			);
	}

	template<typename I, typename F>
	inline
	const 
	ImageProxy<
		void (*)(I&, const I&, const F&, size_t, size_t),
		I,
		I,
		F,
		size_t,
		size_t
	>
	filter(const I &src, const F &filterer, size_t width, size_t height)
	{
		return
			ImageProxy<
				void (*)(I&, const I&, const F&, size_t, size_t),
				I,
				I,
				F,
				size_t,
				size_t
			>(
				&(filter_<I, I, F>),
				src, 
				filterer, 
				width, 
				height
			);
	}

	template<typename To, typename From, typename F>
	inline
	const
	ImageProxy<
		void (*)(To&, const From&, const F&, size_t, size_t),
		To,
		From,
		F,
		size_t,
		size_t
	>
	filter(const From &src, const F &filterer)
	{
		return filter<To, From, F>( src, filterer, src.width(), src.height() );
	}

	template<typename I, typename F>
	inline 
	const 
	ImageProxy<
		void (*)(I&, const I&, const F&, size_t, size_t),
		I,
		I,
		F,
		size_t,
		size_t
	>
	filter(const I &src, const F &filterer)
	{
		return filter<I, F>( src, filterer, src.width(), src.height() );
	}
}

#endif
