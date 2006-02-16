#ifndef GIL_IMAGE_H
#define GIL_IMAGE_H

#include <algorithm>
#include <string>
#include <cstdio>

#include "Color.h"

namespace gil {

    template<typename Type> class Image;
    
    template<typename Type>
    void swap(Image<Type>& a, Image<Type>& b);

    template<typename Type>
    class Image {
	public:
	    typedef Type* PtrType;
	    typedef const Type* ConstPtrType;
	    typedef Type& RefType;
	    typedef const Type& ConstRefType;
	    
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

	    PtrType begin() { return my_data; }
	    ConstPtrType begin() const { return my_data; }
	    PtrType end() { return my_data + my_width*my_height; }
	    ConstPtrType end() const { return my_data + my_width*my_height; }


	    // IO functions	    
	    bool read(const std::string& filename);

	    bool read(FILE* fd);

	    template <class Reader>
	    bool read(const std::string& filename);
	    
	    template <class Reader>
	    bool read(FILE* fd);

	    bool write(const std::string& filename);

	    template <class Writer>
	    bool write(const std::string& filename);
	    
	    template <class Writer>
	    bool write(FILE* fh);
	    
	    friend void swap<>(Image<Type>& a, Image<Type>& b);

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

    template <typename Type>
    void swap(Image<Type>& a, Image<Type>& b){
	using std::swap;
	swap(a.my_width, b.my_width);
	swap(a.my_height, b.my_height);
	swap(a.my_data, b.my_data);
	swap(a.my_row, b.my_row);
    }

    template <typename Type>
    bool Image<Type>::read(const std::string& filename){
	FILE* fd = fopen(filename.c_str(), "rb");
	if(fd == NULL)
	    return false;
	bool status = read(fd);
	fclose(fd);
	return status;
    }
    
    template <typename Type>
    bool Image<Type>::read(FILE* fd){
	// autodetect filetype
    }

    template <typename Type>
    template <typename Reader>
    bool Image<Type>::read(const std::string& filename){
	FILE* fd = fopen(filename.c_str(), "rb");
	if(fd == NULL)
	    return false;
	bool result = read<Reader>(fd);
	fclose(fd);
	return result;
    }
    
    template <typename Type>
    template <typename Reader>
    bool Image<Type>::read(FILE* fd){
	Reader reader;
	reader.init_read(fd);
	size_t w, h, c;
	reader.read_header(w, h, c);
	allocate(w, h);
	reader.read_pixel(my_data[0].begin());
	reader.finish();
	return reader.good();
    }

    template <typename Type>
    bool Image<Type>::write(const std::string& filename){
	// should detect file format by the file extension.
    }

    template <typename Type>
    template <typename Writer>
    bool Image<Type>::write(const std::string& filename){
	FILE* fd = fopen(filename.c_str(), "wb");
	if(fd == NULL)
	    return false;
	bool result = write<Writer>(fd);
	fclose(fd);
	return result;
    }
    
    template <typename Type>
    template <typename Writer>
    bool Image<Type>::write(FILE* fd){
	Writer writer;
	writer.init_write(fd);
	size_t c; // FIXME: how to get the channel number?
	writer.write_header(my_width, my_height, c);
	writer.write_pixel(my_data);
	writer.finish();
	return writer.good();
    }

    typedef Image<Byte1> ByteImage1;
    typedef Image<Byte3> ByteImage3;
    typedef Image<Byte4> ByteImage4;
    typedef Image<Float1> FloatImage1;
    typedef Image<Float3> FloatImage3;
    typedef Image<Float4> FloatImage4;

} // namespace gil

#endif // GIL_IMAGE_H
