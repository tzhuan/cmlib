#ifndef FILTERER_H 
#define FILTERER_H 

#include <cstddef>

namespace gil {

	struct TrueType {};
	struct FalseType {};

	template<typename T>
	struct FiltererTrait
	{
		typedef FalseType Separable;
	};

	template<typename I>
	struct NullFilterer1 {
		typename I::value_type operator()(const I &image, int x, int y) const 
		{
			return image(x, y);
		}
	};

	template<typename I>
	struct NullFilterer2 {
		template<typename T>
		struct NullFilterX {
			typename T::value_type
			operator()(const T &image, size_t x, size_t y) const
			{
				return image(x, y);
			}
		};

		template<typename T>
		struct NullFilterY {
			typename T::value_type 
			operator()(const T &image, size_t x, size_t y) const
			{
				return image(x, y);
			}
		};

		NullFilterX<I> x() const
		{
			return NullFilterX<I>();
		}

		NullFilterY<I> y() const
		{
			return NullFilterY<I>();
		}
	};
	template<typename I>
	struct FiltererTrait< NullFilterer2<I> >
	{
		typedef TrueType Separable;
	};

	template<typename I, typename F>
	void filter(I &dst, const I &src, const F filterer, int)
	{
		for (size_t h = 0; h < dst.height(); ++h)
			for (size_t w = 0; w < dst.width(); ++w)
				dst(w, h) = filterer(src, w, h);
	}

	template<typename I, typename F>
	void filter(I &dst, const I &src, const F filterer, const TrueType&)
	{
		filter(dst, src, filterer.x(), 0);
		filter(dst, src, filterer.y(), 0);
	}

	template<typename I, typename F>
	void filter(I &dst, const I &src, const F filterer, const FalseType&)
	{
		filter(dst, src, filterer, 0);
	}

	template<typename I, typename F>
	void filter(I &dst, const I &src, const F filterer)
	{
		filter(dst, src, filterer, typename FiltererTrait<F>::Separable () );
	}
}

#endif
