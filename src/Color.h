#ifndef __GIL_COLOR_H__
#define __GIL_COLOR_H__

#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

namespace Gil {
    // type definitions
    typedef unsigned char byte;

    // utilities
    template <typename Type>
    Type clamp(Type value, Type lower, Type upper){
	return std::min( std::max(value, lower), upper );
    }
    
    // type traits
    template <typename Type>
    struct TypeTrait {
	typedef Type DebugType;
	
	static Type zero();
	
	// for alpha channel
	static Type transparent();
	static Type opaque();

	static Type multiply(Type a, Type b);
    };

    template <>
    struct TypeTrait<byte> {
	typedef int DebugType;
	static byte zero() { return 0; }
	static byte transparent() { return 0; }
	static byte opaque() { return 255; }
	static byte multiply(byte a, byte b){
	    return (byte)( ((int)a * (int)b) / 255 );
	}
    };

    template <>
    struct TypeTrait<float> {
	typedef float DebugType;
	static float zero() { return 0.0f; }
	static float transparent() { return 0.0f; }
	static float opaque() { return 1.0f; }
	static float multiply(float a, float b){
	    return a*b;
	}
    };
    
    // Basic features for a pixel
    template <typename Type, size_t Channel>
    class Color {
	public:
	    typedef Color<Type,Channel> ColorType;
	    typedef const Color<Type,Channel> ConstColorType;
	    typedef Type* PtrType;
	    typedef const Type* ConstPtrType;
	    typedef Type& RefType;
	    typedef const Type& ConstRefType;
	    
	    RefType operator [](int i){ return my_data[i]; }
	    
	    ConstRefType operator [](int i) const { return my_data[i]; }

	    Color(){
		std::fill(begin(), end(), TypeTrait<Type>::zero());
	    }

	    Color(Type c0){
		set(c0);
	    }

	    Color(Type c0, Type c1, Type c2){
		set(c0, c1, c2);
	    }

	    Color(Type c0, Type c1, Type c2, Type c3){
		set(c0, c1, c2, c3);
	    }

	    Color(const Color<Type,3>& color){
		set(color[0], color[1], color[2]);
		if(Channel == 4)
		    my_data[3] == TypeTrait<Type>::opaque();
	    }

	    Color(const Color<Type,4>& color){
		set(color[0], color[1], color[2], color[3]);
	    }

	    void set(Type c0){
		std::fill(begin(), end(), c0);
	    }

	    void set(Type c0, Type c1, Type c2){
		my_data[0] = c0;
		if(Channel >= 2) my_data[1] = c1;
		if(Channel >= 3) my_data[2] = c2;
	    }
	    
	    void set(Type c0, Type c1, Type c2, Type c3){
		my_data[0] = c0;
		if(Channel >= 2) my_data[1] = c1;
		if(Channel >= 3) my_data[2] = c2;
		if(Channel >= 4) my_data[3] = c3;
	    }

	    ColorType& operator =(const Color<Type,3>& color){
		set(color[0], color[1], color[2]);
		return *this;
	    }

	    ColorType& operator =(const Color<Type,4>& color){
		set(color[0], color[1], color[2], color[3]);
		return *this;
	    }

	    PtrType begin() { return my_data; }
	    ConstPtrType begin() const { return my_data; }
	    PtrType end() { return my_data+Channel; }
	    ConstPtrType end() const { return my_data+Channel; }

	    ColorType operator *(Type v){
		using namespace std;
		ColorType color;
		transform(begin(),
			  end(),
			  color.begin(),
			  bind2nd(ptr_fun(TypeTrait<Type>::multiply), v) );
		return color;
	    }

	    ColorType& operator *=(Type v){
		using namespace std;
		transform(begin(),
			  end(),
			  begin(),
			  bind2nd(ptr_fun(TypeTrait<Type>::multiply), v) );
	    }

	protected:
	    Type my_data[Channel];
    };

    template <typename Type>
    class Color<Type, 1> {
	public:
	    // in Image<Type, 1>, this will make Pixel<Type, 1> 
	    // degenerated to Type.
	    typedef Type ColorType;
	    typedef const Type ConstColorType;
    };

    // default behavior to print all channels in a pixel
    // to avoid unprintable characters, values are converted
    // to DebugType defined in TypeTrait.
    template <typename T, size_t C>
    std::ostream& operator <<(std::ostream& out, Color<T, C> color){
	typedef typename TypeTrait<T>::DebugType DebugType;
	std::copy(color.begin(),
		  color.end(),
		  std::ostream_iterator<DebugType>(out, " "));
	return out;
    }

    // other utilities
    template <typename T, size_t C>
    Color<T,C> min(const Color<T,C>& a, const Color<T,C>& b){
	Color<T,C> result;
	for(size_t i = 0; i < C; i++)
	    result[i] = std::min(a[i], b[i]);
	return result;
    }

    template <typename T, size_t C>
    Color<T,C> max(const Color<T,C>& a, const Color<T,C>& b){
	Color<T,C> result;
	for(size_t i = 0; i < C; i++)
	    result[i] = std::max(a[i], b[i]);
	return result;
    }

} // namespace Gil

#endif // __GIL_COLOR_H__
