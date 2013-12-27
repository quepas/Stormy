#pragma once

#include <cstdint>
#include <string>

namespace stormy {
  namespace common {
    namespace rest {
      namespace constant {      

// api: /aggregate/:station_id
const std::string aggregate_request_pattern = "/aggregate/[0-9a-f]{32}/?";        
// api: /meteo/:station_id
const std::string meteo_request_pattern = "/meteo/[0-9a-f]{32}/?";
// api: /meteo/:station_id/:timestamp
const std::string meteo_timestamp_request_pattern = "/meteo/[0-9a-f]{32}/[0-9]{10}/?";
// api: /station
const std::string station_request_pattern = "/station/?";   
// api: /station/:station_uid
const std::string station_info_request_pattern = "/station/[0-9a-f]{32}/?";
// api: /info
const std::string info_request_pattern = "/info/?";
// api query: ?param_one=value1&param_two=value2 (...)
const std::string uri_query_vars_pattern = ".([a-zA-Z_]+=[a-zA-Z0-9]+&?)*";
/*
 * api available parameters (as filters)
 */
// ?type=:metrics_code
const std::string type_parameter = "type";
// ?from=:timestamp
const std::string from_parameter = "from";
// ?to=:timestamp
const std::string to_parameter = "to";
// ?from_last_hours=:hours
const std::string from_last_hours_parameter = "from_last_hours";
const uint16_t default_from_last_hours = 24;

/*
 *  JSON constants
 */
const std::string json_id = "id";
const std::string json_uid = "uid";
const std::string json_name = "name";
const std::string json_code = "code";
const std::string json_url = "url";
const std::string json_type = "type";
const std::string json_server_marker = "server";
const std::string json_seconds = "seconds";
const std::string json_refresh_time = "refresh_time";
const std::string json_parser_class = "parser_class";
const std::string json_stations_data_marker = "stations";
const std::string json_station_uids_data_marker = "\"station_uids\"";
const std::string json_measurement_size_marker = "size";
const std::string json_measurement_data_marker = "measurements";
const std::string json_measurement_time_marker = "times";
const std::string json_available_metrics_marker = "available_metrics";
const std::string json_available_periods_marker = "available_periods";
const std::string json_empty = "{}";
const std::string json_acquistion_server_type = "A";
const std::string json_storage_and_aggregation_server_type = "S&A";

// ~~ stormy::common::rest::const;
}}}}