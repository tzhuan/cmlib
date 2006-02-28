#include <iostream>
#include <cstdio>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(void){
    Float3 a(0.0, 0.2, 0.4), b(1.0, 0.9, 0.6);
    Byte3 c(0, 30, 60), d(240, 120, 255);
    cout << mix(a, b, 0.5) << endl;
    cout << mix(c, d, (Byte1)127) << endl;

    cout << mix(c, d, 0.5) << endl;
    
    //cout << (int)TypeTrait<Byte1>::mix(0, 240, 127) << endl;
    return 0;
}
