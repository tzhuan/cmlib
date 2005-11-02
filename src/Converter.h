#ifndef __CONVERT_H__
#define __CONVERT_H__

#include "Pixel.h"

namespace Gil {

    template<typename FromType, typename ToType>
	struct TypeConverter {
	    const ToType operator() (const FromType& c) const
	    {
		return static_cast<ToType>(c);
	    }
	};

    template<>
	struct TypeConverter<unsigned char, float> {
	    const float operator() (const unsigned char& c) const
	    {
		return c/255.0f;
	    }
	    const float max() const { return 1.0f; }
	};

    template<>
	struct TypeConverter<unsigned char, unsigned char> {
	    const unsigned char operator() (const unsigned char& c) const
	    {
		return c;
	    }
	    const unsigned char max() const { return 255; }
	};

    template<>
	struct TypeConverter<float, unsigned char> {
	    const unsigned char operator() (const float& c) const
	    {
		return static_cast<unsigned char>(c*255);
	    }
	    const unsigned char max() const { return 255; }
	};

    template<>
	struct TypeConverter<float, float> {
	    const float operator() (const float& c) const
	    {
		return c;
	    }
	    const float max() const { return 1.0f; }
	};

    template<typename FromType, typename ToType,
	Channels FromChannels, Channels ToChannels>
	    struct Converter {
		const Pixel<ToType, ToChannels> operator() (const FromType* c) const
		{
		    ToType c_[ToChannels];
		    TypeConverter<FromType, ToType> tc;
		    for (size_type i = 0; i < ToChannels; ++i)
			c_[i] = tc(c[i]);
		    return Pixel<ToType, ToChannels>(c_);
		}
	    };

    template<typename FromType, typename ToType>
	struct Converter<FromType, ToType, OneChannel, ThreeChannels> {
	    const Pixel<ToType, ThreeChannels> operator() (const FromType* c) const
	    {
		TypeConverter<FromType, ToType> conv;
		return Pixel<ToType, ThreeChannels>(conv(*c), conv(*c), conv(*c));
	    }
	};

    template<typename FromType, typename ToType>
	struct Converter<FromType, ToType, OneChannel, FourChannels> {
	    const Pixel<ToType, FourChannels> operator() (const FromType* c) const
	    {
		TypeConverter<FromType, ToType> conv;
		return Pixel<ToType, FourChannels>(
			conv(*c), conv(*c), conv(*c), conv.max());
	    }
	};

    template<typename FromType, typename ToType>
	struct Converter<FromType, ToType, ThreeChannels, OneChannel> {
	    const Pixel<ToType, OneChannel> operator() (const FromType* c) const
	    {
		// R:G:B = 2:5:1 = 0.25 : 0.625 : 0.125
		TypeConverter<FromType, ToType> conv;
		return Pixel<ToType, OneChannel>(static_cast<ToType>
			(0.25*conv(c[0])+0.625*conv(c[1])+0.125*conv(c[2])));
	    }
	};

    template<typename FromType, typename ToType>
	struct Converter<FromType, ToType, ThreeChannels, FourChannels> {
	    const Pixel<ToType, FourChannels> operator() (const FromType* c) const
	    {
		TypeConverter<FromType, ToType> conv;
		return Pixel<ToType, FourChannels>(
			conv(c[0]), conv(c[1]), conv(c[2]), conv.max());
	    }
	};

    template<typename FromType, typename ToType>
	struct Converter<FromType, ToType, FourChannels, OneChannel> {
	    const Pixel<ToType, OneChannel> operator() (const FromType* c) const
	    {
		// R:G:B = 2:5:1 = 0.25 : 0.625 : 0.125
		TypeConverter<FromType, ToType> conv;
		return Pixel<ToType, OneChannel>(static_cast<ToType>
			(0.25*conv(c[0])+0.625*conv(c[1])+0.125*conv(c[2])));
	    }
	};

    template<typename FromType, typename ToType>
	struct Converter<FromType, ToType, FourChannels, ThreeChannels> {
	    const Pixel<ToType, ThreeChannels> operator() (const FromType* c) const
	    {
		TypeConverter<FromType, ToType> conv;
		return Pixel<ToType, ThreeChannels>(conv(c[0]), conv(c[1]), conv(c[2]));
	    }
	};

}

#endif
