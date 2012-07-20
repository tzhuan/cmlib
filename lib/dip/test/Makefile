# Copyright NTU CSIE CMLAB 2005 - 2012
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file ../../../LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

CMLIB_CXXFLAGS = -I../../.. -I /usr/local/include 
CMLIB_LDFLAGS = -L../../image/src -lcmlib-imageio

BOOST_CXXFALGS = -DBOOST_TEST_DYN_LINK
BOOST_LDFLAGS = -lboost_unit_test_framework

CXXFLAGS = -Wall -O3 -g $(BOOST_CXXFALGS) $(CMLIB_CXXFLAGS)
LDFLAGS = $(BOOST_LDFLAGS) $(CMLIB_LDFLAGS)
