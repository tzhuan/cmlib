HEADERS = -I../include
CXXFLAGS = $(HEADERS) -I/usr/local/include -fPIC -Wall -g -pipe
LIBS =    -L/usr/local/lib -lpng -ljpeg -ltiff -lz
TARGET = libgil2.so

CXXFLAGS += $(shell pkg-config --cflags OpenEXR)
LIBS += $(shell pkg-config --libs OpenEXR)

IO_DIR = io
UTIL_DIR = $(IO_DIR)/io_utils
IO_SRCS = $(IO_DIR)/exr.cpp $(IO_DIR)/jpeg.cpp $(IO_DIR)/png.cpp
IO_OBJS = $(IO_SRCS:.cpp=.o)

%.o: %.cpp
	@ echo Compiling $<
	@ $(CXX) -c $< -o $@ $(CXXFLAGS)

$(TARGET): $(IO_OBJS)
	@ echo Building  $@
	@ $(CXX) -o $@ -shared $(OBJS) $(IO_OBJS) $(UTIL_OBJS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJS) *.gch *.core
	@ cd $(IO_DIR) && $(MAKE) clean

depend:
	makedepend -Y $(HEADERS) $(IO_SRCS) 2>/dev/null
# DO NOT DELETE

io/exr.o: ../include/gil/io/exr.h ../include/gil/Image.h
io/exr.o: ../include/gil/Color.h ../include/gil/Converter.h
io/jpeg.o: ../include/gil/Color.h ../include/gil/io/jpeg.h
