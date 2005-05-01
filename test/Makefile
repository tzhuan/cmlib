CC=	gcc
CXX=	g++
CFLAGS+=    -I../src -Wall -g
CXXFLAGS+=  $(CFLAGS)
LDFLAGS=    -L../src -lgil2
IO_TEST=io-test
PIXEL_TEST=pixel-test
SRCS=	io-test.cc pixel-test.cc

all: clean $(IO_TEST) $(PIXEL_TEST)

$(IO_TEST): io-test.cc
	$(CXX) $(CXXFLAGS) -o $@ io-test.cc $(LDFLAGS)

$(PIXEL_TEST): pixel-test.cc
	$(CXX) $(CXXFLAGS) -o $@ pixel-test.cc $(LDFLAGS)

clean:
	rm -rf $(IO_TEST) $(PIXEL_TEST) *.core byte* float*
