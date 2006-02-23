HEADERS = -I../include
CXXFLAGS = $(HEADERS) -I/usr/local/include -fPIC -Wall -g -pipe
LIBS =    -L/usr/local/lib -lpng -ljpeg -ltiff -lz
TARGET = libgil2.so

CXXFLAGS += $(shell pkg-config --cflags OpenEXR)
LIBS += $(shell pkg-config --libs OpenEXR)

IO_DIR = io
UTIL_DIR = $(IO_DIR)/io_utils
IO_SRCS = $(IO_DIR)/exr.cpp $(IO_DIR)/jpeg.cpp $(IO_DIR)/png.cpp $(IO_DIR)/tiff.cpp $(IO_DIR)/crw.cpp
IO_OBJS = $(IO_SRCS:.cpp=.o) 
IO_OBJS += $(IO_DIR)/dcraw.o

%.o: %.cpp
	@ echo Compiling $<
	@ $(CXX) -c $< -o $@ $(CXXFLAGS)

$(TARGET): $(IO_OBJS) 
	@ echo Building  $@
	@ $(CXX) -o $@ -shared $(OBJS) $(IO_OBJS) $(UTIL_OBJS) $(LIBS)

$(IO_DIR)/dcraw.o: $(IO_DIR)/dcraw.c
	@ echo Compiling $<
	@ $(CC) -c $< -o $@ 

clean:
	rm -f $(TARGET) $(OBJS) *.gch *.core
	@ cd $(IO_DIR) && $(MAKE) clean

depend:
	makedepend -Y $(HEADERS) $(IO_SRCS) 2>/dev/null
# DO NOT DELETE

io/exr.o: ../include/gil/io/exr.h ../include/gil/Image.h
io/exr.o: ../include/gil/Color.h ../include/gil/Converter.h
io/jpeg.o: ../include/gil/io/jpeg.h ../include/gil/Color.h
io/png.o: ../include/gil/io/png.h ../include/gil/Color.h
io/tiff.o: ../include/gil/io/tiff.h ../include/gil/Color.h
io/crw.o: io/dcraw.o ../include/gil/io/crw.h ../include/gil/Color.h
