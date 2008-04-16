DESTDIR =

PREFIX = $(DESTDIR)/usr
INSTALL_INCLUDE = $(PREFIX)/include
INSTALL_LIB = $(PREFIX)/lib
TARGET = libgil2.so
HEADERS = -I../include
CXXFLAGS = -g $(HEADERS) -I/usr/local/include -fPIC -Wall -pipe
LIBS =    -L/usr/local/lib -lpng -ljpeg -ltiff -lz

CXXFLAGS += $(shell pkg-config --cflags OpenEXR)
LIBS += $(shell pkg-config --libs OpenEXR)

IO_DIR = io
UTIL_DIR = $(IO_DIR)/io_utils
SRCS = #FileFormat.cpp
IO_SRCS = $(IO_DIR)/jpeg.cpp $(IO_DIR)/png.cpp $(IO_DIR)/tiff.cpp \
	  $(IO_DIR)/exr.cpp $(IO_DIR)/hdr.cpp $(IO_DIR)/crw.cpp $(IO_DIR)/bmp.cpp

OBJS = $(SRCS:.cpp=.o)
IO_OBJS = $(IO_SRCS:.cpp=.o)
IO_OBJS += $(IO_DIR)/dcraw.o

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
	@ svn --force export ../include/gil $(INSTALL_INCLUDE)/gil

clean:
	rm -f $(TARGET) $(OBJS) *.gch *.core
	@ cd $(IO_DIR) && $(MAKE) clean

depend:
	makedepend -Y $(HEADERS) $(SRCS) $(IO_SRCS) 2>/dev/null
# DO NOT DELETE

io/jpeg.o: ../include/gil/io/jpeg.h ../include/gil/Exception.h
io/jpeg.o: ../include/gil/Color.h ../include/gil/Converter.h
io/jpeg.o: ../include/gil/Color.h
io/png.o: ../include/gil/Exception.h ../include/gil/io/png.h
io/png.o: ../include/gil/Exception.h ../include/gil/Color.h
io/png.o: ../include/gil/Converter.h ../include/gil/Color.h
io/tiff.o: ../include/gil/io/tiff.h ../include/gil/Exception.h
io/tiff.o: ../include/gil/Color.h ../include/gil/Converter.h
io/tiff.o: ../include/gil/Color.h
io/exr.o: ../include/gil/Exception.h ../include/gil/io/exr.h
io/exr.o: ../include/gil/Image.h ../include/gil/Color.h
io/exr.o: ../include/gil/ImageProxy.h ../include/gil/Converter.h
io/hdr.o: ../include/gil/Exception.h ../include/gil/io/hdr.h
io/hdr.o: ../include/gil/Color.h ../include/gil/Converter.h
io/hdr.o: ../include/gil/Color.h
io/crw.o: ../include/gil/Exception.h ../include/gil/io/crw.h
io/crw.o: ../include/gil/Color.h ../include/gil/Converter.h
io/crw.o: ../include/gil/Color.h
io/bmp.o: ../include/gil/io/bmp.h ../include/gil/Exception.h
io/bmp.o: ../include/gil/Color.h ../include/gil/Converter.h
io/bmp.o: ../include/gil/Color.h ../include/gil/Exception.h
