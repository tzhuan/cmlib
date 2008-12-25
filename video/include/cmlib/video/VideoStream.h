#ifndef CMLIB_VIDEO_VIDEO_STREAM_H
#define CMLIB_VIDEO_VIDEO_STREAM_H

/** @file VideoStream.h
 *  @brief The base class of VideoStream
 *  @author Tz-Huan Huang
 */

#include <stdexcept>
#include <cmlib/image.h>

namespace cmlib {
namespace video {

	/** @brief The base class of VideoStream
	 *
	 */
	template<class ImageType>
	class VideoStream {
	public:

		typedef int seekdir;
		typedef int fmtflags;
		typedef int iostate;

		typedef std::runtime_error failure;

		static const seekdir beg = 0;
		static const seekdir cur = 1;
		static const seekdir end = 2;

		static const fmtflags skip_to_keyframe = 1 << 0;

		static const iostate goodbit = 0;
		static const iostate badbit = 1 << 0;
		static const iostate eofbit = 1 << 1;
		static const iostate failbit = 1 << 2;

		virtual ~VideoStream() // {{{
		{
			// empty
		} //}}}

		/* interfaces of ios_base */

		/**
		 * @brief  Access to format flags.
		 * @return  The format control flags for both input and output.
		*/
		inline fmtflags flags() const 
		{ 
			return my_flags;
		}

		/**
		 * @brief  Setting new format flags all at once.
		 * @param  flags The new flags to set.
		 * @return  The previous format control flags.
		 *
		 * This function overwrites all the format flags with @a fmtfl.
		*/
		inline fmtflags flags(fmtflags flags)
		{ 
		  fmtflags old = my_flags;
		  my_flags = flags;
		  return old;
		}

		/**
		 * @brief  Setting new format flags.
		 * @param  flags Additional flags to set.
		 * @return  The previous format control flags.
		 *
		 * This function sets additional flags in format control.  Flags that
		 * were previously set remain set.
		*/
		inline fmtflags setf(fmtflags flags)
		{                  
		  fmtflags old = my_flags;
		  my_flags |= flags;
		  return old;
		}
		
		/**
		 * @brief  Setting new format flags.
		 * @param  flags Additional flags to set.
		 * @param  mask  The flags mask for @a fmtfl.
		 * @return  The previous format control flags.
		 *
		 * This function clears @a mask in the format flags, then sets
		 * @a fmtfl @c & @a mask.  An example mask is @c ios_base::adjustfield.
		*/
		inline fmtflags setf(fmtflags flags, fmtflags mask)
		{                  
		  fmtflags old = my_flags;
		  my_flags &= ~mask;
		  my_flags |= (flags & mask);
		  return old;
		}

		/**
		 * @brief  Clearing format flags.
		 * @param  mask  The flags to unset.
		 *
		 * This function clears @a mask in the format flags.
		*/
		inline void unsetf(fmtflags mask) 
		{
			my_flags &= ~mask;
		}          


		/* interface of ios */

		operator void*() const
		{
			return this->fail() ? 0 : const_cast<VideoStream*>(this);
		}
															  
		bool operator!() const
		{
			return this->fail(); 
		}

		iostate rdstate() const
		{
			return my_state;
		}

		void setstate(iostate state)
		{
			this->clear( this->rdstate() | state );
		}

		void clear(iostate state = goodbit)
		{
			my_state = state;
			if (this->exceptions() & this->rdstate())
				throw failure("VideoStream::clear");
		}

		bool good() const
		{
			return (my_state == goodbit);
		}

		bool eof() const
		{
			return (my_state & eofbit);
		}

		bool fail() const
		{
			return (my_state & failbit);
		}

		bool bad() const
		{
			return (my_state & badbit);
		}

		iostate exceptions() const
		{
			return my_exceptions;
		}

		void exceptions(iostate except)
		{
			my_exceptions = except;
			this->clear(this->rdstate());
		}

		/**
		 * @brief extract one image from video with the same image type
		 */
		VideoStream& operator>>(ImageType& image)
		{
			if (this->flags() & skip_to_keyframe)
				this->get_keyframe(image);
			else
				this->get(image);
			return (*this);
		}

		/**
		 * @brief extract one image from video and translate it to target
		 * image type with DefaultConverter
		 */
		template<class DstImage>
		VideoStream& operator>>(DstImage& image)
		{
			return this->template operator>> <
				cmlib::image::DefaultConverter<DstImage, ImageType
			>, DstImage>(image);
		}

		/**
		 * @brief extract one image from video and translate it to target
		 * image type with custom's converter
		 */
		template<class Converter, class DstImage>
		VideoStream& operator>>(DstImage& image)
		{
			if (this->flags() & skip_to_keyframe)
				this->get_keyframe(my_buffer);
			else
				this->get(my_buffer);
			transform(
				my_buffer.begin(), my_buffer.end(), 
				image.begin(), Converter()
			);
			return (*this);
		}

		VideoStream& operator>>(VideoStream& (*pf)(VideoStream&))
		{ 
			return pf(*this); 
		}
protected:

		virtual void get(ImageType& image) = 0;
		virtual void get_keyframe(ImageType& image) = 0;

		fmtflags my_flags;
		iostate my_state;
		iostate my_exceptions;

		ImageType my_buffer;

		/**
		 * @brief default constructor
		 */
		VideoStream(): my_flags(0), my_state(0), my_exceptions(0)
		{
			// empty
		}


		VideoStream& operator=(const VideoStream&);
	private:
    };

	template<class ImageType>
	inline VideoStream<ImageType>&
	noskipws(VideoStream<ImageType>& vs)
	{
		vs.setf(VideoStream<ImageType>::skip_to_keyframe);
		return vs;
	}

} // namespace video
} // namespace cmlib

#endif // CMLIB_VIDEO_VIDEO_STREAM_H
