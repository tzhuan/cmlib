#ifndef CMLIB_DIP_KERNEL_H
#define CMLIB_DIP_KERNEL_H

#include <cstddef>
#include <vector>
#include <algorithm>

namespace cmlib {
namespace dip {

	template<typename T>
	class SquareKernel { 
	public:
		typedef T value_type;

		SquareKernel(size_t x = 0, size_t y = 0)
			: my_xradius(x/2), my_yradius(y/2), my_data(x*y), my_kernel(y)
		{
			init(x, y);
		}

		T& operator()(int x, int y)
		{
			return my_kernel[my_yradius + y][my_xradius + x];
		}

		const T& operator()(int x, int y) const
		{
			return my_kernel[my_yradius + y][my_xradius + x];
		}

		size_t ysize() const
		{
			return my_kernel.size();
		}

		size_t xsize() const
		{
			return ysize() ? my_data.size()/ysize() : 0;
		}

		size_t xradius() const
		{
			return my_xradius;
		}

		size_t yradius() const
		{
			return my_yradius;
		}

		void fill(T v)
		{
			std::fill(my_data.begin(), my_data.end(), v);
		}

		void resize(size_t x, size_t y)
		{
			my_data.resize(x*y);
			my_kernel.resize(y);
			my_xradius = x/2;
			my_yradius = y/2;
			init(x, y);
		}

	protected:
		void init(size_t x, size_t y)
		{
			my_kernel[0] = my_data.begin();
			for (size_t i = 1; i < y; ++i)
				my_kernel[i] = my_kernel[i-1] + x;
		}

		size_t my_xradius;
		size_t my_yradius;
		std::vector<T> my_data;
		std::vector< typename std::vector<T>::iterator > my_kernel;
	}; 

	template<typename T>
	class LineKernel { 
	public:
		typedef T value_type;

		LineKernel(size_t size = 0): my_radius(size/2), my_kernel(size)
		{
		}

		T& operator()(int i)
		{
			return my_kernel[my_radius + i];
		}

		const T& operator()(int i) const
		{
			return my_kernel[my_radius + i];
		}

		size_t size() const
		{
			return my_kernel.size();
		}

		void fill(T v)
		{
			std::fill(my_kernel.begin(), my_kernel.end(), v);
		}

		void resize(size_t size)
		{
			my_kernel.resize(size);
			my_radius = size/2;
		}

		size_t radius() const
		{
			return my_radius;
		}
	protected:
		size_t my_radius;
		std::vector<T> my_kernel;
	}; 

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_KERNEL_H
