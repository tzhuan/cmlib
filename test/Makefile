CXXFLAGS = -I../include 
LIBS = -L../src -lgil2

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -Wall -O3

test: test.o ../src/libgil2.so
	$(CXX) -o $@ test.o $(LIBS)

clean:
	rm -rf $(IO_TEST) $(PIXEL_TEST) *.core byte* float*
