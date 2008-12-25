#ifndef CMLIB_VIDEO_VIDEO_H
#define CMLIB_VIDEO_VIDEO_H

#include <cmath>
#include <cassert>
#include <cstddef>
#include <algorithm>
#include <string>
#include <deque>
#include <iterator>
#include <stdexcept>

extern "C" {
#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
}

#include <cmlib/image.h>

// using namespace std;
// using namespace cmlib::image;

namespace cmlib {
namespace video {

	template<class ImageType>
	class Video {
		public:

			template<class Type> class Iterator;
			template<class Type> class KeyIterator;

            // STL-compliance
            typedef ImageType value_type;
			// both iterator and const_iterator are const!
			typedef Iterator<ImageType> iterator;
			typedef Iterator<ImageType> const_iterator;
            typedef ImageType& reference;
            typedef const ImageType& const_reference;
            typedef ImageType* pointer;
            typedef std::ptrdiff_t difference_type;
            typedef std::size_t size_type;

			// both key_iterator and const_key_iterator are const!
			typedef KeyIterator<ImageType> key_iterator;
			typedef KeyIterator<ImageType> const_key_iterator;

			Video(const std::string& name, size_t stream_index = -1) // {{{
				: my_filename(name), my_stream_index(stream_index)
			{
				this->init();
			} // }}}

            virtual ~Video() // {{{
            {
                avcodec_close(my_codec_context);
                av_close_input_file(my_format_context);
            } //}}}
			
			const ImageType operator[](size_t i) const // {{{
			{
				// allocate video frame
				AVFrame* frame = avcodec_alloc_frame();
				if ( frame == 0 ) {
					throw std::runtime_error("avcodec_alloc_frame frame fail");
				}

				AVPacket packet;
				read_frame(i, frame, packet);

				// allocate picture buffer
				size_t size = 
					avpicture_get_size(PIX_FMT_RGB24, my_width, my_height);
				vector<uint8_t> buffer(size);
				AVPicture picture;
				avpicture_fill(
					&picture, &(buffer[0]), PIX_FMT_RGB24, my_width, my_height
				);

				// get the picture
				struct SwsContext* sws_context = 
					sws_getContext(
						my_width, my_height, my_codec_context->pix_fmt, 
						my_width, my_height, PIX_FMT_RGB24, SWS_FAST_BILINEAR,
						0, 0, 0
					);
				sws_scale(
					sws_context, frame->data, frame->linesize, 0, my_height, 
					picture.data, picture.linesize
				);
				sws_freeContext(sws_context);
				av_free_packet(&packet);

				// convert the picture to cmlib::image
				ImageType image(my_width, my_height);
				DefaultConverter<typename ImageType::value_type, Byte3> convert;
				std::vector<uint8_t>::iterator ptr = buffer.begin();
				for(size_t h = 0; h < my_height; ++h) {
					for(size_t w = 0 ; w < my_width ; ++w) {
						Byte3 color(*ptr, *(ptr+1), *(ptr+2));
						image(w, h) = convert(color);
						ptr += 3;
					}
				}
				return image;
			} // }}}

            void dump_info() const  // {{{
			{ 
				// FIXME: my_stream_index should be 0?
				dump_format(
					my_format_context, my_stream_index, my_filename.c_str(), 0
				);
			} // }}}

			const_iterator begin() const // {{{
			{
				return iterator(*this, 0);
			} // }}}

			const_iterator end() const // {{{
			{
				return iterator(*this, this->size()+1);
			} // }}}

			size_t size() const // {{{
			{
				return my_size;
			} // }}}

			template<class Type>
			class Iterator: // {{{
				public std::iterator<std::random_access_iterator_tag, Type> {

				friend class Video<Type>::key_iterator;
			public:
				Iterator(Video<Type>& v, size_t i = 0)
					: my_video(v), my_index(i)
				{
					// empty
				}

				Iterator(const typename Video<Type>::KeyIterator& i)
					: my_video(i.my_video), my_index(i.my_index)
				{
					// empty
				}

				Iterator& operator =(const typename Video<Type>::KeyIterator& i)
				{
					if (my_video != i.my_video)
						throw std::runtime_error("invalid assignment");
					my_index = i.my_index;
					return (*this);
				}

				const Type& operator *() const
				{
					return my_video[my_index];
				}

				Iterator& operator +=(size_t n)
				{
					my_index += n;
					return (*this);
				}

				Iterator operator +(size_t n)
				{
					Iterator tmp(*this);
					tmp += n;
					return tmp;
				}

				Iterator& operator -=(size_t n)
				{
					my_index -= n;
					return (*this);
				}

				Iterator operator -(size_t n)
				{
					Iterator tmp(*this);
					tmp -= n;
					return tmp;
				}

				Iterator& operator ++()
				{
					++my_index;
					return (*this);
				}

				Iterator operator ++(int)
				{
					Iterator tmp(*this);
					++(*this);
					return tmp;
				}

				Iterator& operator --()
				{
					--my_index;
					return (*this);
				}

				Iterator operator --(int)
				{
					Iterator tmp(*this);
					--(*this);
					return tmp;
				}

				std::ptrdiff_t operator -(const Iterator& i) const
				{
					return 
						static_cast<std::ptrdiff_t>(this->my_index) - 
						static_cast<std::ptrdiff_t>(i.my_index);
				}

				const Type operator [](std::ptrdiff_t n) const
				{
					return my_video[my_index + n];
				}

				bool operator <(const Iterator& i) const
				{
					return (my_index < i.my_index);
				}

			protected:
			private:
				Video<Type>& my_video;
				size_t my_index;
			}; // }}}

			template<class Type>
			class KeyIterator:  // {{{
				public std::iterator<std::forward_iterator_tag, Type> { 
				friend class Video<Type>::iterator;
			public:
				KeyIterator(Video<Type>& v, size_t i = 0)
					: my_video(v), my_index(i)
				{
					// empty
				}

				KeyIterator(const typename Video<Type>::Iterator& i)
					: my_video(i.video)
				{
					my_index = next_key_frame(i.my_index);
				}

				KeyIterator& operator =(const typename Video<Type>::Iterator& i)
				{
					if (my_video != i.my_video)
						throw std::runtime_error("invalid assignment");
					my_index = next_key_frame(i.my_index);
					return (*this);
				}

				const Type& operator *() const
				{
					return my_video[my_index];
				}

				const Type* operator ->() const
				{
					// FIXME
				}

				KeyIterator& operator ++()
				{
					my_index = next_key_frame(++my_index);
					return (*this);
				}

				KeyIterator operator ++(int)
				{
					KeyIterator tmp(*this);
					++(*this);
					return tmp;
				}

				bool operator <(const KeyIterator& i) const
				{
					return (my_index < i.my_index);
				}

			protected:
				size_t next_key_frame(size_t i) const
				{
					while (i < my_video.size()) {
						AVFrame* frame = avcodec_alloc_frame();
						if ( frame == 0 ) {
							throw std::runtime_error(
								"avcodec_alloc_frame frame fail"
							);
						}
						read_frame(i, frame);
						if (frame->key_frame)
							return i;
					}
					return i;
				}
			private:
				Video<Type>& my_video;
				size_t my_index;
			}; // }}}

        protected:
			void init() // {{{
			{
				AVFormatContext* &fc = my_format_context;

				// open the video file
				av_register_all();
				if(av_open_input_file(&fc, my_filename.c_str(), 0, 0, 0) != 0) {
					throw std::runtime_error("av_open_input_file error");
				}
				
				// check the stream exists or not
				if( av_find_stream_info(fc) < 0 ) {
					throw std::runtime_error("av_find_stream_info fail");
				}
				
				// find the stream index
				if (my_stream_index < 0) {
					for(size_t i = 0; i < fc->nb_streams; ++i) {
						CodecType &type = fc->streams[i]->codec->codec_type;
						if(type == CODEC_TYPE_VIDEO) {
							my_stream_index = i;                           
							break;
						}
					}
					if(my_stream_index < 0) {
						throw std::runtime_error("no video stream found");
					}
				}

				// prepare the codec
				AVStream* stream = fc->streams[my_stream_index];
				AVCodecContext* &cc = my_codec_context;
				cc = stream->codec;
				AVCodec* codec = avcodec_find_decoder(cc->codec_id);
				if(codec == 0) {
					throw std::runtime_error("no codec found");
				}

				if( avcodec_open(cc, codec) < 0 ) {
					throw std::runtime_error("avcodec_open fail");
				}

				// read the first packet to get the duration
				AVPacket packet;
				while ( av_read_frame(fc, &packet) == 0 ) {
					if ( packet.stream_index == my_stream_index ) {
						my_duration = packet.duration;
						av_free_packet(&packet);
						break;
					}
					av_free_packet(&packet);
				}
				if (my_duration == 0) {
					throw std::runtime_error("av_read_frame fail");
				}

				// store the necessary info
				my_start_time = stream->start_time;
				my_total_duration = stream->duration;
				my_width = cc->width;                         
				my_height = cc->height;

				// calculate the size
				my_size = 1 + (my_total_duration - my_start_time) / my_duration;

			} // }}}

			AVFrame* read_frame(size_t i, AVFrame *frame, AVPacket& packet) // {{{
			{
				// seek the frame
				int ret = av_seek_frame(
					my_format_context, 
					my_stream_index,
					my_start_time + i * my_duration, 
					AVSEEK_FLAG_ANY
				);
				if (ret < 0) {
					throw std::runtime_error("av_seek_frame fail");
				}

				// read the frame
				ret = av_read_frame(my_format_context, &packet);
				if (ret < 0) {
					throw std::runtime_error("av_read_frame fail");
				}

				// decode the frame
				int finished = 0;
				ret = avcodec_decode_video(
					my_codec_context, 
					frame, 
					&finished, 
					packet.data, 
					packet.size
				);
				if (ret == 0) {
					throw std::runtime_error("no frame to be decode");
				} else if (ret < 0) {
					throw std::runtime_error("avcodec_decode_video fail");
				}
				assert(finished);

				return frame;
			} // }}}

        private:
			std::string	my_filename;
			size_t my_size;
			size_t my_width;
			size_t my_height;
			size_t my_stream_index;
			
			int64_t	my_start_time;
			int my_duration;
			int64_t	my_total_duration;
			AVFormatContext* my_format_context;
			AVCodecContext* my_codec_context;
			AVRational my_frame_rate;
			AVRational my_time_base;
    };

} // namespace video
} // namespace cmlib

#endif // CMLIB_VIDEO_VIDEO_H
