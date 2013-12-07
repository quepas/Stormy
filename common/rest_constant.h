#pragma once

#include <string>

namespace stormy {
  namespace common {
    namespace rest {
      namespace constant {      

// api: /aggregate/:station_id
std::string aggregate_request_pattern = "/aggregate/[0-9a-f]{32}/{0, 1}";        
// api: /meteo/:station_id
std::string meteo_request_pattern = "/meteo/[0-9a-f]{32}/{0, 1}";
// api: /station
std::string station_request_pattern = "/station/{0, 1}";   
// api: /info
std::string info_request_pattern = "/info/{0, 1}";
// ~~ stormy::common::rest::const;
}}}}