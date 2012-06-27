/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_IMAGE_ALGORITHM
#define CMLIB_IMAGE_ALGORITHM

namespace cmlib {
namespace image{
	template<typename InputItor1, typename InputItor2, typename Op>
	InputItor1 inplace_transform(InputItor1 begin1, InputItor1 end1, InputItor2 begin2, Op op)	
	{
		for (; begin1 != end1; ++begin1, ++begin2)
			op(*begin1, *begin2);
		return begin1;
	}

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_ALGORITHM
