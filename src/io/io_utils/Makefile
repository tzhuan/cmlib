CXXFLAGS+=$(CFLAGS) -D__UNIX__ -DTHREADS -DUSE_LIBPNG

SRCS = color.cc colrops.cc header.cc resolu.cc dcraw.cc \
       logImageCore.cc dpxlib.cc logImageLib.cc

OBJS = ${SRCS:.cc=.o}

.SUFFIXES: .cc

io_utils:   $(OBJS) 
	@echo $@ is made

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *.d core *.bak err
