#include <iostream>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(void){
    FloatImage3 test;
    ExrReader r;
    read(test, "input.exr", r);

    int h = test.height() / 2;
    
    for(size_t y = 0; y < h; y++)
	for(size_t x = 0; x < test.width(); x++)
	    swap( test(x, y), test(x, test.height()-y-1) );

    ExrWriter w;
    write(test, "output.exr", w);

    return 0;
}
