INSTALL_INCLUDE = /usr/local/include
INSTALL_LIB = /usr/local/lib
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

install: $(TARGET)
	@ echo Export headers to $(INSTALL_INCLUDE)
	@ svn --force export ../include/gil $(INSTALL_INCLUDE)/gil
	@ echo Installing $(TARGET) to $(INSTALL_LIB)
	@ install -m644 $(TARGET) $(INSTALL_LIB)
	@ echo -n Running ldconfig...
	@ ldconfig
	@ echo done

clean:
	rm -f $(TARGET) $(OBJS) *.gch *.core
	@ cd $(IO_DIR) && $(MAKE) clean

depend:
	makedepend -Y $(HEADERS) $(SRCS) $(IO_SRCS) 2>/dev/null
# DO NOT DELETE

io/jpeg.o: ../include/gil/core/io/jpeg.h ../include/gil/core/Exception.h
io/jpeg.o: ../include/gil/core/Color.h ../include/gil/core/Converter.h
io/jpeg.o: ../include/gil/core/Color.h
io/png.o: ../include/gil/core/Exception.h ../include/gil/core/io/png.h
io/png.o: ../include/gil/core/Exception.h ../include/gil/core/Color.h
io/png.o: ../include/gil/core/Converter.h ../include/gil/core/Color.h
io/tiff.o: ../include/gil/core/io/tiff.h ../include/gil/core/Exception.h
io/tiff.o: ../include/gil/core/Color.h ../include/gil/core/Converter.h
io/tiff.o: ../include/gil/core/Color.h
io/exr.o: ../include/gil/core/Exception.h ../include/gil/core/io/exr.h
io/exr.o: ../include/gil/core/Image.h ../include/gil/core/Color.h
io/exr.o: ../include/gil/core/Converter.h
io/hdr.o: ../include/gil/core/Exception.h ../include/gil/core/io/hdr.h
io/hdr.o: ../include/gil/core/Color.h ../include/gil/core/Converter.h
io/hdr.o: ../include/gil/core/Color.h
io/crw.o: ../include/gil/core/Exception.h ../include/gil/core/io/crw.h
io/crw.o: ../include/gil/core/Color.h ../include/gil/core/Converter.h
io/crw.o: ../include/gil/core/Color.h
io/bmp.o: ../include/gil/core/io/bmp.h ../include/gil/core/Exception.h
io/bmp.o: ../include/gil/core/Color.h ../include/gil/core/Converter.h
io/bmp.o: ../include/gil/core/Color.h ../include/gil/core/Exception.h
