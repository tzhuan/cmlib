#ifndef __GIL_PIXEL_H__
#define __GIL_PIXEL_H__

// #include "Channels.h"
#include <algorithm>

namespace Gil {
    
    template<typename Type, size_t Channel>
    class BasicPixel {
	public:
	    typedef Type& RefType;
	    typedef const Type& ConstRefType;
	    typedef Type* PtrType;
	    typedef const Type& ConstPtrType;
	    typedef BasicPixel<Type, Channel> Self;
	    typedef const BasicPixel<Type, Channel> ConstSelf;

	    // default constructor, do nothing
	    BasicPixel(){}

	    BasicPixel(ConstRefType c){ std::fill(m_data, m_data+Channel, c); }

	    BasicPixel(ConstPtrType c){ std::copy(c, c+Channel, m_data); }

	    RefType operator [](int i){ return m_data[i]; }
	    
	    ConstRefType operator [](int i) const { return m_data[i]; }

	    Self& color(){ return *this; } // will be used later
	    
	    ConstSelf& color() const { return *this; } // will be used later

	protected:
	    Type m_data[Channel];
    };

    template <typename Type>
    class AlphaPixel{
	public:
	    Type& alpha(){ return m_alpha; }
	    const Type& alpha(){ return m_alpha; }

	protected:
	    Type m_alpha;
    };

    // selection template, from Loki
    template <typename T1, typename T2, bool S>
    struct Select{
	typedef T1 Result;
    };

    template <typename T1, typename T2>
    struct Select<T1, T2, true>{
	typedef T2 Result;
    };

    struct EmptyType{};

    template <typename Type, size_t Channel, bool HasAlpha = false>
    class Pixel
	: public BasicPixel<Type, Channel>,
	  public Select<EmptyType, AlphaPixel<Type>, HasAlpha>::Result
    {
	    
    };

    // utilities to make pixel
    // Pixel with 3 channels or 4 channels are most commonly used.
    /*
    template<typename Type>
    Pixel<Type, 3> make_pixel(const Type& c1, const Type& c2, const Type& c3){
	Pixel<Type, 3> p;
	p[0] = c1;
	p[1] = c2;
	p[2] = c3;
	return p;
    }
    
    template<typename Type>
    Pixel<Type, 4> make_pixel(const Type& c1, const Type& c2, const Type& c3, const Type& c4){
	Pixel<Type, 4> p;
	p[0] = c1;
	p[1] = c2;
	p[2] = c3;
	p[3] = c4;
	return p;
    }*/
}

#endif // __GIL_PIXEL_H__
