#include <iostream>
#include <cstdio>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(void){
    ByteImage4 img;
    read(img, "input.png");
    write(img, "output.tif");
    return 0;
}
