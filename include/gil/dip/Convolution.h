#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <stdexcept>
#include <vector>
#include <cassert>
#include <cmath>

#include "Filterer.h"
#include "../dip.h"

namespace gil {

	template<typename I, typename K>
	void convolute(I &dst, const I &src, const K kernel)
	{
		filter(dst, src, kernel);
	}

	template<typename I, typename K>
	const I convolute(const I &src, const K kernel)
	{
		I dst(src.width(), src.height());
		convolute(dst, src, kernel);
		return dst;
	}

	template<typename I> struct BindX { 
		static int offset(size_t x, size_t, int i) 
		{
			return static_cast<int>(x) + i;
		}

		static int size(const I &image)
		{
			return image.width();
		}

		static const typename I::value_type 
		color(const I &image, size_t, size_t y, size_t i)
		{
			return image(i, y);
		}
	}; 

	template<typename I> struct BindY { 
		static int offset(size_t, size_t y, int i)
		{
			return static_cast<int>(y) + i;
		}

		static int size(const I &image)
		{
			return image.height();
		}

		static const typename I::value_type 
		color(const I &image, size_t x, size_t, size_t i)
		{
			return image(x, i);
		}
	}; 

	template<typename I>
	class Kernel { 
		public:
			typedef 
				typename TypeTrait<
					typename ColorTrait<
						typename I::value_type
					>::BaseType
				>::MathType value_type;

			Kernel(size_t x, size_t y)
				: my_radius_x(x/2), my_radius_y(y/2), my_kernel(y), my_data(x*y)
			{
				my_kernel[0] = my_data.begin();
				for (size_t i = 1; i < y; ++i)
					my_kernel[i] = my_kernel[i-1] + x;
			}

			const typename I::value_type 
			operator()(const I &image, size_t x, size_t y) const
			{
				value_type num = 0;
				typename I::value_type sum = 0;

				for (int h = -my_radius_y; h <= my_radius_y; ++h) {
					const int cur_y = static_cast<int>(y) + h;
					if (cur_y < 0)
						continue;
					else if (cur_y >= static_cast<int>(image.height()))
						break;
					for (int w = -my_radius_x; w <= my_radius_x; ++w) {
						const int cur_x = static_cast<int>(x) + w;
						if (cur_x < 0)
							continue;
						else if (cur_x >= static_cast<int>(image.width()))
							break;
						sum += (*this)(w, h) * image(cur_x, cur_y);
						num += (*this)(w, h);
					}
				}

				assert(num);
				return sum / num;
			}

			value_type& operator()(int x, int y)
			{
				return my_kernel[my_radius_y + y][my_radius_x + x];
			}

			const value_type& operator()(int x, int y) const
			{
				return my_kernel[my_radius_y + y][my_radius_x + x];
			}

			size_t height() const
			{
				return my_kernel.size();
			}

			size_t width() const
			{
				return (height() ? my_data.size()/my_kernel.size() : 0);
			}
		protected:
			size_t my_radius_x;
			size_t my_radius_y;
			std::vector< typename std::vector<value_type>::iterator > my_kernel;
			std::vector<value_type> my_data;
	}; 

	template<typename I, typename B>
	class SubKernel { 
		public:
			typedef 
				typename TypeTrait<
					typename ColorTrait<
						typename I::value_type
					>::BaseType
				>::MathType value_type;

			SubKernel (size_t size)
				: my_radius(size/2), my_kernel(size)
			{
			}

			const typename I::value_type 
			operator()(const I &image, size_t x, size_t y) const
			{
				value_type num = 0;
				typename I::value_type sum = 0;

				for (int i = -static_cast<int>(this->my_radius); 
					i <= static_cast<int>(this->my_radius); ++i) {
					int cur = B::offset(x, y, i);
					if ( cur < 0 ) 
						continue;
					if ( cur >= B::size(image) ) 
						break;

					sum += (*this)(i) * B::color(image, x, y, cur);
					num += (*this)(i);
				}

				assert(num);
				return sum / num;
			}

			value_type& operator()(int i)
			{
				return my_kernel[my_radius + i];
			}

			const value_type& operator()(int i) const
			{
				return my_kernel[my_radius + i];
			}

			size_t size() const
			{
				return my_kernel.size();
			}
		protected:
			size_t my_radius;
			std::vector<value_type> my_kernel;
	}; 

	template<typename I, typename Kx, typename Ky>
	class Kernel2 { 
		public:
			Kernel2(const Kx &kx, const Ky &ky)
				: my_x_kernel(kx), my_y_kernel(ky)
			{
			}

			const Kx& x() const
			{
				return my_x_kernel;
			}

			const Ky& y() const
			{
				return my_y_kernel;
			}
		protected:
			Kx my_x_kernel;
			Ky my_y_kernel;
	}; 

	template<typename I, typename B>
	class NullSubKernel: public SubKernel<I, B> {
		public:
			typedef 
				typename TypeTrait<
					typename ColorTrait<
						typename I::value_type
					>::BaseType
				>::MathType value_type;

			NullSubKernel(): SubKernel<I, B>( static_cast<value_type>(1) )
			{
				(*this)(0) = static_cast<value_type>(1);
			}
	};

	template<typename I>
	class NullKernel: public Kernel2<
				 I, NullSubKernel<I, BindX<I> >, NullSubKernel<I, BindY<I> > 
			> {
		public:
			NullKernel()
				: Kernel2<
					  I, 
					  NullSubKernel<I, BindX<I> >, 
					  NullSubKernel<I, BindY<I> >
				>(
					NullSubKernel< I, BindX<I> > (),
					NullSubKernel< I, BindY<I> > ()
				)
			{
			}
	};

	template<typename I>
	struct FiltererTrait< NullKernel<I> >
	{
		typedef TrueType Separable;
	};

	template<typename I, typename B>
	class BoxSubKernel: public SubKernel<I, B> {
		public:
			typedef 
				typename TypeTrait<
					typename ColorTrait<
						typename I::value_type
					>::BaseType
				>::MathType value_type;

			BoxSubKernel(size_t size): SubKernel<I, B>(size)
			{
				if (size % 2 == 0) {
					throw std::runtime_error(
						"the kernel size should be odd number."
					);
				}

				value_type weight = static_cast<value_type>(1) / size;

				for (int i = - static_cast<int>(this->my_radius); 
						i <= static_cast<int>(this->my_radius); ++i) {
					(*this)(i) = weight;
				}
			}
	};

	template<typename I>
	class BoxFilterer : 
		public Kernel2<
			I, 
			BoxSubKernel<I, BindX<I> >, 
			BoxSubKernel<I, BindY<I> > 
		> 
	{
		public:
			typedef BoxSubKernel<I, BindX<I> > SubKernelX;
			typedef BoxSubKernel<I, BindY<I> > SubKernelY;

			BoxFilterer(size_t x, size_t y)
				: Kernel2<I, SubKernelX, SubKernelY> 
				  ( SubKernelX(x), SubKernelY(y) )
			{
			}
	};

	template<typename I>
	struct FiltererTrait< BoxFilterer<I> >
	{
		typedef TrueType Separable;
	};

	template<typename I, typename B>
	class GaussianSubKernel: public SubKernel<I, B> {
		public:
			typedef 
				typename TypeTrait<
					typename ColorTrait<
						typename I::value_type
					>::BaseType
				>::MathType value_type;

			GaussianSubKernel(value_type sigma)
				: SubKernel<I, B>( our_round_to_odd(sigma) )
			{
				for (int i = - static_cast<int>(this->my_radius); 
						i <= static_cast<int>(this->my_radius); ++i) {
					(*this)(i) = std::exp( -(i*i) / sigma );
				}
			}
		protected:
			static size_t our_round_to_odd(value_type sigma)
			{
				size_t size = static_cast<size_t>( std::floor(sigma * 6) );
				if (size % 2 == 0)
					++size;
				return size;
			}
	};

	template<typename I>
	class GaussianFilterer :
		public Kernel2< I, 
			GaussianSubKernel<I, BindX<I> >, 
			GaussianSubKernel<I, BindY<I> > 
		> 
	{
		public:
			typedef GaussianSubKernel<I, BindX<I> > SubKernelX;
			typedef GaussianSubKernel<I, BindY<I> > SubKernelY;

			typedef 
				typename TypeTrait<
					typename ColorTrait<
						typename I::value_type
					>::BaseType
				>::MathType value_type;

			GaussianFilterer(value_type sigma_x, value_type sigma_y)
				: Kernel2<I, SubKernelX, SubKernelY>
					( SubKernelX(sigma_x), SubKernelY(sigma_y) )
			{
			}
	};

	template<typename I>
	struct FiltererTrait< GaussianFilterer<I> >
	{
		typedef TrueType Separable;
	};
}

#endif
