#ifndef GIL_CONVERTER_H
#define GIL_CONVERTER_H

#include "Color.h"

namespace gil {

    template <typename T>
    class Converter {
	public:
	    typedef T Internal;
	    typedef typename ColorTrait<T>::BaseType BaseType;

	    Converter(size_t c) : my_channels(c);

	    template <typename F>
	    Internal operator () const (F v)
	    {
		return static_cast<Internal>(v);
	    }

	private:
	    size_t my_channels;
    };

} // namespace gil

#endif // GIL_CONVERTER_H
