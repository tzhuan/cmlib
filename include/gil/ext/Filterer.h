#ifndef FILTERER_H 
#define FILTERER_H 

namespace gil {

	struct TrueType {};
	struct FalseType {};

	template<typename T>
	struct FiltererTrait
	{
		typedef FalseType Separable;
	}

	template<typename T, typename R>
	struct Filterer {
		T operator()(const Image<T> &image, int x, int y) const ;
		R x();
		R y();
		bool separable() const;
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
		filter(dst, src, filterer, FilterTrait<F>::Separable() );
	}

}

#endif
