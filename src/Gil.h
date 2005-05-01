#ifndef __GIL_H__
#define __GIL_H__

#include "Channels.h"
#include "Pixel.h"
#include "Image.h"

namespace Gil {
    // instantiation
    template class Image<unsigned char, OneChannel>;
    template class Image<unsigned char, ThreeChannels>;
    template class Image<unsigned char, FourChannels>;
    template class Image<float, OneChannel>;
    template class Image<float, ThreeChannels>;
    template class Image<float, FourChannels>;

    // alias
    typedef Image<unsigned char, OneChannel>	ByteImage1;
    typedef Image<unsigned char, ThreeChannels>	ByteImage3;
    typedef Image<unsigned char, FourChannels>	ByteImage4;
    typedef Image<float, OneChannel>		FloatImage1;
    typedef Image<float, ThreeChannels>		FloatImage3;
    typedef Image<float, FourChannels>		FloatImage4;

    template class Pixel<unsigned char, OneChannel>;
    template class Pixel<unsigned char, ThreeChannels>;
    template class Pixel<unsigned char, FourChannels>;
    template class Pixel<float, OneChannel>;
    template class Pixel<float, ThreeChannels>;
    template class Pixel<float, FourChannels>;

    typedef Pixel<unsigned char, OneChannel>	BytePixel1;
    typedef Pixel<unsigned char, ThreeChannels>	BytePixel3;
    typedef Pixel<unsigned char, FourChannels>	BytePixel4;
    typedef Pixel<float, OneChannel>		FloatPixel1;
    typedef Pixel<float, ThreeChannels>		FloatPixel3;
    typedef Pixel<float, FourChannels>		FloatPixel4;
}

#endif
