CXXFLAGS+=    -I/usr/local/include

SRCS = exr.cpp

OBJS = ${SRCS:.cpp=.o}

.SUFFIXES: .cpp

io: $(OBJS)
	@echo $@ is made

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) *.gch *.core
