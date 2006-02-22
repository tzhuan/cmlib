#include <iostream>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(void){
    ByteImage1 img;
    JpegReader r;
    read(img, "input.jpg", r);
    
    cout << "reading done, size: " << img.width() << 'x' << img.height() << endl;

    int h = img.height() / 2;
    
    for(size_t y = 0; y < h; y++)
	for(size_t x = 0; x < img.width(); x++)
	    swap( img(x, y), img(x, img.height()-y-1) );

    JpegWriter w;
    write(img, "output.jpg", w);

    return 0;
}
