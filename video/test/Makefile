CFLAGS+=-O2 

GVL_CFLAGS=-I../include
GIL_CFLAGS=-I ../../gil/include
GVL_LIBS=-L../src -lgvl
GIL_LIBS=-L../../gil/src -lgil

CFLAGS+=-DDEBUG $(GIL_CFLAGS) $(GVL_CFLAGS)
LDFLAGS=$(GIL_LIBS) $(GVL_LIBS) -lpthread

check_video: check_video.cpp
	g++ -I/usr/local/include/ffmpeg $< -o $@ -L/usr/local/lib \
		-lavformat -lavcodec -lavutil -lz -lm -lpthread

test_picture: test_picture.cpp
	g++ -I/usr/local/include/ffmpeg $< -o $@ -L/usr/local/lib \
		-lavformat -lavcodec -lavutil -lz -lm -lpthread

extract_picture: extract_picture.cpp
	g++ -I/usr/local/include/ffmpeg $(GIL_CFLAGS) $< -o $@ -L/usr/local/lib \
		$(GIL_LIBS) -lavformat -lavcodec -lavutil -lz -lm -lpthread

test: test.cpp
	g++ $(CFLAGS) $(GIL_CFLAGS) $< -o $@ $(LDFLAGS) 

run: test
	env LD_LIBRARY_PATH="../src:../../gil/src" ./test ccf19No04hr.mp4

clean:
	rm -f test check_video
