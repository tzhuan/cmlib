CXXFLAGS = -I../include 
LIBS = -L../src -lgil2

%.o: %.c
	$(CXX) -c $< $(CXXFLAGS)

test: test.o ../src/libgil2.so
	$(CXX) -o $@ test.o $(LIBS)

clean:
	rm -rf $(IO_TEST) $(PIXEL_TEST) *.core byte* float*
