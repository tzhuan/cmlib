CFLAGS = -I../../.. -I /usr/local/include -Wall -O3 -g
GIL_LIBS = -L../src -L../lib -L../../image/src -lcmlib-imageio
BOOST_LIBS = -L /usr/local/lib -lboost_unit_test_framework-gcc
LIBS = $(GIL_LIBS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ 

test-colorspace: test-colorspace.o
	$(CXX) $< -o $@

clean:
	rm -rf *.o *.core 
	rm -rf ByteImage* FloatImage*
