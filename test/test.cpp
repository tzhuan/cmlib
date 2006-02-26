#include <iostream>
#include <cstdio>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(void){
    FloatImage4 img;
    read(img, "input.hdr");
    write(img, "output.tiff");
    return 0;
}
