#include "gil/gil.h"

using namespace gil;

int main(void){
    FloatImage3 img;
    read(img, "input.jpg");
    write(img, "output.png");
    return 0;
}
