CMLIB_VIDEO_CFLAGS=-I../../../
CMLIB_IMAGE_CFLAGS=-I../../../
CMLIB_VIDEO_LIBS=-L../src -lcmlib-videostream
CMLIB_IMAGE_LIBS=-L../../image/src -lcmlib-imageio
FFMPEG_CFLAGS=`pkg-config --cflags libavcodec libavdevice libavfilter libavformat libavutil libswscale`
FFMPEG_LIBS=`pkg-config --libs libavcodec libavdevice libavfilter libavformat libavutil libswscale`

CFLAGS+=-O2 -g -DDEBUG $(CMLIB_IMAGE_CFLAGS) $(CMLIB_VIDEO_CFLAGS) $(FFMPEG_CFLAGS)
CXXFLAGS=$(CFLAGS)
LDFLAGS=$(CMLIB_IMAGE_LIBS) $(CMLIB_VIDEO_LIBS) $(FFMPEG_LIBS)

CC=gcc
CXX=g++

clean:
	rm -f test check_video
