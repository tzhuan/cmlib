/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

extern "C" {
#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


using namespace std;

int main(int argc, char *argv[])
try {
	if (argc < 2) {
		string what = 
			string("usage: ") + argv[0] + " video-file [video stream index]";
		throw runtime_error(what);
	}
	string file(argv[1]);

    int video_stream_index = -1;
	if (argc > 2) {
		stringstream istr(argv[2]);
		istr >> video_stream_index;
		if (istr.fail())
			throw runtime_error("video stream index must be integer");
	}

    // Register all formats and codecs
    av_register_all();

    // Open video file
    AVFormatContext* format_context = 0;
    if( avformat_open_input(&format_context, file.c_str(), 0, 0) != 0 )
		throw runtime_error(string("could not open ") + file);

    // Retrieve stream information
    if( avformat_find_stream_info(format_context, 0) < 0 )
		throw runtime_error("could not find stream information");

    // Dump information about file onto standard error
    av_dump_format(format_context, 0, file.c_str(), 0);

    // Find the first video stream
	if (video_stream_index < 0) {
		for(size_t i = 0; i < format_context->nb_streams; ++i) {
			AVMediaType type = format_context->streams[i]->codec->codec_type;
			if (type == AVMEDIA_TYPE_VIDEO) {
				video_stream_index = i;
				break;
			}
		}
	} else {
		AVMediaType type = 
			format_context->streams[video_stream_index]->codec->codec_type;
		if (type != AVMEDIA_TYPE_VIDEO)
			throw runtime_error("invalid video stream index");
	}
    if(video_stream_index < 0) 
		throw runtime_error("could not find video stream");

	AVStream* stream = format_context->streams[video_stream_index];

    // Get a pointer to the codec context for the video stream
    AVCodecContext* codec_context = stream->codec;
    AVCodec* codec = avcodec_find_decoder(codec_context->codec_id);
    if(codec == 0)
		throw runtime_error("could not find codec");

    // Open codec
    if( avcodec_open2(codec_context, codec, 0) < 0 )
		throw runtime_error("could not open codec");

    // Allocate video frame
    AVFrame* frame = avcodec_alloc_frame();
	if ( frame == 0 )
		throw runtime_error("could not alloc frame");

	int duration = stream->time_base.den * stream->r_frame_rate.den /
		stream->time_base.num / stream->r_frame_rate.num;
	int64_t start_time = stream->start_time;
	int64_t total_duration = stream->duration;

	map<int, int> duration_count;
	vector<int64_t> non_finished_index;

	int64_t count = 0;

	AVPacket packet;

	int64_t guess_count = 0;
	if (duration)
		guess_count = total_duration / duration;
	int64_t dot = guess_count / 100;
	int64_t show = guess_count / 10;

	cout << "frame rate: " << stream->r_frame_rate.num 
		<< "/" << stream->r_frame_rate.den << endl;
	cout << "time base: "
		<< stream->time_base.num << "/" << stream->time_base.den << endl;
	cout << "start time: " << start_time << endl;
	cout << "total duration: " << total_duration << endl;
	cout << "duration: " << duration << endl;
	cout << "estimated frame number: " << guess_count << endl;
	cout << "start to decoding " << endl;
	
	// read the whole frames
	while ( av_read_frame(format_context, &packet) == 0 ) {
		if ( packet.stream_index == video_stream_index ) {

			// decode the frame
			int finished = 0;
			avcodec_decode_video2(codec_context, frame, &finished, &packet);
			if (finished == 0) {
				non_finished_index.push_back(count);
			} else {
				++count;
				duration_count[packet.duration]++;
				if (packet.dts == packet.pts)
					cerr << packet.dts; 
				else
					cerr << "@ " << packet.dts << " " << packet.pts;
				cerr << " " << codec_context->frame_number << endl;
			}

			av_free_packet(&packet);

			/*
			if (guess_count) {
				int shift = dot ? 1 : 0;
				if (dot && count % dot == 0) 
					cout << ".";
				if (show && count % show == 0)
					cout << " " << shift + 100 * count / guess_count << "% ";
				cout.flush();
			}
			*/
			++count;
			if (packet.dts == packet.pts)
				cout << packet.dts; 
			else
				cout << "@ " << packet.dts << " " << packet.pts;
			cout << " " << count << endl;
		}
		av_free_packet(&packet);
	}
	av_read_frame(format_context, &packet);
	if ( packet.stream_index == video_stream_index ) {
		++count;
		if (packet.dts == packet.pts)
			cerr << packet.dts; 
		else
			cerr << "@ " << packet.dts << " " << packet.pts;
		cerr << " " << count << endl;
	}
	av_free_packet(&packet);

	cout << endl;

	cout << "total frame number: " << count << endl;

	string prefix("[NOTICE]: ");

	if (non_finished_index.size()) {
		cerr << prefix << non_finished_index.size() 
			<< " non finished packet: ";
		for (size_t i = 0; i < non_finished_index.size(); ++i)
			cerr << " " << non_finished_index[i];
		cerr << endl;
	}
	if (duration_count.size() > 1) {
		cerr << prefix << "variant durations: " << endl;
		for (map<int, int>::iterator i = duration_count.begin();
				i != duration_count.end(); ++i)
			cerr << "  " << (*i).first << ": " << (*i).second << endl;
	} else if (duration_count.size() == 1) {
		cerr << "duration: ";
		for (map<int, int>::iterator i = duration_count.begin();
				i != duration_count.end(); ++i)
			cerr << (*i).first << ": " << (*i).second << endl;

		int duration = (*duration_count.begin()).first;
		if (count*duration != total_duration) {
			cerr << prefix << count << " * " << duration 
				<< " != " << total_duration << endl;
		}
	} else {
		cerr << prefix << "could not get duration information" << endl;
	}

    av_free(frame);
    avcodec_close(codec_context);
    avformat_close_input(&format_context);

    return 0;
}
catch (std::exception& e) {
	cerr << "error: " << e.what() << endl;
	return 1;
}
catch (...) {
	cerr << "unknown error" << endl;
	return 2;
}
