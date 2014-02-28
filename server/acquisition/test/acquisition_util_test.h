#pragma once

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../acquisition_util.h"
using namespace stormy::acquisition::util;

class AcquisitionUtilTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(AcquisitionUtilTest);
    CPPUNIT_TEST(TestTryExtractFirstNumeric);
    CPPUNIT_TEST(TestSecondsToMiliseconds);
    CPPUNIT_TEST(TestMinutesToMiliseconds);
  CPPUNIT_TEST_SUITE_END();

public:
  void TestTryExtractFirstNumeric() 
  {

  }

  void TestSecondsToMiliseconds() 
  {
    CPPUNIT_ASSERT(SecondsToMiliseconds(60) == 60000);
    CPPUNIT_ASSERT(SecondsToMiliseconds(0) == 0);
    CPPUNIT_ASSERT(SecondsToMiliseconds(-1) == 0);
    CPPUNIT_ASSERT(SecondsToMiliseconds(-999999999) == 0);
  }

  void TestMinutesToMiliseconds()
  {

  }
};