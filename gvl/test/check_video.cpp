extern "C" {
#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
}

#include <iostream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		cerr << "usage: " << argv[0] 
			<< " video-file [video stream index]" << endl;
		return 1;
	}
	string file(argv[1]);

    int video_stream_index = -1;
	if (argc > 2) {
		stringstream istr(argv[2]);
		istr >> video_stream_index;
		if (istr.fail()) {
			cerr << "error: video stream index must be integer" << endl;
			return 1;
		}
	}

    // Register all formats and codecs
    av_register_all();

    // Open video file
    AVFormatContext* format_context;
    if( av_open_input_file(&format_context, file.c_str(), 0, 0, 0) != 0 ) {
		cerr << "error: could not open " << file << endl;
        return 1;
	}

    // Retrieve stream information
    if( av_find_stream_info(format_context) < 0 ) {
		cerr << "error: could not find stream information" << endl;
        return 1; 
	}

    // Dump information about file onto standard error
    // dump_format(format_context, 0, file.c_str(), 0);

    // Find the first video stream
	if (video_stream_index < 0) {
		for(size_t i = 0; i < format_context->nb_streams; ++i) {
			CodecType type = format_context->streams[i]->codec->codec_type;
			if (type == CODEC_TYPE_VIDEO) {
				video_stream_index = i;
				break;
			}
		}
	} else {
		CodecType type = 
			format_context->streams[video_stream_index]->codec->codec_type;
		if (type != CODEC_TYPE_VIDEO) {
			cerr << "error: invalid video stream index" << endl;
			return 1;
		}
	}
    if(video_stream_index < 0) {
		cerr << "error: could not find video stream" << endl;
        return 1; 
	}

	AVStream* stream = format_context->streams[video_stream_index];

    // Get a pointer to the codec context for the video stream
    AVCodecContext* codec_context = stream->codec;
    AVCodec* codec = avcodec_find_decoder(codec_context->codec_id);
    if(codec == 0) {
		cerr << "error: could not find codec" << endl;
        return 1;
	}

    // Open codec
    if( avcodec_open(codec_context, codec) < 0 ) {
		cerr << "error: could not open codec" << endl;
        return 1;
	}

    // Allocate video frame
    AVFrame* frame = avcodec_alloc_frame();
	if ( frame == 0 ) {
		cerr << "error: could not alloc frame" << endl;
		return 1;
	}


	int start_time = stream->start_time;
	int64_t total_duration = stream->duration;

	map<int, int> duration_count;
	vector<int64_t> non_finished_index;

	int64_t count = 0;
	// read the frame
	AVPacket packet;
	while ( av_read_frame(format_context, &packet) == 0 ) {
		if ( packet.stream_index == video_stream_index ) {

			// decode the frame
			int finished = 0;
			avcodec_decode_video(
				codec_context, frame, &finished, packet.data, packet.size
			);
			if (!finished) {
				non_finished_index.push_back(count);
			}

			++count;
			duration_count[packet.duration]++;

			av_free_packet(&packet);
		}
	}

	if (non_finished_index.size()) {
		cerr << "error: non finished packet: ";
		for (size_t i = 0; i < non_finished_index.size(); ++i)
			cerr << " " << non_finished_index[i];
		cerr << endl;
	}
	if (duration_count.size() > 1) {
		cerr << "error: variant duration: " << endl;
		for (map<int, int>::iterator i = duration_count.begin();
				i != duration_count.end(); ++i)
			cerr << "  " << (*i).first << ": " << (*i).second << endl;
	} else if (duration_count.size() == 1) {
		int duration = (*duration_count.begin()).first;
		if ((count-1)*duration != total_duration) {
			cerr << "error: (" << count << " - 1) * " << duration 
				<< " != " << total_duration << endl;
		}
	} else {
		cerr << "error: could not get duration information" << endl;
	}

    av_free(frame);
    avcodec_close(codec_context);
    av_close_input_file(format_context);

    return 0;
}
