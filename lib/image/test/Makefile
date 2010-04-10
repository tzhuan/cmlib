CFLAGS = -I../include -I../../common/include -I /usr/local/include -Wall -O3 -g
GIL_LIBS = -L../src -L../lib -lcmlib-imageio
BOOST_LIBS = -L /usr/local/lib -lboost_unit_test_framework-gcc
LIBS = $(GIL_LIBS)

CONVERT = convert
TEST_IO = test-io

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ 

all: $(CONVERT) $(TEST_IO)

$(TEST_IO): $(TEST_IO).o 
	$(CXX) $(TEST_IO).o -o $@ $(LIBS)

$(SUBIMAGE_TEST): $(SUBIMAGE_TEST).o 
	$(CXX) $(SUBIMAGE_TEST).o -o $@ $(LIBS)

$(CONVERT): $(CONVERT).o
	$(CXX) $(CONVERT).o -o $@ $(LIBS)

test: $(TEST_IO)
	./$(TEST_IO)

test-imageoperation: test-imageoperation.o
	$(CXX) $< -o $@ 

test-typeselector: test-typeselector.o
	$(CXX) $< -o $@ 

test-algorithm: test-algorithm.o
	$(CXX) $< -o $@ 

test-image: test-image.o
	$(CXX) $< -o $@

test-color: test-color.o
	$(CXX) $< -o $@

test-color-output: test-color-output.o
	$(CXX) $< -o $@

test-converter: test-converter.o
	$(CXX) $< -o $@

clean:
	rm -rf $(CONVERT) $(TEST_IO) $(SUBIMAGE_TEST) *.o *.core 
	rm -rf ByteImage* FloatImage*
