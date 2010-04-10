#ifndef CMLIB_IMAGE_COLORSELECT_H
#define CMLIB_IMAGE_COLORSELECT_H

#include <cmlib/image/Type.h>

namespace cmlib {
namespace image {

	template<typename LargerType, typename SmallerType>
	struct SameTypeSelector {
		typedef LargerType priority_type;	
		typedef SmallerType non_priority_type;
	};

	// char {{{
	template<>
	struct SameTypeSelector<char, unsigned char> {
		typedef unsigned char priority_type;
		typedef char non_priority_type;
	};

	template<>
	struct SameTypeSelector<char, short> {
		typedef short priority_type;
		typedef char non_priority_type;
	};

	template<>
	struct SameTypeSelector<char, unsigned short> {
		typedef unsigned short priority_type;
		typedef char non_priority_type;
	};

	template<>
	struct SameTypeSelector<char, int> {
		typedef int priority_type;
		typedef char non_priority_type;
	};

	template<>
	struct SameTypeSelector<char, unsigned int> {
		typedef unsigned int priority_type;
		typedef char non_priority_type;
	};

	template<>
	struct SameTypeSelector<char, long> {
		typedef long priority_type;
		typedef char non_priority_type;
	};

	template<>
	struct SameTypeSelector<char, unsigned long> {
		typedef unsigned long priority_type;
		typedef char non_priority_type;
	};
	// }}}

	// unsigned char {{{
	template<>
	struct SameTypeSelector<unsigned char, short> {
		typedef short priority_type;
		typedef unsigned char non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned char, unsigned short> {
		typedef unsigned short priority_type;
		typedef unsigned char non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned char, int> {
		typedef int priority_type;
		typedef unsigned char non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned char, unsigned int> {
		typedef unsigned int priority_type;
		typedef unsigned char non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned char, long> {
		typedef long priority_type;
		typedef unsigned char non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned char, unsigned long> {
		typedef unsigned long priority_type;
		typedef unsigned char non_priority_type;
	};
	// }}}

	// short {{{
	template<>
	struct SameTypeSelector<short, unsigned short> {
		typedef unsigned short priority_type;
		typedef short non_priority_type;
	};

	template<>
	struct SameTypeSelector<short, int> {
		typedef int priority_type;
		typedef short non_priority_type;
	};

	template<>
	struct SameTypeSelector<short, unsigned int> {
		typedef unsigned int priority_type;
		typedef short non_priority_type;
	};

	template<>
	struct SameTypeSelector<short, long> {
		typedef long priority_type;
		typedef short non_priority_type;
	};

	template<>
	struct SameTypeSelector<short, unsigned long> {
		typedef unsigned long priority_type;
		typedef short non_priority_type;
	};
	// }}}

	// unsigned short {{{
	template<>
	struct SameTypeSelector<unsigned short, int> {
		typedef int priority_type;
		typedef unsigned short non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned short, unsigned int> {
		typedef unsigned int priority_type;
		typedef unsigned short non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned short, long> {
		typedef long priority_type;
		typedef unsigned short non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned short, unsigned long> {
		typedef unsigned long priority_type;
		typedef unsigned short non_priority_type;
	};
	// }}}

	// int {{{
	template<>
	struct SameTypeSelector<int, unsigned int> {
		typedef unsigned int priority_type;
		typedef int non_priority_type;
	};

	template<>
	struct SameTypeSelector<int, long> {
		typedef long priority_type;
		typedef int non_priority_type;
	};

	template<>
	struct SameTypeSelector<int, unsigned long> {
		typedef unsigned long priority_type;
		typedef int non_priority_type;
	};
	// }}}

	// unsigned int {{{
	template<>
	struct SameTypeSelector<unsigned int, long> {
		typedef long priority_type;
		typedef unsigned int non_priority_type;
	};

	template<>
	struct SameTypeSelector<unsigned int, unsigned long> {
		typedef unsigned long priority_type;
		typedef unsigned int non_priority_type;
	};
	// }}}

	// long {{{
	template<>
	struct SameTypeSelector<long, unsigned long> {
		typedef unsigned long priority_type;
		typedef long non_priority_type;
	};
	// }}}

	// float {{{
	template<>
	struct SameTypeSelector<float, double> {
		typedef double priority_type;
		typedef float non_priority_type;
	};
	// }}}
	
	template<typename Type1, typename Type2, size_t Channel>
	struct SameTypeSelector< Color<Type1, Channel>, Color<Type2, Channel> > {
		typedef Color<typename SameTypeSelector<Type1, Type2>::priority_type, Channel> priority_type;
		typedef Color<typename SameTypeSelector<Type1, Type2>::non_priority_type, Channel> non_priority_type;
	};

	template<typename Type1, typename Tag1, typename Type2, typename Tag2>
	struct RealColorSelector {};

	template<typename IntegerType, typename FloatingPointType>
	struct RealColorSelector<IntegerType, IntegerTag, FloatingPointType, FloatingPointTag> {
		typedef FloatingPointType priority_type;
		typedef IntegerType non_priority_type;
	};

	template<typename FloatingPointType, typename IntegerType>
	struct RealColorSelector<FloatingPointType, FloatingPointTag, IntegerType, IntegerTag> {
		typedef FloatingPointType priority_type;
		typedef IntegerType non_priority_type;
	};

	template<typename Type1, typename Type2, typename Tag>
	struct RealColorSelector<Type1, Tag, Type2, Tag> {
		typedef typename SameTypeSelector<Type1, Type2>::priority_type priority_type;
		typedef typename SameTypeSelector<Type1, Type2>::non_priority_type non_priority_type;
	};

	template<typename Type1, typename Type2>
	struct ColorSelector {
		typedef typename 
			RealColorSelector<
				Type1, typename TypeTrait<Type1>::type_category,	
				Type2, typename TypeTrait<Type2>::type_category
			>::priority_type priority_type;
		typedef typename 
			RealColorSelector<
				Type1, typename TypeTrait<Type1>::type_category,	
				Type2, typename TypeTrait<Type2>::type_category
			>::non_priority_type non_priority_type;
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_COLORSELECT_H
