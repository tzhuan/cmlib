# Copyright NTU CSIE CMLAB 2005 - 2012
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file ../../../LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

CXXFLAGS = -I../../.. -I /usr/local/include -Wall -O3 -g
CMLIB_LIBS = -L../src -L../lib -L../../image/src -lcmlib-imageio
BOOST_LIBS = -L /usr/local/lib -lboost_unit_test_framework-gcc
LIBS = $(CMLIB_LIBS)

#%.o: %.cpp
#	$(CXX) $(CFLAGS) -c $< -o $@ 
#
#test-colorspace: test-colorspace.o
#	$(CXX) $< -o $@

clean:
	rm -rf *.o *.core 
	rm -rf ByteImage* FloatImage*
