/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <iostream>
#include <iterator>
#include <algorithm>

#include <cmlib/image/Algorithm.h>
#include <cmlib/image/Functional.h>

using namespace std;
using namespace cmlib::image;

int main()
{
	double array1[] = {1, 2, 3, 4, 5};
	int array2[] = {2, 3, 4, 5, 6};
	size_t size = sizeof(array1)/sizeof(array1[0]);
	cmlib::image::inplace_transform(array1, array1 + size, array2, PlusAssigns<double, int>());
	copy(array1, array1+size, ostream_iterator<double>(cout, " "));
	cout << endl;
	return 0;
}
