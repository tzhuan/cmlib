CXXFLAGS = -I../include 
LIBS = -L../src -lgil2
IO_TEST = io-test
CONVERT = convert

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -Wall -O3

all: $(CONVERT) $(IO_TEST)

$(IO_TEST): $(IO_TEST).o ../src/libgil2.so
	$(CXX) -o $@ $(IO_TEST).o $(LIBS)

$(CONVERT): $(CONVERT).o ../src/libgil2.so
	$(CXX) -o $@ $(CONVERT).o $(LIBS)

test:
	./$(IO_TEST)

clean:
	rm -rf $(CONVERT) $(IO_TEST) *.o *.core 
	rm -rf ByteImage* FloatImage*
