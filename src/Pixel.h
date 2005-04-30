#ifndef __PIXEL_H__
#define __PIXEL_H__

#include "Channels.h"

namespace Gil {
    template<typename Type, Channels channels>
    class Pixel {
	public:
	protected:
	private:
	    Pixel();
    };

    template<typename Type>
    class Pixel<Type, OneChannel> {
	public:
	    Pixel() {}
	    Pixel(const Type& c) {m_data[0] = c; }
	    Pixel(const Type* c) {m_data[0] = *c; }
	    const Type& operator[] (const size_type c) const 
		{ return m_data[c]; }
	    Type& operator[] (const size_type c) 
		{ return m_data[c]; }
	    operator Type () const
		{ return m_data[0]; }
	protected:
	private:
	    Type m_data[OneChannel];
    };

    template<typename Type>
    class Pixel<Type, ThreeChannels> {
	public:
	    Pixel() {}
	    Pixel(const Type& c0, const Type& c1, const Type& c2)
		{ m_data[0] = c0; m_data[1] = c1; m_data[2] = c2;}
	    Pixel(const Type* c) 
		{ m_data[0] = c[0]; m_data[1] = c[1]; m_data[2] = c[2]; }
	    const Type& operator[] (const size_type c) const 
		{ return m_data[c]; }
	    Type& operator[] (const size_type c) 
		{ return m_data[c]; }
	private:
	    Type m_data[ThreeChannels];
    };

    template<typename Type>
    class Pixel<Type, FourChannels> {
	public:
	    Pixel() {}
	    Pixel(const Type& c0, const Type& c1, 
		    const Type& c2, const Type& c3)
		{ m_data[0]=c0; m_data[1]=c1; m_data[2]=c2; m_data[3]=c3;}
	    Pixel(const Type* c) 
		{ m_data[0] = c[0]; m_data[1] = c[1]; 
		    m_data[2] = c[2]; m_data[3] = c[3]; }
	    const Type& operator[] (const size_type c) const 
		{ return m_data[c]; }
	    Type& operator[] (const size_type c) 
		{ return m_data[c]; }
	private:
	    Type m_data[FourChannels];
    };
}

#endif
