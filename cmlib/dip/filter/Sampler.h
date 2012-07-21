/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_DIP_FILTER_SAMPLER_H
#define CMLIB_DIP_FILTER_SAMPLER_H

namespace cmlib {
namespace dip {

	template<class Image>
	class DefaultSampler {
	public:

		enum { outside_normalize = false };

		typedef Image image_type;
		typedef typename Image::value_type value_type;
		typedef typename Image::size_type size_type;
		typedef typename Image::difference_type difference_type;

		explicit DefaultSampler(const Image& image, value_type value = value_type(0))
			: my_image(image), my_default(value)
		{
			// empty
		}

		const value_type operator ()(difference_type x, difference_type y, bool& countable) const
		{
			countable = true;
			if (x < 0 || x >= static_cast<difference_type>(width()) ||
				y < 0 || y >= static_cast<difference_type>(height()))
				return my_default;
			return (*this)(x, y);
		}

		const value_type& operator ()(difference_type x, difference_type y) const
		{
			return my_image(x, y);
		}

		size_type width() const
		{
			return my_image.width();
		}

		size_type height() const
		{
			return my_image.height();
		}

		const Image& image() const
		{
			return my_image;
		}

	private:
		const Image& my_image;
		const value_type my_default;
	};

	template<class Image>
	class BasicSampler {
	public:

		enum { outside_normalize = true };

		typedef Image image_type;
		typedef typename Image::value_type value_type;
		typedef typename Image::size_type size_type;
		typedef typename Image::difference_type difference_type;

		explicit BasicSampler(const Image& image)
			: my_image(image)
		{
			// empty
		}

		const value_type operator ()(difference_type x, difference_type y, bool& countable) const
		{
			if (x < 0 || x >= static_cast<difference_type>(width()) ||
				y < 0 || y >= static_cast<difference_type>(height())) {
				countable = false;
				return value_type(0);
			}
			countable = true;
			return (*this)(x, y);
		}

		const value_type& operator ()(difference_type x, difference_type y) const
		{
			return my_image(x, y);
		}

		size_type width() const
		{
			return my_image.width();
		}

		size_type height() const
		{
			return my_image.height();
		}

		const Image& image() const
		{
			return my_image;
		}

	private:
		const Image& my_image;
	};

	template<class Image>
	class SymmetricSampler {
	public:
		enum { outside_normalize = false };

		typedef Image image_type;
		typedef typename Image::value_type size_type;
		typedef typename Image::value_type value_type;
		typedef typename Image::difference_type difference_type;

		explicit SymmetricSampler(const Image& image)
			: my_image(image)
		{
			// empty
		}

		const value_type operator ()(difference_type x, difference_type y, bool& countable) const
		{
			countable = true;
			if (x < 0)
				x = -x - 1;
			else if (x >= static_cast<difference_type>(width()))
				x = width() + width() - x - 1;
			if (y < 0)
				y = -y - 1;
			else if (y >= static_cast<difference_type>(height()))
				y = height() + height() - y - 1;
			return (*this)(x, y);
		}

		const value_type& operator ()(difference_type x, difference_type y) const
		{
			return my_image(x, y);
		}

		size_type width() const
		{
			return my_image.width();
		}

		size_type height() const
		{
			return my_image.height();
		}

		const Image& image() const
		{
			return my_image;
		}

	private:
		const Image& my_image;
	};

	template<class Image>
	class ReplicateSampler {
	public:
		enum { outside_normalize = false };

		typedef Image image_type;
		typedef typename Image::size_type size_type;
		typedef typename Image::value_type value_type;
		typedef typename Image::difference_type difference_type;

		explicit ReplicateSampler(const Image& image)
			: my_image(image)
		{
			// empty
		}

		value_type operator ()(difference_type x, difference_type y, bool& countable) const
		{
			countable = true;
			if (x < 0)
				x = 0;
			else if (x >= static_cast<difference_type>(width()))
				x = width() - 1;
			if (y < 0)
				y = 0;
			else if (y >= static_cast<difference_type>(height()))
				y = height() - 1;
			return (*this)(x, y);
		}

		const value_type& operator ()(difference_type x, difference_type y) const
		{
			return my_image(x, y);
		}

		size_type width() const
		{
			return my_image.width();
		}

		size_type height() const
		{
			return my_image.height();
		}

		const Image& image() const
		{
			return my_image;
		}

	private:
		const Image& my_image;

	};

	template<class Image>
	class CircularSampler {
	public:
		enum { outside_normalize = false };

		typedef Image image_type;
		typedef typename Image::size_type size_type;
		typedef typename Image::value_type value_type;
		typedef typename Image::difference_type difference_type;

		explicit CircularSampler(const Image& image)
			: my_image(image)
		{
			// empty
		}

		value_type operator ()(difference_type x, difference_type y, bool& countable) const
		{
			countable = true;
			if (x < 0)
				x += width();
			else if (x >= static_cast<difference_type>(width()))
				x -= width();
			if (y < 0)
				y += height();
			else if (y >= static_cast<difference_type>(height()))
				y -= height();
			return (*this)(x, y);
		}

		const value_type& operator ()(difference_type x, difference_type y) const
		{
			return my_image(x, y);
		}

		size_type width() const
		{
			return my_image.width();
		}

		size_type height() const
		{
			return my_image.height();
		}

		const Image& image() const
		{
			return my_image;
		}

	private:
		const Image& my_image;
	};

	template<class Image>
	DefaultSampler<Image> default_sampler(const Image& image, typename Image::value_type value = (typename Image::value_type)(0))
	{
		return DefaultSampler<Image>(image, value);
	}

	template<class Image>
	BasicSampler<Image> basic_sampler(const Image& image)
	{
		return BasicSampler<Image>(image);
	}

	template<class Image>
	SymmetricSampler<Image> symmetric_sampler (const Image& image)
	{
		return SymmetricSampler<Image>(image);
	}

	template<class Image>
	ReplicateSampler<Image> replicate_sampler(const Image& image)
	{
		return ReplicateSampler<Image>(image);
	}

	template<class Image>
	CircularSampler<Image> circular_sampler(const Image& image)
	{
		return CircularSampler<Image>(image);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_SAMPLER_H
