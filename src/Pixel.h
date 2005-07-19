#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <algorithm>

namespace Gil{
    
    template <typename Type, size_t Channel>
    class Pixel{
	public:
	    typedef Pixel<Type, Channel> PixelType; // will be used later
	    typedef Type& RefType;
	    typedef const Type& ConstRefType;
	    typedef Type* PtrType;
	    typedef const Type& ConstPtrType;

	    // default constructor, do nothing
	    Pixel(){}

	    Pixel(ConstRefType c){ std::fill(m_data, m_data+Channel, c); }

	    Pixel(ConstPtrType c){ std::copy(c, c+Channel, m_data); }

	    RefType operator [](int i){ return m_data[i]; }
	    
	    ConstRefType operator [](int i) const { return m_data[i]; }

	private:
	    Type m_data[Channel];
    };

    // partial specialization for Channel == 1
    template <typename Type>
    class Pixel<Type, 1>{
	public:
	    typedef Type PixelType;

	    // We don't need anything else, because
	    // Pixel<Type, 1> should be degrade to Type.
    };

    // utilities to make pixel
    // Pixel with 3 channels or 4 channels are most commonly used.
    //
    // For most cases, Type are primitive types such as unsigned char 
    // or float, so we pass arguments by value.
    template <typename Type>
    Pixel<Type, 3> make_pixel(Type c1, Type c2, Type c3){
	Pixel<Type, 3> p;
	p[0] = c1;
	p[1] = c2;
	p[2] = c3;
	return p;
    }
    
    template <typename Type>
    Pixel<Type, 4> make_pixel(Type c1, Type c2, Type c3, Type c4){
	Pixel<Type, 4> p;
	p[0] = c1;
	p[1] = c2;
	p[2] = c3;
	p[3] = c4;
	return p;
    }

} // namespace Gil

#endif // __PIXEL_H__
