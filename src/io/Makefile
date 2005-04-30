CXXFLAGS+=  $(CFLAGS) -D__UNIX__ -DTHREADS -DUSE_LIBPNG

SRCS = bmp.cc hdrio.cc jpg.cc pngio.cc tif.cc ppm.cc tga.cc crw.cc \
       pfm.cc uve.cc dpx.cc

OBJS = ${SRCS:.cc=.o}

.SUFFIXES: .cpp

io:   $(OBJS) 
	@echo $@ is made

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *.d core *.bak err
