CFLAGS+=-O2 -I/usr/include/ffmpeg/ 
LDFLAGS+=-lavformat -lavcodec -lavutil -lz -lm -lpthread 

check_video: check_video.cpp
	g++ $(CFLAGS) $< -o $@ $(LDFLAGS)
