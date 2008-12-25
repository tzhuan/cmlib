CFLAGS = -I../include -I /usr/local/include
GIL_LIBS = -L../src -L../lib -lcmlib-imageio
BOOST_LIBS = -L /usr/local/lib -lboost_unit_test_framework-gcc
LIBS = $(GIL_LIBS)

CONVERT = convert
IO_TEST = io-test

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ 

all: $(CONVERT) $(IO_TEST)

$(IO_TEST): $(IO_TEST).o 
	$(CXX) $(IO_TEST).o -o $@ $(LIBS)

$(SUBIMAGE_TEST): $(SUBIMAGE_TEST).o 
	$(CXX) $(SUBIMAGE_TEST).o -o $@ $(LIBS)

$(CONVERT): $(CONVERT).o
	$(CXX) $(CONVERT).o -o $@ $(LIBS)

test: $(IO_TEST)
	./$(IO_TEST)

clean:
	rm -rf $(CONVERT) $(IO_TEST) $(SUBIMAGE_TEST) *.o *.core 
	rm -rf ByteImage* FloatImage*
