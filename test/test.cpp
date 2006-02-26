#include <iostream>
#include <cstdio>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(void){
    ByteImage1 img;
    read(img, "input.hdr");
    write(img, "output.png");
    return 0;
}
