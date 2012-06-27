/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/test/included/unit_test_framework.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>

using boost::unit_test::test_suite;
using boost::unit_test::test_case;
using boost::test_tools::close_at_tolerance;
using namespace boost;

// BOOST
#include <boost/lexical_cast.hpp>

// STL
#include <functional>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>

#include "cmlib/image.h"

using namespace cmlib::image;

template<typename Color> struct ColorTester { 
	ColorTester()
	{
	}

	void test_mix()
	{
	}

	void test_add()
	{
	}

	void test_minus()
	{
	}

	void test_multiply()
	{
	}

	void test_divide()
	{
	}

	void test_min()
	{
	}

	void test_max()
	{
	}

	void test_clamp()
	{
	}
};

struct ColorTestSuite: public test_suite { 
	ColorTestSuite(): test_suite("color test suite")
	{
		init<Byte1>();
		init<Byte3>();
		init<Byte4>();

		init<Float1>();
		init<Float3>();
		init<Float4>();
	}

	template<typename Color>
	void init()
	{
		typedef ColorTester<Color> Tester;
		shared_ptr< Tester > tester(new Tester);

		add( BOOST_CLASS_TEST_CASE( &Tester::test_mix, tester ) );
		add( BOOST_CLASS_TEST_CASE( &Tester::test_add, tester ) );
		add( BOOST_CLASS_TEST_CASE( &Tester::test_minus, tester ) );
		add( BOOST_CLASS_TEST_CASE( &Tester::test_multiply, tester ) );
		add( BOOST_CLASS_TEST_CASE( &Tester::test_divide, tester ) );
		add( BOOST_CLASS_TEST_CASE( &Tester::test_min, tester ) );
		add( BOOST_CLASS_TEST_CASE( &Tester::test_max, tester ) );
		add( BOOST_CLASS_TEST_CASE( &Tester::test_clamp, tester ) );
	}
}; 

template<typename To, typename From>
struct ConverterTester {
	void test_conversion()
	{
	}
};

struct ConverterTestSuite: public test_suite {
	ConverterTestSuite(): test_suite("converter testsuite")
	{
		init<Byte1, Byte1>();
		init<Byte1, Byte3>();
		init<Byte1, Byte4>();

		init<Byte3, Byte1>();
		init<Byte3, Byte3>();
		init<Byte3, Byte4>();

		init<Byte4, Byte1>();
		init<Byte4, Byte3>();
		init<Byte4, Byte4>();

		init<Float1, Float1>();
		init<Float1, Float3>();
		init<Float1, Float4>();

		init<Float3, Float1>();
		init<Float3, Float3>();
		init<Float3, Float4>();

		init<Float4, Float1>();
		init<Float4, Float3>();
		init<Float4, Float4>();

		init<Byte1, Float1>();
		init<Byte1, Float3>();
		init<Byte1, Float4>();

		init<Byte3, Float1>();
		init<Byte3, Float3>();
		init<Byte3, Float4>();

		init<Byte4, Float1>();
		init<Byte4, Float3>();
		init<Byte4, Float4>();

		init<Float1, Byte1>();
		init<Float1, Byte3>();
		init<Float1, Byte4>();

		init<Float3, Byte1>();
		init<Float3, Byte3>();
		init<Float3, Byte4>();

		init<Float4, Byte1>();
		init<Float4, Byte3>();
		init<Float4, Byte4>();
	}

	template<typename To, typename From>
	void init() 
	{
		typedef ConverterTester<To, From> Tester;
		shared_ptr< Tester > tester(new Tester);

		add( BOOST_CLASS_TEST_CASE( &Tester::test_conversion, tester ) );
	}
};

template<typename Image>
struct ImageTester {

};

struct ImageTestSuite: public test_suite {
	ImageTestSuite(): test_suite("image test suite")
	{
	}

	template<typename Image>
	void init()
	{
		typedef ImageTester<Image> Tester;
		shared_ptr< Tester > tester(new Tester);

	}
};

test_suite* init_unit_test_suite( int argc, char * argv[] ) 
{
    test_suite* test = BOOST_TEST_SUITE("cmlib::image core test");

    try {
        test->add( new ColorTestSuite() );
		test->add( new ConverterTestSuite() );
        test->add( new ImageTestSuite() );
        // test->add( new SubImageTestSuite() );
        // test->add( new LeptoImageTestSuite() );
        // test->add( new IOTestSuite( argv[1] ) );
    } catch ( boost::bad_lexical_cast& ) {
        return (test_suite*)0;
    } catch ( std::exception& e) {
		std::cerr << e.what() << std::endl;
        return (test_suite*)0;
	}

    return test;
}
