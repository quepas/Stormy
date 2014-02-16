#pragma once

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../util.h"

using std::string;
using namespace stormy::common;

class UtilTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(UtilTest);
    CPPUNIT_TEST(TestToDouble);
    CPPUNIT_TEST(TestIsNumeric);
  CPPUNIT_TEST_SUITE_END();

public:
  void TestToDouble() 
  {
    CPPUNIT_ASSERT(ToDouble("123") == 123);
    CPPUNIT_ASSERT(ToDouble("abc 321abc#") == 321);
    CPPUNIT_ASSERT(ToDouble("#!#12.321abc123cba") == 12.321);
    CPPUNIT_ASSERT(ToDouble("abc") == 0.0);
    CPPUNIT_ASSERT(ToDouble(".6") == 0.6);
    CPPUNIT_ASSERT(ToDouble(" ") == 0.0);
  }

  void TestIsNumeric()
  {
    CPPUNIT_ASSERT(IsNumeric("abc123.123") == false);
    CPPUNIT_ASSERT(IsNumeric("123") == true);
    CPPUNIT_ASSERT(IsNumeric("123.321") == true);
    CPPUNIT_ASSERT(IsNumeric(" ") == false);
    CPPUNIT_ASSERT(IsNumeric(".5") == true);
  }
};