#pragma once

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../rest_uri_parser.h"
using namespace stormy::common::rest;

class RestURIParserTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RestURIParserTest);
    CPPUNIT_TEST(TestPathSegments);
    CPPUNIT_TEST(TestQuerySegments);
  CPPUNIT_TEST_SUITE_END();
public:
  void TestPathSegments()
  {
    URIParser uri_example_1("http://abc.bda/aaa/bbb/ccc");
    auto uri_1_segments = uri_example_1.getPathSegments();
    CPPUNIT_ASSERT(uri_1_segments.size() == 3);
    CPPUNIT_ASSERT(uri_1_segments[0] == "aaa");
    CPPUNIT_ASSERT(uri_1_segments[1] == "bbb");
    CPPUNIT_ASSERT(uri_1_segments[2] == "ccc");

    URIParser uri_example_2("abc/bcd/cde/");
    auto uri_2_segments = uri_example_2.getPathSegments();
    CPPUNIT_ASSERT(uri_2_segments.size() == 3);
    CPPUNIT_ASSERT(uri_2_segments[0] == "abc");
    CPPUNIT_ASSERT(uri_2_segments[1] == "bcd");
    CPPUNIT_ASSERT(uri_2_segments[2] == "cde");

    URIParser uri_example_3("");
    auto uri_3_segments = uri_example_3.getPathSegments();
    CPPUNIT_ASSERT(uri_3_segments.size() == 0);

    URIParser uri_example_4("abc");
    auto uri_4_segments = uri_example_4.getPathSegments();
    CPPUNIT_ASSERT(uri_4_segments.size() == 1);
    CPPUNIT_ASSERT(uri_4_segments[0] == "abc");

    URIParser uri_example_5("://abc.cba/aaa/ccc");
    auto uri_5_segments = uri_example_5.getPathSegments();
    CPPUNIT_ASSERT(uri_5_segments.size() == 2);
    CPPUNIT_ASSERT(uri_5_segments[0] == "aaa");
    CPPUNIT_ASSERT(uri_5_segments[1] == "ccc");

    URIParser uri_example_6("http://abc/aaa/bbb?ccc=123");
    auto uri_6_segments = uri_example_6.getPathSegments();
    CPPUNIT_ASSERT(uri_6_segments.size() == 2);
    CPPUNIT_ASSERT(uri_6_segments[0] == "aaa");
    CPPUNIT_ASSERT(uri_6_segments[1] == "bbb");

    URIParser uri_example_7("     \n\t  ");
    auto uri_7_segments = uri_example_7.getPathSegments();
    CPPUNIT_ASSERT(uri_7_segments.size() == 0);
  }

  void TestQuerySegments() 
  {
    URIParser uri_example_1("http://abc/aaa/bbb?ccc=123");
    auto uri_1_queries = uri_example_1.getQuerySegments();
    CPPUNIT_ASSERT(uri_1_queries.size() == 1);
    CPPUNIT_ASSERT(uri_1_queries.find("ccc") != uri_1_queries.end());
    CPPUNIT_ASSERT(uri_1_queries["ccc"] == "123");

    URIParser uri_example_2("     \n\t  ");
    auto uri_2_queries = uri_example_2.getQuerySegments();
    CPPUNIT_ASSERT(uri_2_queries.size() == 0);

    URIParser uri_example_3("   ?=af");
    auto uri_3_queries = uri_example_3.getQuerySegments();
    CPPUNIT_ASSERT(uri_3_queries.size() == 0);

    URIParser uri_example_4("");
    auto uri_4_queries = uri_example_4.getQuerySegments();
    CPPUNIT_ASSERT(uri_4_queries.size() == 0);

    URIParser uri_example_5("http://aaa?");
    auto uri_5_queries = uri_example_5.getQuerySegments();
    CPPUNIT_ASSERT(uri_5_queries.size() == 0);

    URIParser uri_example_6("http://asd?=&aaa=111&bbb=&=333&ccc");
    auto uri_6_queries = uri_example_6.getQuerySegments();
    CPPUNIT_ASSERT(uri_6_queries.size() == 2);
    CPPUNIT_ASSERT(uri_6_queries.find("aaa") != uri_6_queries.end());
    CPPUNIT_ASSERT(uri_6_queries["aaa"] == "111");
    CPPUNIT_ASSERT(uri_6_queries.find("bbb") != uri_6_queries.end());
    CPPUNIT_ASSERT(uri_6_queries["bbb"] == "");
  }
};