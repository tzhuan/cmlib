CFLAGS+=-O2 -I/usr/local/include -I/usr/local/include/ffmpeg/ 
LDFLAGS+=-L/usr/local/lib -lavformat -lavcodec -lavutil -lz -lm -lpthread 

.SUFFIXES: .cc

test: test.cpp
	g++ $(CFLAGS) $(.ALLSRC) -o $(.TARGET) $(LDFLAGS)

check_video: check_video.cpp
	g++ $(CFLAGS) $(.ALLSRC) -o $(.TARGET) $(LDFLAGS)

run: test 
	time ./$(.ALLSRC) LGW_20071101_E1_CAM4.mpeg

#test: test.cc
#	g++ $(CFLAGS) $(.ALLSRC) -o $(.TARGET) $(LDFLAGS)

