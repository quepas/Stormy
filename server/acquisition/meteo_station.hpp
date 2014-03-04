#pragma once

#include <string>

namespace stormy {
  namespace meteo {

struct Station 
{
  std::string id;
  std::string name;
  // http scheme
  std::string url;
  // seconds
  unsigned refresh_time;
  // python script name
  std::string script_name;
  // decimal degree
  double latitude;
  // decimal degree
  double longitude;
};
// ~~ stormy::meteo::Station
}}
