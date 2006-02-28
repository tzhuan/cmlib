#include <iostream>
#include <cstdio>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(void){
    ByteImage3 img;
    read(img, "input.exr");
    write(img, "output.jpg");
    return 0;
}
