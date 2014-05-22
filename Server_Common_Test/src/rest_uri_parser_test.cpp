#include "catch.hpp"

#include "rest_uri_parser.h"
using namespace stormy::common::rest;

TEST_CASE("URI decomposition on segments") {
  SECTION("proper URI") {
    URIParser uri_parser("http://abc.bda/aaa/bbb/ccc");
    auto segments = uri_parser.getPathSegments();

    REQUIRE(segments.size() == 3);
    REQUIRE(segments[0] == "aaa");
    REQUIRE(segments[1] == "bbb");
    REQUIRE(segments[2] == "ccc");
  }

  SECTION("bad URI with only path section") {
    URIParser uri_parser("abc/bcd/cde/");
    auto segments = uri_parser.getPathSegments();

    REQUIRE(segments.size() == 3);
    REQUIRE(segments[0] == "abc");
    REQUIRE(segments[1] == "bcd");
    REQUIRE(segments[2] == "cde");
  }

  SECTION("empty URI") {
    URIParser uri_parser("");
    auto segments = uri_parser.getPathSegments();

    REQUIRE(segments.size() == 0);
  }

  SECTION("single path URI") {
    URIParser uri_parser("abc");
    auto segments = uri_parser.getPathSegments();

    REQUIRE(segments.size() == 1);
    REQUIRE(segments[0] == "abc");
  }

  SECTION("URI without protocol") {
    URIParser uri_parser("://abc.cba/aaa/ccc");
    auto segments = uri_parser.getPathSegments();

    REQUIRE(segments.size() == 2);
    REQUIRE(segments[0] == "aaa");
    REQUIRE(segments[1] == "ccc");
  }

  SECTION("URI with segments and queries") {
    URIParser uri_parser("http://abc/aaa/bbb?ccc=123");
    auto segments = uri_parser.getPathSegments();

    REQUIRE(segments.size() == 2);
    REQUIRE(segments[0] == "aaa");
    REQUIRE(segments[1] == "bbb");
  }

  SECTION("whitespaces") {
    URIParser uri_parser("     \n\t  ");
    auto segments = uri_parser.getPathSegments();

    REQUIRE(segments.size() == 0);
  }
}

TEST_CASE("URI decomposition on queries") {
  SECTION("proper URI with segments and queries") {
    URIParser uri_parser("http://abc/aaa/bbb?ccc=123");
    auto queries = uri_parser.getQuerySegments();

    REQUIRE(queries.size() == 1);
    REQUIRE(queries["ccc"] == "123");
  }

  SECTION("whitespaces") {
    URIParser uri_parser("     \n\t  ");
    auto queries = uri_parser.getQuerySegments();

    REQUIRE(queries.size() == 0);
  }

  SECTION("query anonymous parameter") {
    URIParser uri_parser("   ?=af");
    auto queries = uri_parser.getQuerySegments();

    REQUIRE(queries.size() == 0);
  }

  SECTION("empty") {
    URIParser uri_parser("");
    auto queries = uri_parser.getQuerySegments();

    REQUIRE(queries.size() == 0);
  }

  SECTION("empty query") {
    URIParser uri_parser("http://aaa?");
    auto queries = uri_parser.getQuerySegments();

    REQUIRE(queries.size() == 0);
  }

  SECTION("complex query with anonymous query parameters") {
    URIParser uri_parser("http://asd?=&aaa=111&bbb=&=333&ccc");
    auto queries = uri_parser.getQuerySegments();

    REQUIRE(queries.size() == 2);
    REQUIRE(queries["aaa"] == "111");
    REQUIRE(queries["bbb"] == "");
  }
}
