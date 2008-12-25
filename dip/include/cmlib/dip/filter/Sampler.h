#ifndef CMLIB_DIP_FILTER_SAMPLER_H
#define CMLIB_DIP_FILTER_SAMPLER_H

namespace cmlib {
namespace dip {

	template<class Image>
	class BasicSampler {
	public:

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
			if (x < 0 || x >= width() || y < 0 || y >= height()) {
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

	private:
		const Image& my_image;
	};

	template<class Image>
	class SymmetricSampler {
	public:
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
				x = -x;
			else if (x >= width())
				x = width() - x;
			if (y < 0)
				y = -y;
			else if (y >= height())
				y = height() - y;
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
	private:
		const Image& my_image;
	};

	template<class Image>
	class ReplicateSampler {
	public:

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
			else if (x >= width())
				x = width() - 1;
			if (y < 0)
				y = 0;
			else if (y >= height())
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

	private:
		const Image& my_image;

	};

	template<class Image>
	class CircularSampler {
	public:
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
			else if (x >= width())
				x -= width();
			if (y < 0)
				y += height();
			else if (y >= height())
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

	private:
		const Image& my_image;
	};

	template<class Image>
	const BasicSampler<Image> basic_sampler(Image& image)
	{
		return BasicSampler<Image>(image);
	}

	template<class Image>
	const SymmetricSampler<Image> symmetric_sampler (Image& image)
	{
		return SymmetricSampler<Image>(image);
	}

	template<class Image>
	const ReplicateSampler<Image> replicate_sampler(Image& image)
	{
		return ReplicateSampler<Image>(image);
	}

	template<class Image>
	const CircularSampler<Image> circular_sampler(Image& image)
	{
		return CircularSampler<Image>(image);
	}

} // namespace dip
} // namespace cmlib

#endif // CMLIB_DIP_FILTER_SAMPLER_H
