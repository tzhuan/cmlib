#ifndef CMLIB_VIDEO_VIDEO_FILE_STREAM_H
#define CMLIB_VIDEO_VIDEO_FILE_STREAM_H

/** @file VideoStream.h
 *  @brief The base class of VideoStream
 *  @author Tz-Huan Huang
 */

#ifdef DEBUG
#include <ostream>
#endif

#include <string>
#include <utility>
#include <vector>

#include "VideoStream.h"

struct AVFormatContext;
struct AVCodecContext;
struct AVFrame;
struct SwsContext;

namespace cmlib {
namespace video {

	/** @brief The base class of VideoStream
	 *
	 */
	class VideoFileStream: public VideoStream<cmlib::image::ByteImage3> {

	public:

		typedef VideoStream<cmlib::image::ByteImage3> base_type;

		/**
		 * @berif default constructor
		 */
		VideoFileStream(void);

		/**
		 * @berif constructor with a string parameter, which is the 
		 * video file name to be opened
		 * @param name the video file name to be opened
		 */
		explicit VideoFileStream(const std::string& name);

		/**
		 * @berif defulat disstructor, close the video file and destroy 
		 * myself
		 */
		virtual ~VideoFileStream(void);

		/** 
		 * @berif open the video file to be read
		 * @param name the video file name to be opened
		 */
		void open(std::string& name);

		/**
		 * @berif close the video file
		 */
		void close(void);

		/**
		 * @berif check if the video file is opened
		 */
		bool is_open(void)
		{
			return my_format_context;
		}

		VideoFileStream& seekg(int pos, seekdir dir = beg);

		VideoFileStream& seek_by_time(int64_t msec, seekdir dir = beg);

		VideoFileStream& seek_by_time(
			size_t hr, size_t min, size_t sec, size_t msec, 
			bool sign = true, seekdir dir = beg
		);

		size_t tell_index()
		{
			return (this->tellg() - 1);
		}

		size_t tellg()
		{
			return my_current;
		}

		bool is_keyframe();

#ifdef DEBUG
		std::ostream& info(std::ostream& os)
		{
			using namespace std;
			os << "file: " << my_filename << endl;
			os << "resolution: " << my_width << " x " << my_height << endl;
			os << "frame rate: "
				<< my_frame_rate.first << " / " << my_frame_rate.second << endl;
			os << "time base: "
				<< my_time_base.first << " / " << my_time_base.second << endl;
			os << "duration: " << my_duration << endl;
			os << "stream index: " << my_stream_index << endl;
			os << "start time: " << my_start_time << endl;
			os << "total duration: " << my_total_duration << endl;
			os << "# of frames: " << my_size << endl;
			return os;
		}
#endif
	protected:
		void init();
		virtual void get(cmlib::image::ByteImage3& image);
		virtual void get_keyframe(cmlib::image::ByteImage3& image);

	private:
#ifdef _MSC_VER
		typedef __int64 int64_t;
#endif // _MSC_VER

		std::string my_filename;
		size_t my_width;
		size_t my_height;
		size_t my_size;
		size_t my_current;

		int my_stream_index;
		int my_duration;
		int64_t my_start_time;
		int64_t my_total_duration;

		std::pair<int, int> my_frame_rate;
		std::pair<int, int> my_time_base;
		std::vector<int64_t> my_timestamp_map;
		
		AVFormatContext* my_format_context;
		AVCodecContext* my_codec_context;
		AVFrame* my_frame;
		SwsContext* my_sws_context;

		cmlib::image::ByteImage3 my_buffer;	
    };

} // namespace video
} // namespace cmlib

#endif // CMLIB_VIDEO_VIDEO_FILE_STREAM_H
