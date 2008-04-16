CXXFLAGS = -I../include -I /usr/local/include
GIL_LIBS = -L../src -L../lib -lgil2
BOOST_LIBS = -L /usr/local/lib -lboost_unit_test_framework-gcc
LD_FLAGS = $(GIL_LIBS) $(BOOST_LIBS)

%.o: %.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(LD_FLAGS)

all: $(CONVERT) $(IO_TEST)

$(IO_TEST): $(IO_TEST).o 
	$(CXX) -o $@ $(IO_TEST).o $(LIBS)

$(SUBIMAGE_TEST): $(SUBIMAGE_TEST).o 
	$(CXX) -o $@ $(SUBIMAGE_TEST).o $(LIBS)

$(CONVERT): $(CONVERT).o
	$(CXX) -o $@ $(CONVERT).o $(LIBS)

test: $(IO_TEST)
	./$(IO_TEST)

clean:
	rm -rf $(CONVERT) $(IO_TEST) $(SUBIMAGE_TEST) *.o *.core 
	rm -rf ByteImage* FloatImage*
