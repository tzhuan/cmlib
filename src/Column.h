#ifndef __COLUMN_H__
#define __COLUMN_H__

#include "Channels.h"
#include "Pixel.h"

namespace Gil {
    //template<typename Type, Channels channel>class Image;

    template<typename Type, Channels channels>
    class Column {
	//friend class Image<Type, channel>;
	public:
	    const Pixel<Type, channels>& operator[] (const size_type row)
		const { return m_row[row][m_column]; }
	    Pixel<Type, channels>& operator[] (const size_type row)
	    { return m_row[row][m_column]; }
	    Column(const size_type c, Pixel<Type, channels>** r)
		: m_column(c), m_row(r) {}
	protected:
	    Column();
	private:
	    size_type m_column;
	    Pixel<Type, channels> **m_row;
    };
}

#endif
