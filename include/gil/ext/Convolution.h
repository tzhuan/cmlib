#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <stdexcept>
#include <vector>
#include <cassert>
#include <cmath>

#include <iostream>

#include "Filterer.h"
#include "../Color.h"

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

	template<typename I> struct BindX { 
		static int offset(size_t x, size_t, int i) 
		{
			return static_cast<int>(x) + i;
		}

		static int size(const I &image)
		{
			return image.width();
		}

		static typename I::value_type 
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

		static typename I::value_type 
		color(const I &image, size_t x, size_t, size_t i)
		{
			return image(x, i);
		}
	}; 

	template<typename T> class WeightedVector { 
		public:
			typedef T value_type;

			WeightedVector(size_t size): my_radius(size/2), my_vector(size)
			{
			}

			T& operator()(int i)
			{
				return my_vector[my_radius + i];
			}

			const T& operator()(int i) const
			{
				return my_vector[my_radius + i];
			}

			size_t size() const
			{
				return my_vector.size();
			}
		private:
			size_t my_radius;
			std::vector<T> my_vector;
	}; 

	template<typename T> 
	class WeightedMatrix { 
		public:
			typedef T value_type;

			WeightedMatrix(size_t x, size_t y): 
				my_radius_x(x/2), my_radius_y(y/2), my_matrix(y)
			{
				for (size_t i = 0; i < y; ++i)
					my_matrix[i].resize(x);
			}

			T& operator()(int x, int y)
			{
				return my_matrix[my_radius_y + y][my_radius_x + x];
			}

			const T& operator()(int x, int y) const
			{
				return my_matrix[my_radius_y + y][my_radius_x + x];
			}

			size_t height() const
			{
				return my_matrix.size();
			}

			size_t width() const
			{
				return (height() ? my_matrix[0].size() : 0);
			}
		protected:
			size_t my_radius_x;
			size_t my_radius_y;
			std::vector< std::vector<T> > my_matrix;
	}; 

	template<typename I, typename T> class WeightedKernel { 
		public:
			WeightedKernel(const T &table): my_table(table)
			{
			}

			typename I::value_type 
			operator()(const I &image, size_t x, size_t y) const
			{
				const int r_x = my_table.width() / 2;
				const int r_y = my_table.height() / 2;

				typename T::value_type num = 0;
				typename I::value_type sum = 0;

				for (int h = -r_y; h <= r_y; ++h) {
					const int cur_y = static_cast<int>(y) + h;
					if (cur_y < 0)
						continue;
					else if (cur_y >= static_cast<int>(image.height()))
						break;
					for (int w = -r_x; w <= r_x; ++w) {
						const int cur_x = static_cast<int>(x) + w;
						if (cur_x < 0)
							continue;
						else if (cur_x >= static_cast<int>(image.width()))
							break;
						sum += my_table(w, h) * image(cur_x, cur_y);
						num += my_table(w, h);
					}
				}

				assert(num);
				return sum / num;
			}
		protected:
			size_t my_size_x;
			size_t my_size_y;
			T my_table;
	}; 

	template<typename I, typename B, typename T>
	class WeightedSeparableSubKernel { 
		public:
			WeightedSeparableSubKernel(const T &table)
				: my_table(table)
			{
			}

			typename I::value_type 
			operator()(const I &image, size_t x, size_t y) const
			{
				const int r = my_table.size() / 2;

				// std::cerr << "(" << x << ", " << y << "): " << std::endl;

				typename T::value_type num = 0;
				typename I::value_type sum = 0;

				for (int i = -r; i <= r; ++i) {
					int cur = B::offset(x, y, i);
					if ( cur < 0 ) 
						continue;
					if ( cur >= B::size(image) ) 
						break;

					sum += my_table(i) * B::color(image, x, y, cur);
					num += my_table(i);

					/*
					std::cerr << "  " << cur << ": " << my_table(i) << " x " <<
						B::color(image, x, y, cur) << std::endl;
					*/
				}

				assert(num);

				/*
				std::cerr << "num: " << num << std::endl;
				std::cerr << "sum: " << sum << std::endl;
				std::cerr << "return: " << (sum/num) << std::endl;
				std::cerr << std::endl;
				*/

				return sum / num;
			}
		protected:
			T my_table;
	}; 

	template<typename I, typename Kx, typename Ky>
	class WeightedSeparableKernel { 
		public:
			WeightedSeparableKernel(const Kx &kx, const Ky &ky)
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
	class NullSubKernel: 
		public WeightedSeparableSubKernel<
			I, B, 
			WeightedVector<
				typename ColorTrait<typename I::value_type>::BaseType
			> 
		> 
	{
		public:

			typedef 
				typename ColorTrait<typename I::value_type>::BaseType
				value_type;

			NullSubKernel()
				: WeightedSeparableSubKernel<I, B, WeightedVector<value_type> >(
					WeightedVector<value_type>(1)
				)
			{
				this->my_table(0) = 1;
			}
	};
	template<typename I>
	class NullKernel: public WeightedSeparableKernel<
				 I, NullSubKernel<I, BindX<I> >, NullSubKernel<I, BindY<I> > 
			> {
		public:
			NullKernel()
				: WeightedSeparableKernel<
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
	class BoxSubKernel: 
		public WeightedSeparableSubKernel<
			I, B,
			WeightedVector< 
				typename ColorTrait<typename I::value_type>::BaseType 
			> 
		> 
	{
		public:
			typedef 
				typename ColorTrait<typename I::value_type>::BaseType 
				value_type;

			BoxSubKernel(size_t size)
				: WeightedSeparableSubKernel< I, B, WeightedVector<value_type> >
				  ( WeightedVector<value_type>(size) )
			{
				if (size % 2 == 0) {
					throw std::runtime_error(
						"the kernel size should be odd number."
					);
				}

				const int radius = size/2;
				value_type weight = static_cast<value_type>(1) / size;

				for (int i = -radius; i <= radius; ++i) {
					this->my_table(i) = weight;
				}
			}
	};

	template<typename I>
	class BoxKernel: public WeightedSeparableKernel<
				 I, BoxSubKernel<I, BindX<I> >, BoxSubKernel<I, BindY<I> > 
			> {
		public:
			BoxKernel(size_t x, size_t y)
				: WeightedSeparableKernel<
					  I, 
					  BoxSubKernel<I, BindX<I> >, 
					  BoxSubKernel<I, BindY<I> >
				>(
						BoxSubKernel< I, BindX<I> >(x),
						BoxSubKernel< I, BindY<I> >(y) 
				)
			{
			}
	};
	template<typename I>
	struct FiltererTrait< BoxKernel<I> >
	{
		typedef TrueType Separable;
	};

	/*
	template<typename I>
	class BoxKernel: public WeightedKernel<
				I, 
				WeightedMatrix<
					typename ColorTrait<typename I::value_type>::BaseType
				>
			> {
		public:
			typedef 
				typename ColorTrait<typename I::value_type>::BaseType
				value_type;
			BoxKernel(size_t x, size_t y)
				: WeightedKernel<I, WeightedMatrix<value_type> >( 
					WeightedMatrix<value_type>(x, y)
				)
			{
				if (x % 2 == 0 || y % 2 == 0) {
					throw std::runtime_error(
						"the kernel size should be odd number."
					);
				}

				const int rx = x/2;
				const int ry = y/2;
				value_type weight = 
					static_cast<value_type>(1) / (x * y);

				for (int h = -ry; h <= ry; ++h) 
					for (int w = -rx; w <= rx; ++w) {
						this->my_table(w, h) = weight;
				}
			}
	};
	*/

	template<typename I, typename B, typename T = double>
	class GaussianSubKernel: 
		public WeightedSeparableSubKernel<
			I, B,
			WeightedVector< 
				typename ColorTrait<typename I::value_type>::BaseType 
			> 
		> 
	{
		public:
			typedef 
				typename ColorTrait<typename I::value_type>::BaseType 
				value_type;

			GaussianSubKernel(T sigma)
				: WeightedSeparableSubKernel< I, B, WeightedVector<value_type> >
					( WeightedVector<value_type>( round_to_odd(sigma) ) )
			{
				const size_t size = round_to_odd(sigma);
				const int radius = size / 2;

				for (int i = -radius; i <= radius; ++i) {
					this->my_table(i) = std::exp( -(i*i) / sigma );
				}

				/*
				std::cerr << "(";
				for (int i = -radius; i < radius; ++i)
					std::cerr << this->my_table(i) << ", ";
				std::cerr << this->my_table(radius) << ")" << std::endl;
				*/
			}
		protected:
			static size_t round_to_odd(T sigma)
			{
				size_t size = static_cast<size_t>( std::floor(sigma * 6) );
				if (size % 2 == 0)
					++size;
				return size;
			}
		private:
	};

	template<typename I, typename T = double>
	class GaussianKernel: public WeightedSeparableKernel<
				 I, 
				 GaussianSubKernel<I, BindX<I> >, 
				 GaussianSubKernel<I, BindY<I> > 
			> {
		public:
			GaussianKernel(T sigma_x, T sigma_y)
				: WeightedSeparableKernel<
					  I, 
					  GaussianSubKernel<I, BindX<I>, T >, 
					  GaussianSubKernel<I, BindY<I>, T >
				>(
						GaussianSubKernel< I, BindX<I>, T >(sigma_x),
						GaussianSubKernel< I, BindY<I>, T >(sigma_y) 
				)
			{
			}
	};

	template<typename I>
	struct FiltererTrait< GaussianKernel<I> >
	{
		typedef TrueType Separable;
	};
}

#endif
