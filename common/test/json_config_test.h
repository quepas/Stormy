#pragma once

#include <vector>
#include <exception>
#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../json_config.h"

using stormy::JSONConfig;

class JSONConfigTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(JSONConfigTest);
    CPPUNIT_TEST(TestFileOpening);
    CPPUNIT_TEST(TestReading);
  CPPUNIT_TEST_SUITE_END();

public:
  void TestFileOpening() 
  {    
    CPPUNIT_ASSERT_THROW(
      JSONConfig example_1(""),
      JSONConfig::no_file_exception);
    CPPUNIT_ASSERT_THROW(
      JSONConfig example_2("abc/aaa.123"),
      JSONConfig::no_file_exception);
    CPPUNIT_ASSERT_THROW(
      JSONConfig example_3("test/resources/json_config_invalid.json"),
      JSONConfig::bad_file_exception);   
    CPPUNIT_ASSERT_NO_THROW(
      JSONConfig example_4("test/resources/json_config_empty.json"));
  }

  void TestReading() 
  {    
    JSONConfig example("test/resources/json_valid_config_simple.json");
    CPPUNIT_ASSERT(example.root().get("Name", "Default").asString() == "Default");
    CPPUNIT_FAIL(example["name"].asString());
    CPPUNIT_ASSERT(example["name"].asString() == "Wisla Malinka");
    CPPUNIT_ASSERT(example["url"].asString() == "http://stacjameteo.pl/dane/index.php?stacja=21");
    CPPUNIT_ASSERT(example["parserClass"].asString() == "Stacjameteo_MeteoParser");
    CPPUNIT_ASSERT(example["refreshTime"].asInt() == 300);
  }
};