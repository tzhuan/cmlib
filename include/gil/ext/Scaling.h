#ifndef SCALING_H
#define SCALING_H

#include <algorithm>

#include "Filterer.h"

namespace std {
	template<typename I, typename F>
	void scale(I &dst, const I &src, const F &filterer)
	{
		filter(dst, src, filterer);
	}

	template<typename I, typename F>
	I scale(const I &src, const F &filterer, size_t width, size_t height)
	{
		I dst(width, height);
		scale(dst, src, filterer);
		return dst;
	}

	template<typename I, typename T = double>
	class NearestFilterer {
		public:
			typedef typename I::value_type value_type;

			NearestFilterer(T x, T y): my_ratio_x(x), my_ratio_y(y)
			{
			}

			value_type operator()(const I &image, int x, int y) const 
			{
				int _x = static_cast<int>( x/my_ratio_x + 0.5 );
				int _y = static_cast<int>( y/my_ratio_y + 0.5 );

				_x = std::min( _x, static_cast<int>(image.width())-1 );
				_x = std::max( _x, 0 );

				_y = std::min( _y, static_cast<int>(image.height())-1 );
				_y = std::max( _y, 0 );

				return image(_x, _y);
			}
		private:
			T my_ratio_x;
			T my_ratio_y;
	};

	template<typename I, typename T = double>
	class BilinearFilterer {
		public:
			typedef typename I::value_type value_type;
			BilinearFilterer(T x, T y): my_ratio_x(x), my_ratio_y(y)
			{
			}

			value_type operator ()(const I &image, int x, int y) const
			{
				T _x = x/my_ratio_x;
				T _y = y/my_ratio_y;

				_x = std::min( _x, static_cast<T>( image.width()-1 ) );
				_x = std::max( _x, static_cast<T>(0) );

				_y = std::min( _y, static_cast<T>( image.height()-1 ) );
				_y = std::max( _y, static_cast<T>(0) );

				return image.lerp(_x, _y);
			}
		private:
			T my_ratio_x;
			T my_ratio_y;
	};
}

#endif
