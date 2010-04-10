DESTDIR =

PREFIX = $(DESTDIR)/usr
INSTALL_INCLUDE = $(PREFIX)/include
INSTALL_LIB = $(PREFIX)/lib
TARGET = libcmlib-imageio.so
HEADERS = -I ../../.. 
CXXFLAGS = -g $(HEADERS) -I /usr/local/include -fPIC -Wall -pipe
LIBS =    -L/usr/local/lib -lpng -ljpeg -ltiff -lz

CXXFLAGS += $(shell pkg-config --cflags OpenEXR)
LIBS += $(shell pkg-config --libs OpenEXR)

IO_DIR = io
UTIL_DIR = $(IO_DIR)/io_utils
SRCS = #FileFormat.cpp
IO_SRCS = $(IO_DIR)/jpeg.cpp $(IO_DIR)/png.cpp $(IO_DIR)/tiff.cpp \
	  $(IO_DIR)/exr.cpp $(IO_DIR)/hdr.cpp $(IO_DIR)/bmp.cpp #$(IO_DIR)/crw.cpp

OBJS = $(SRCS:.cpp=.o)
IO_OBJS = $(IO_SRCS:.cpp=.o)
#IO_OBJS += $(IO_DIR)/dcraw.o

%.o: %.cpp
	@ echo Compiling $<
	@ $(CXX) -c $< -o $@ $(CXXFLAGS)

$(TARGET): $(OBJS) $(IO_OBJS)
	@ echo Building  $@
	@ $(CXX) -o $@ -shared $(OBJS) $(IO_OBJS) $(UTIL_OBJS) $(LIBS)

$(IO_DIR)/dcraw.o: $(IO_DIR)/dcraw.c
	@ echo Compiling $<
	@ $(CC) -c $< -o $@ $(CXXFLAGS)

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
	@ svn --force export ../include/cmlib $(INSTALL_INCLUDE)/cmlib

clean:
	rm -f $(TARGET) $(OBJS) *.gch *.core
	@ cd $(IO_DIR) && $(MAKE) clean

depend:
	makedepend -Y $(HEADERS) $(SRCS) $(IO_SRCS) 2>/dev/null
# DO NOT DELETE

io/jpeg.o: ../../../cmlib/imageio/io/jpeg.h ../../../cmlib/image/Converter.h
io/jpeg.o: ../../../cmlib/image/Color.h ../../../cmlib/image/Type.h
io/jpeg.o: ../../../cmlib/image/TypeTrait.h ../../../cmlib/common.h
io/jpeg.o: ../../../cmlib/image/ColorTrait.h
io/jpeg.o: ../../../cmlib/image/ColorSelector.h
io/jpeg.o: ../../../cmlib/image/Functional.h ../../../cmlib/image/Algorithm.h
io/jpeg.o: ../../../cmlib/imageio/Exception.h
io/png.o: ../../../cmlib/imageio/Exception.h ../../../cmlib/imageio/io/png.h
io/png.o: ../../../cmlib/image/Converter.h ../../../cmlib/image/Color.h
io/png.o: ../../../cmlib/image/Type.h ../../../cmlib/image/TypeTrait.h
io/png.o: ../../../cmlib/common.h ../../../cmlib/image/ColorTrait.h
io/png.o: ../../../cmlib/image/ColorSelector.h
io/png.o: ../../../cmlib/image/Functional.h ../../../cmlib/image/Algorithm.h
io/png.o: ../../../cmlib/imageio/Exception.h
io/tiff.o: ../../../cmlib/imageio/io/tiff.h ../../../cmlib/image/Converter.h
io/tiff.o: ../../../cmlib/image/Color.h ../../../cmlib/image/Type.h
io/tiff.o: ../../../cmlib/image/TypeTrait.h ../../../cmlib/common.h
io/tiff.o: ../../../cmlib/image/ColorTrait.h
io/tiff.o: ../../../cmlib/image/ColorSelector.h
io/tiff.o: ../../../cmlib/image/Functional.h ../../../cmlib/image/Algorithm.h
io/tiff.o: ../../../cmlib/imageio/Exception.h
io/exr.o: ../../../cmlib/imageio/Exception.h ../../../cmlib/imageio/io/exr.h
io/exr.o: ../../../cmlib/image/Converter.h ../../../cmlib/image/Color.h
io/exr.o: ../../../cmlib/image/Type.h ../../../cmlib/image/TypeTrait.h
io/exr.o: ../../../cmlib/common.h ../../../cmlib/image/ColorTrait.h
io/exr.o: ../../../cmlib/image/ColorSelector.h
io/exr.o: ../../../cmlib/image/Functional.h ../../../cmlib/image/Algorithm.h
io/exr.o: ../../../cmlib/imageio/Exception.h
io/hdr.o: ../../../cmlib/imageio/Exception.h ../../../cmlib/imageio/io/hdr.h
io/hdr.o: ../../../cmlib/image/Converter.h ../../../cmlib/image/Color.h
io/hdr.o: ../../../cmlib/image/Type.h ../../../cmlib/image/TypeTrait.h
io/hdr.o: ../../../cmlib/common.h ../../../cmlib/image/ColorTrait.h
io/hdr.o: ../../../cmlib/image/ColorSelector.h
io/hdr.o: ../../../cmlib/image/Functional.h ../../../cmlib/image/Algorithm.h
io/bmp.o: ../../../cmlib/imageio/io/bmp.h ../../../cmlib/image/Converter.h
io/bmp.o: ../../../cmlib/image/Color.h ../../../cmlib/image/Type.h
io/bmp.o: ../../../cmlib/image/TypeTrait.h ../../../cmlib/common.h
io/bmp.o: ../../../cmlib/image/ColorTrait.h
io/bmp.o: ../../../cmlib/image/ColorSelector.h
io/bmp.o: ../../../cmlib/image/Functional.h ../../../cmlib/image/Algorithm.h
io/bmp.o: ../../../cmlib/imageio/Exception.h
io/bmp.o: ../../../cmlib/imageio/Exception.h
