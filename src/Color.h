#ifndef __GIL_COLOR_H__
#define __GIL_COLOR_H__

namespace Gil {
    
    // Basic features for a pixel
    template <typename Type, size_t Channel>
    class Color {
	public:
	    Type& operator [](int i){ return my_data[i]; }
	    
	    const Type& operator [](int i) const { return my_data[i]; }

	    void set(Type c0, Type c1, Type c2){
		my_data[0] = c0;
		if(Channel >= 2) my_data[1] = c1;
		if(Channel >= 3) my_data[2] = c2;
	    }

	protected:
	    Type my_data[Channel];
    };

    template <typename Type, size_t Channel>
    class Pixel : public BasicPixel<Type, Channel> {};

    template <typename Type>
    class Pixel<Type, 1> {
	public:
	    // in Image<Type, 1>, this will make Pixel<Type, 1> 
	    // degenerated to Type.
	    typedef Type PixelType;
	    typedef const Type ConstPixelType;
    };

    // Pixels with 3 or 4 channels are used most commonly.
    // Here we make them cooperate more smoothly.
    // Useful constructor and set() member function are
    // also included.
    template <typename Type>
    class Pixel<Type, 3> : public BasicPixel<Type, 3>{
	public:
	    typedef Pixel<Type, 3> PixelType;
	    typedef const Pixel<Type, 3> ConstPixelType;

	    Pixel(){}

	    Pixel(Type c0, Type c1, Type c2){
		set(c0, c1, c2);
	    }

	    Pixel(const Pixel<Type, 4>& p){
		set(p[0], p[1], p[2]);
	    }
	    
	    // color only assignment
	    PixelType& operator =(const Pixel<Type, 4>& p){
		return set(p[0], p[1], p[2]);
	    }

	    PixelType& set(Type c0, Type c1, Type c2){
		Super::m_data[0] = c0;
		Super::m_data[1] = c1;
		Super::m_data[2] = c2;
		return *this;
	    }
	private:
	    // I don't know why m_data remains undeclaired even BasicPixel
	    // is inherited. However this can be solved by specifying the
	    // base class name.
	    typedef BasicPixel<Type, 3> Super;
    };

    template <typename Type>
    class Pixel<Type, 4> : public BasicPixel<Type, 4> {
	public:
	    typedef Pixel<Type, 4> PixelType;
	    typedef const Pixel<Type, 4> ConstPixelType;

	    Pixel(){}
	    
	    Pixel(Type c0, Type c1, Type c2, Type a){
		set(c0, c1, c2);
		Super::m_data[3] = a;
	    }
	    Pixel(const Pixel<Type, 3>& p){
		set(p[0], p[1], p[2]);
	    }
	    
	    Type& alpha(){ return Super::m_data[3]; }
	    const Type& alpha() const { return Super::m_data[3]; }
	    
	    // color only assignment
	    PixelType& operator =(const Pixel<Type, 3>& p){
		return set(p[0], p[1], p[2]);
	    }
	    
	    PixelType& set(Type c0, Type c1, Type c2){
		Super::m_data[0] = c0;
		Super::m_data[1] = c1;
		Super::m_data[2] = c2;
		return *this;
	    }
	    
	private:
	    typedef BasicPixel<Type, 4> Super;
    };

} // namespace Gil

#endif // __GIL_COLOR_H__
