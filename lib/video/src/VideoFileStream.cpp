/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/**
 * @file VideoFileStream.cpp
 * @brief implementation of class VideoFileStream
 * @author Tz-Huan Huang
 */

#include <cstring>
#include <stdexcept>
#include <iostream>
#include <algorithm>

extern "C" {
#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <cmlib/video/VideoFileStream.h>

namespace cmlib {
namespace video {

	using cmlib::image::ByteImage4;
	using std::string;
	using namespace std;

	VideoFileStream::VideoFileStream(const string& name) // {{{
		: my_filename(name)
	{
		this->init();
		this->open(my_filename);
	} // }}}

	VideoFileStream::VideoFileStream(void) // {{{
	{
		this->init();
	} // }}}

	VideoFileStream::~VideoFileStream(void) // {{{
	{
		this->close();
	} // }}}

	void VideoFileStream::init(void) // {{{
	{
		my_stream_index = -1;
		my_format_context = 0;
		my_codec_context = 0;
		my_frame = 0;
		my_sws_context = 0;
		my_current = 0;
	} // }}}

	VideoFileStream& VideoFileStream::seekg(int pos, seekdir dir) // {{{
	{
		size_t target;
		if (dir == cur)
			target = my_current + pos;	
		else if (dir == end)
			target = my_size - pos - 1;
		else 
			target = pos;
		if (target >= my_size) {
			this->setstate(failbit);
		} else {
#ifdef DEBUG
			int64_t offset = 
				my_timestamp_map[target] - my_timestamp_map[my_current];
			cerr 
				<< my_current << "(" << my_timestamp_map[my_current] 
				<< ") -> " 
				<< target << "(" << my_timestamp_map[target] << ")" << endl;
			cerr << "offset: " << offset << endl;
#endif
			if (av_seek_frame(
				my_format_context, 
				my_stream_index,
				my_timestamp_map[target],
				0
				/*AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD*/
			) < 0)
				this->setstate(badbit);
			else
				my_current = target;
		}
		return (*this);
	} // }}}

	VideoFileStream& VideoFileStream::seek_by_time(int64_t msec, seekdir dir) // {{{
	{
		int64_t target;
		int64_t unit = msec * my_time_base.second / my_time_base.first;
		if (dir == cur)
			target = my_timestamp_map[my_current] + unit;
		else if (dir == end)
			target = my_timestamp_map[my_size - 1] - unit;
		else 
			target = unit;
		if (target < 0 || target > my_timestamp_map[my_size - 1]) {
			this->setstate(failbit);
		} else {
			if (av_seek_frame(
				my_format_context, 
				my_stream_index,
				target, 
				AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD
			) < 0)
				this->setstate(badbit);
		}
		return (*this);
	} // }}}

	VideoFileStream& VideoFileStream::seek_by_time( // {{{
		size_t hr, size_t min, size_t sec, size_t msec, 
		bool sign, seekdir dir
	)
	{
		int64_t totalmsec = 1000000*(60*(60*hr + min) + sec) + msec;
		if (!sign)
			totalmsec = -totalmsec;
		return this->seek_by_time(totalmsec, dir);
	} // }}}

	bool VideoFileStream::is_keyframe() // {{{
	{
		return my_frame->key_frame;
	} // }}}

	void VideoFileStream::open(std::string& name) // {{{
#ifdef _MSC_VER
#if _MSC_VER < 1500
// The function-try-block is not supportted by MSVC prior to 9.0 (15.00)
	{ // {{{
#endif
#endif
	try { // {{{

		// if is_open(), close it first.
		if (this->is_open())
			this->close();

		AVFormatContext* &fc = my_format_context;

		// open the video file
		av_register_all();
		if (avformat_open_input(&fc, name.c_str(), 0, 0) != 0) {
			throw std::runtime_error("no video stream found");
		}
		
		// check the stream exists or not
		if (avformat_find_stream_info(fc, 0) < 0) {
			throw std::runtime_error("no video stream found");
		}

		// find the stream index
		if (my_stream_index < 0) {
			for(size_t i = 0; i < fc->nb_streams; ++i) {
				AVMediaType &type = fc->streams[i]->codec->codec_type;
				if (type == AVMEDIA_TYPE_VIDEO) {
					my_stream_index = i;                           
					break;
				}
			}
			if (my_stream_index < 0) {
				throw std::runtime_error("no video stream found");
			}
		}

		// prepare the codec
		AVStream* stream = fc->streams[my_stream_index];
		AVCodecContext* &cc = my_codec_context;
		cc = stream->codec;
		AVCodec* codec = avcodec_find_decoder(cc->codec_id);
		if (codec == 0) {
			throw std::runtime_error("no codec found");
		}

		if (avcodec_open2(cc, codec, 0) < 0) {
			throw std::runtime_error("avcodec_open fail");
		}


		// store the necessary info
		my_start_time = stream->start_time;
		my_total_duration = stream->duration;

		my_frame_rate.first = stream->r_frame_rate.num;
		my_frame_rate.second = stream->r_frame_rate.den;
		my_time_base.first = stream->time_base.num;
		my_time_base.second = stream->time_base.den;

		my_width = cc->width;                         
		my_height = cc->height;

		// allocate video frame
		if ((my_frame = avcodec_alloc_frame()) == 0) {
			throw std::runtime_error("avcodec_alloc_frame frame fail");
		}

		// allocate av picture
		my_picture = new AVPicture;
		if (avpicture_alloc(my_picture, PIX_FMT_RGBA, my_width, my_height) < 0)
			throw std::runtime_error("avpicture_alloc fail");

		// allocate sws context
		my_sws_context = sws_getContext(
			my_width, my_height, my_codec_context->pix_fmt, 
			my_width, my_height, PIX_FMT_RGBA, SWS_POINT,
			0, 0, 0
		);
		if (my_sws_context == 0)
			throw std::runtime_error("sws_getContext fail");

		// scan the whole video to build the timestamp map
		my_duration = 
			my_time_base.second * my_frame_rate.second /
			my_time_base.first / my_frame_rate.first;

		size_t frame_no = 1.1 * (my_total_duration / my_duration);
		my_timestamp_map.reserve(frame_no);
		AVPacket packet;
		while (av_read_frame(fc, &packet) == 0) {
			if (packet.stream_index == my_stream_index)
				my_timestamp_map.push_back(packet.dts);		
			av_free_packet(&packet);
		}
		my_size = my_timestamp_map.size();
#ifdef DEBUG
		std::copy(
			my_timestamp_map.begin(), 
			my_timestamp_map.begin()+10, 
			ostream_iterator<int64_t>(cerr, "\n")
		);
#endif

		// reset the reading pointer
		int flag = AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD;
		av_seek_frame(fc, my_stream_index, my_timestamp_map[0], flag);
	} // }}} 
	catch (std::exception& e) { // {{{
		this->close();
		throw e;
	} // }}}
#ifdef _MSC_VER
#if _MSC_VER < 1500
	} // }}}
#endif
#endif
	// }}}

	void VideoFileStream::close(void) // {{{
	{
		if (my_codec_context) {
			avcodec_close(my_codec_context);
			my_codec_context = 0;
		}
		if (my_format_context) {
			avformat_close_input(&my_format_context);
			my_format_context = 0;
		}
		if (my_frame) {
			av_free(my_frame);
			my_frame = 0;
		}
		if (my_picture) {
			avpicture_free(my_picture);
			delete my_picture;
		}
		if (my_sws_context) {
			sws_freeContext(my_sws_context);
			my_sws_context = 0;
		}
	} // }}}

	void VideoFileStream::get(ByteImage4& image) // {{{
	{
		AVPacket packet;
		while (true) {
			// read the frame
			if (av_read_frame(my_format_context, &packet) != 0) {
				// XXX fail or eof 
				this->setstate(failbit | eofbit);	
				break;
			}
			
			if (packet.stream_index == my_stream_index) {
#ifdef DEBUG
				cerr << "current: " << my_current 
					<< ", dts: " << packet.dts 
					<< ", pts: " << packet.pts << endl;
#endif
				// decode the frame
				++my_current;
				int finished = 0;
				int ret = avcodec_decode_video2(
					my_codec_context, my_frame, &finished, &packet
				);
				if (ret == 0) {
					// FIXME: no frame to be decode
					this->setstate(eofbit | failbit);	
					break;
				} else if (ret < 0) {
					// FIXME: decode fail
					this->setstate(badbit | failbit);	
					break;
				}

				if (finished == 0) {
					// bad frame
					this->setstate(failbit);	
					break;
				} else {
					// get the picture
					sws_scale(
						my_sws_context, 
						my_frame->data, my_frame->linesize, 0, my_height, 
						my_picture->data, my_picture->linesize
					);

					image.resize(my_width, my_height);
					for (ByteImage4::size_type h = 0; h < image.height(); ++h) {
						memcpy(
							&(image(0, h)[0]),
							my_picture->data[0] + h*my_picture->linesize[0],
							my_width*4 /* PIX_FMT_RGBA */
						);
					}

					break;
				}
			}
			av_free_packet(&packet);
		}
		av_free_packet(&packet);
	}  // }}}

	void VideoFileStream::get_keyframe(ByteImage4& image) // {{{
	{
		do {
			this->get(image);
		} while (this->is_keyframe());
	} // }}}

} // namespace video
} // namespace cmlib
