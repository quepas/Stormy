#include "catch.hpp"

#include "json_config.h"
using stormy::JSONConfig;

TEST_CASE("json config basic operations") {
  SECTION("file opening") {    
    REQUIRE_THROWS_AS(
      JSONConfig config_1(""), 
      JSONConfig::no_file_exception);
    REQUIRE_THROWS_AS(
      JSONConfig config("abc/aaa.123"), 
      JSONConfig::no_file_exception);
    REQUIRE_THROWS_AS(
      JSONConfig config("resources/json_config_invalid.json"), 
      JSONConfig::bad_file_exception);
    REQUIRE_NOTHROW(
      JSONConfig config("resources/json_config_empty.json"));
  }

  SECTION("file reading") {
    JSONConfig config("resources/json_valid_config_simple.json");
    REQUIRE(config.root()
      .get("Name", "Default").asString() == "Default");
    REQUIRE(config["name"].asString() == "Wisla Malinka");
    REQUIRE(config["url"].asString() == "http://stacjameteo.pl/dane/index.php?stacja=21");
    REQUIRE(config["parserClass"] == "Stacjameteo_MeteoParser");
    REQUIRE(config["refreshTime"] == 300);
  }
}