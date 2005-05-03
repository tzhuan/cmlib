CFLAGS+=    -I/usr/local/include -fPIC -g -Wall
CXXFLAGS+=  $(CFLAGS) -D__UNIX__ -DTHREADS -DUSE_LIBPNG
LDFLAGS=    -L/usr/local/lib -lpng -ljpeg -ltiff -lz -lm
TARGET= libgil2.so

IO_DIR= io
UTIL_DIR=$(IO_DIR)/io_utils
UTIL_OBJS=$(UTIL_DIR)/color.o $(UTIL_DIR)/colrops.o \
	$(UTIL_DIR)/header.o $(UTIL_DIR)/resolu.o $(UTIL_DIR)/dcraw.o\
	$(UTIL_DIR)/logImageCore.o $(UTIL_DIR)/dpxlib.o \
	$(UTIL_DIR)/logImageLib.o
IO_OBJS=$(IO_DIR)/bmp.o $(IO_DIR)/hdrio.o $(IO_DIR)/jpg.o \
	$(IO_DIR)/crw.o $(IO_DIR)/pfm.o $(IO_DIR)/uve.o \
	$(IO_DIR)/pngio.o $(IO_DIR)/ppm.o $(IO_DIR)/tga.o \
	$(IO_DIR)/tif.o $(IO_DIR)/dpx.o $(IO_UTILS_OBJS)
OBJS=	FileFormat.o


.SUFFIXES: .cc

$(TARGET): $(OBJS)
	@echo Compiling $@
	@cd $(IO_DIR) && $(MAKE)
	@cd $(UTIL_DIR) && $(MAKE)
	$(CXX) $(CXXFLAGS) -shared -o $@ \
	    $(OBJS) $(IO_OBJS) $(UTIL_OBJS) $(LDFLAGS)
check:
	g++ -c -Wall Gil.h

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS) *.gch *.core

clean-all: clean
	@cd $(IO_DIR) && $(MAKE) clean
	@cd $(UTIL_DIR) && $(MAKE) clean
