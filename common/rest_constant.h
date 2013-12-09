#pragma once

#include <string>

namespace stormy {
  namespace common {
    namespace rest {
      namespace constant {      

// api: /aggregate/:station_id
const std::string aggregate_request_pattern = "/aggregate/[0-9a-f]{32}/?";        
// api: /meteo/:station_id
const std::string meteo_request_pattern = "/meteo/[0-9a-f]{32}/?";
// api: /station
const std::string station_request_pattern = "/station/?";   
// api: /info
const std::string info_request_pattern = "/info/?";
// api query: ?param_one=value1&param_two=value2 (...)
const std::string uri_query_vars_pattern = ".([a-zA-Z_]+=[a-zA-Z0-9]+&?)*";
// api available parameters
const std::string type_parameter = "type";
const std::string from_parameter = "from";
const std::string to_parameter = "to";
const std::string from_last_hours_parameter = "from_last_hours";

/*
 *  JSON constants
 */
const std::string json_id = "id";
const std::string json_uid = "uid";
const std::string json_name = "name";
const std::string json_url = "url";
const std::string json_refresh_time = "refresh_time";
const std::string json_stations_data_marker = "stations";
const std::string json_measurement_size_marker = "size";
const std::string json_measurement_data_marker = "measurements";
const std::string json_measurement_time_marker = "times";
const std::string json_empty = "{}";

// ~~ stormy::common::rest::const;
}}}}