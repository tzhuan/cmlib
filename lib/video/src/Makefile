DESTDIR =

PREFIX = $(DESTDIR)/usr
INSTALL_INCLUDE = $(PREFIX)/include/cmlib
INSTALL_LIB = $(PREFIX)/lib
TARGET = libcmlib-videostream.so
HEADERS = -I../include -I/usr/include/ffmpeg -I/usr/local/include/ffmpeg \
		  -I../../image/include -I../../common/include \
		  `pkg-config --cflags libswscale`
CXXFLAGS = -g $(HEADERS) -DDEBUG -I/usr/local/include -fPIC -Wall -pipe
LIBS =    -L/usr/local/lib -lavformat -lavcodec -lavutil -lz -lm -lpthread `pkg-config --libs libswscale`

CXXFLAGS += $(shell pkg-config --cflags OpenEXR)
LIBS += $(shell pkg-config --libs OpenEXR)

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
	makedepend -Y $(HEADERS) $(SRCS) 2>/dev/null
# DO NOT DELETE

VideoFileStream.o: ../include/cmlib/video/VideoFileStream.h
VideoFileStream.o: ../include/cmlib/video/VideoStream.h
