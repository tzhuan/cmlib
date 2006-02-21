CXXFLAGS+=    -I/usr/local/include -I../include -fPIC -g -Wall
LIBS=    -L/usr/local/lib -lpng -ljpeg -ltiff -lz -lm
TARGET= libgil2.so

IO_DIR= io

.SUFFIXES: .cc

$(TARGET): $(OBJS)
	@echo Compiling $@
	@cd $(IO_DIR) && $(MAKE)
	@cd $(UTIL_DIR) && $(MAKE)
	$(CXX) $(CXXFLAGS) -shared -o $@ \
	    $(OBJS) $(IO_OBJS) $(UTIL_OBJS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS) *.gch *.core

clean-all: clean
	@cd $(IO_DIR) && $(MAKE) clean
	@cd $(UTIL_DIR) && $(MAKE) clean
