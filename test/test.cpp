#include <iostream>
#include <cstdio>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(void){
    FloatImage3 img;
    read(img, "input.jpg");
    write(img, "output.tif");
    return 0;
}
