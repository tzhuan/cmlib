/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/**
 * @file test-hsv.cpp
 * @author Yili Zhao <panovr@gmail.com>
 */

#include <iostream>
#include <cmlib/image.h>
#include <cmlib/dip.h>

using namespace std;
using namespace cmlib::image;
using namespace cmlib::dip;

/**
 * rgb of 64 46 120
 * in gimp the hsv shows as 254 62 47
 * in opencv the hsv shows as 127 157 120
 * gimp ranges are:
 * h 0 - 360
 * s 0 - 100
 * v 0 - 100
 *
 * opencv ranges seem to be
 * h 0 - 180
 * s 0 - 255
 * v 0 - 255
 *
 * so working out
 * 127/180 * 360 = 254
 * 157/255 * 100 = 62
 * 120/255 * 100 = 47
 *
 * My implementation will use h: [0, 360), s: [0.0, 10], v[0.0, 1.0],
 * and convert them to [0, 255] of ByteImage3 or [0.0, 1.0] of FloatImage3.
 */

int main(int argc, char *argv[])
{
    ByteImage3 src(3, 1);
    src(0,0)[0] = 64;  src(0,0)[1] = 46;  src(0,0)[2] = 120;
    src(1,0)[0] = 112; src(1,0)[1] = 172; src(1,0)[2] = 182;
    src(2,0)[0] = 93;  src(2,0)[1] = 188; src(2,0)[2] = 210;
    std::cout << "src: " << src << std::endl;

    std::cout << "rgb_to_hsv: ByteImage3 -> ByteImage3" << std::endl;
    ByteImage3 bdst(3, 1);
    rgb_to_hsv(src, bdst);
    std::cout << "bdst: " << bdst << std::endl;
    std::cout << "hsv_to_rgb: ByteImage3 -> ByteImage3" << std::endl;
    ByteImage3 brgb(3, 1);
    hsv_to_rgb(bdst, brgb);
    std::cout << "brgb: " << brgb << std::endl;

    std::cout << "rgb_to_hsv: ByteImage3 -> FloatImage3" << std::endl;
    FloatImage3 fdst(3, 1);
    rgb_to_hsv(src, fdst);
    std::cout << "fdst: " << fdst << std::endl;
    std::cout << "hsv_to_rgb: FloatImage3 -> ByteImage3" << std::endl;
    hsv_to_rgb(fdst, brgb);
    std::cout << "brgb: " << brgb << std::endl;

    std::cout << "rgb_to_hsv: FloatImage3 -> ByteImage3" << std::endl;
    FloatImage3 frgb(1, 1);
    frgb(0,0)[0] = 0.25; frgb(0,0)[1] = 0.18; frgb(0,0)[2] = 0.47;
    std::cout << "frgb: " << frgb << std::endl;
    ByteImage3 bhsv(1, 1);
    rgb_to_hsv(frgb, bhsv);
    std::cout << "bhsv: " << bhsv << std::endl;
    std::cout << "hsv_to_rgb: ByteImage3 -> FloatImage3" << std::endl;
    brgb.resize(1,1);
    hsv_to_rgb(bhsv, brgb);
    std::cout << "ByteImage3 RGB: " << brgb << std::endl;
    
    return 0;
}
