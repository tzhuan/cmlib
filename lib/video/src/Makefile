DESTDIR =

PREFIX = $(DESTDIR)/usr
INSTALL_INCLUDE = $(PREFIX)/include/cmlib
INSTALL_LIB = $(PREFIX)/lib
TARGET = libcmlib-videostream.so
CMLIB_HEADERS = -I../../.. 
FFMPEG_HEADERS = `pkg-config --cflags libavdevice libavcodec libavformat libavutil libswscale`
FFMPEG_LIBS = `pkg-config --libs libavdevice libavcodec libavformat libavutil libswscale`
CXXFLAGS = -g $(CMLIB_HEADERS) $(FFMPEG_HEADERS) -DDEBUG -fPIC -Wall -pipe
LIBS = $(FFMPEG_LIBS)

SRCS = VideoFileStream.cpp

OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	@ echo Compiling $<
	@ $(CXX) -c $< -o $@ $(CXXFLAGS)

$(TARGET): $(OBJS)
	@ echo Building  $@
	$(CXX) -o $@ -shared $(OBJS) $(LIBS)

install: post-install

real-install: $(TARGET) install-header
	@ echo Installing $(TARGET) to $(INSTALL_LIB)
	@ install -m644 $(TARGET) $(INSTALL_LIB)
	@ echo done

post-install: real-install
	@ echo -n Running ldconfig...
	@ ldconfig

install-header:
	@ echo Export headers to $(INSTALL_INCLUDE)
	@ install -m755 -d $(INSTALL_INCLUDE)
	@ svn --force export ../include/cmlib $(INSTALL_INCLUDE)

clean:
	rm -f $(TARGET) $(OBJS) *.gch *.core

depend:
	makedepend -Y $(CMLIB_HEADERS) $(SRCS) 2>/dev/null
# DO NOT DELETE

VideoFileStream.o: ../../../cmlib/video/VideoFileStream.h
VideoFileStream.o: ../../../cmlib/video/VideoStream.h ../../../cmlib/common.h
VideoFileStream.o: ../../../cmlib/image.h ../../../cmlib/image/Image.h
VideoFileStream.o: ../../../cmlib/image/Color.h ../../../cmlib/image/Type.h
VideoFileStream.o: ../../../cmlib/image/TypeTrait.h
VideoFileStream.o: ../../../cmlib/image/ColorTrait.h
VideoFileStream.o: ../../../cmlib/image/ColorSelector.h
VideoFileStream.o: ../../../cmlib/image/Functional.h
VideoFileStream.o: ../../../cmlib/image/Algorithm.h
VideoFileStream.o: ../../../cmlib/image/Converter.h
VideoFileStream.o: ../../../cmlib/image/SubImage.h
VideoFileStream.o: ../../../cmlib/image/SliceImage.h
