#include "catch.hpp"

#include "common/util.h"
using namespace stormy::common;

TEST_CASE("conversion functions") {
  SECTION("string to double") {
    REQUIRE(ToDouble("123") == 123.0);
    REQUIRE(ToDouble("abc 321abc#") == 321.0);
    REQUIRE(ToDouble("#!#12.321abc123cba") == 12.321);
    REQUIRE(ToDouble("abc") == 0.0);
    REQUIRE(ToDouble(".6") == 0.6);
    REQUIRE(ToDouble(" ") == 0.0);
    REQUIRE(ToDouble("") == 0.0);
  }

  SECTION("string is numeric?") {
    REQUIRE(IsNumeric("abc123.123") == false);
    REQUIRE(IsNumeric("123") == true);
    REQUIRE(IsNumeric("123.321") == true);
    REQUIRE(IsNumeric(".5") == true);
    REQUIRE(IsNumeric(" ") == false);
    REQUIRE(IsNumeric("") == false);
  }
}