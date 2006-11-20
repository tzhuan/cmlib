CXXFLAGS = -I../include 
LIBS = -L../src -L../lib -lgil2
IO_TEST = io-test
SUBIMAGE_TEST = subimage-test
CONVERT = convert

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -Wall -g

all: $(CONVERT) $(IO_TEST)

$(IO_TEST): $(IO_TEST).o #../src/libgil2.so
	$(CXX) -o $@ $(IO_TEST).o $(LIBS)

$(SUBIMAGE_TEST): $(SUBIMAGE_TEST).o 
	$(CXX) -o $@ $(SUBIMAGE_TEST).o $(LIBS)

$(CONVERT): $(CONVERT).o #../src/libgil2.so
	$(CXX) -o $@ $(CONVERT).o $(LIBS)

test: $(IO_TEST)
	./$(IO_TEST)

clean:
	rm -rf $(CONVERT) $(IO_TEST) $(SUBIMAGE_TEST) *.o *.core 
	rm -rf ByteImage* FloatImage*
