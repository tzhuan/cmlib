#ifndef GIL_IMAGE_H
#define GIL_IMAGE_H

#include <algorithm>
#include <string>
#include <cstdio>

#include "Color.h"
#include "Converter.h"

namespace gil {

    template<typename, template<typename,typename> class Conv=DefaultConverter> 
    class Image;
    
    template<typename Type, template<typename,typename> class Conv>
    void swap(Image<Type,Conv>& a, Image<Type,Conv>& b);

    template<typename Type, template<typename,typename> class Conv>
    class Image {
	public:
	    typedef Type ColorType;
	    typedef Type* PtrType;
	    typedef const Type* ConstPtrType;
	    typedef Type& RefType;
	    typedef const Type& ConstRefType;

	    typedef PtrType iterator;
	    typedef ConstPtrType const_iterator;
	    
	    Image()
		: my_width(0), my_height(0), my_data(NULL), my_row(NULL)
	    {
		// empty
	    }

	    Image(size_t w, size_t h)
		: my_width(0), my_height(0), my_data(NULL), my_row(NULL)
	    {
		allocate(w, h);
	    }

	    ~Image() { 
		delete [] my_data; 
		delete [] my_row; 
	    }

	    size_t width() const { return my_width; }
	    size_t height() const { return my_height; }
	    size_t size() const { return my_width*my_height; }
	    size_t channels() const { return ColorTrait<Type>::channels(); }

	    void fill(ConstRefType pixel){
		std::fill(begin(), end(), pixel);
	    }

	    void allocate(size_t w, size_t h){
		if(w != my_width or h != my_height){
		    // the delete operator will automatically check for NULL
		    delete [] my_data;
		    delete [] my_row;
		    
		    if(w != 0 and h != 0){
			my_data = new Type[w*h];
			my_row = new Type*[h];
			my_width = w;
			my_height = h;
			init_row();
		    }else{
			my_width = my_height = 0;
			my_data = NULL;
			my_row = NULL;
		    }
		}
	    }

	    RefType operator ()(int x, int y) { return my_row[y][x]; }
	    ConstRefType operator ()(int x, int y) const { return my_row[y][x]; }

	    Image& operator =(const Image& img){
		allocate(img.width(), img.height());
		std::copy(img.begin(), img.end(), begin());
		return *this;
	    }

	    iterator begin() { return my_data; }
	    const_iterator begin() const { return my_data; }
	    iterator end() { return my_data + my_width*my_height; }
	    const_iterator end() const { return my_data + my_width*my_height; }

	    
	    friend void swap<>(Image<Type>& a, Image<Type>& b);

	    // converter
	    struct Converter {
		typedef Type Internal;
		
		template <typename External>
		static void ext2int(Internal& to, const External& from){
		    Conv<Internal,External>::convert(to, from);
		}
		
		template <typename External>
		static void int2ext(External& to, const Internal& from){
		    Conv<External,Internal>::convert(to, from);
		}
	    };

	protected:
	    void init_row(){
		my_row[0] = my_data;
		for (size_t i = 1; i < my_height; ++i)
		    my_row[i] = my_row[i-1] + my_width;
	    }

	private:
	    size_t my_width;
	    size_t my_height;
	    Type *my_data;
	    Type **my_row;
    };

    template<typename Type, template<typename,typename> class Conv>
    void swap(Image<Type,Conv>& a, Image<Type,Conv>& b)
    {
	using std::swap;
	swap(a.my_width, b.my_width);
	swap(a.my_height, b.my_height);
	swap(a.my_data, b.my_data);
	swap(a.my_row, b.my_row);
    }


    typedef Image<Byte1> ByteImage1;
    typedef Image<Byte3> ByteImage3;
    typedef Image<Byte4> ByteImage4;
    typedef Image<Short1> ShortImage1;
    typedef Image<Short3> ShortImage3;
    typedef Image<Short4> ShortImage4;
    typedef Image<Float1> FloatImage1;
    typedef Image<Float3> FloatImage3;
    typedef Image<Float4> FloatImage4;

} // namespace gil

#endif // GIL_IMAGE_H
