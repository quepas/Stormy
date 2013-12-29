#pragma once

#include <cstdint>
#include <string>

namespace stormy {
  namespace common {
    namespace rest {
      namespace constant {      

// api: /station
const std::string station_request_pattern = "/station/?";   
// api: /station/:station_uid
const std::string station_info_request_pattern = "/station/[0-9a-f]{32}/?";
// api: /meteo
const std::string meteo_request_pattern = "/meteo/?";
// api: /meteo/:station_id
const std::string meteo_station_uid_request_pattern = "/meteo/[0-9a-f]{32}/?";
// api: /meteo/:station_id/:timestamp
const std::string meteo_station_uid_ts_request_pattern = "/meteo/[0-9a-f]{32}/[0-9]{10}/?";
// api: /aggregate/:station_id
const std::string aggregate_request_pattern = "/aggregate/[0-9a-f]{32}/?";
// api: /metrics
const std::string metrics_request_pattern = "/metrics/?";
// api: /metrics/:metrics_code
const std::string metrics_info_request_pattern = "/metrics/[a-zA-Z0-9]+/?";
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
const std::string json_uid = "\"uid\":";
const std::string json_name = "\"name\":";
const std::string json_code = "code";
const std::string json_count = "\"count\":";
const std::string json_url = "\"url\":";
const std::string json_type = "type";
const std::string json_metrics = "\"metrics\":";
const std::string json_error = "\"error\":";
const std::string json_msg = "\"msg\":";
const std::string json_measurements = "\"measurements\":";
const std::string json_timestamp = "\"timestamp\":";
const std::string json_data = "\"data\":";
const std::string json_server_marker = "server";
const std::string json_seconds = "seconds";
const std::string json_refresh_time = "\"refresh_time\":";
const std::string json_parser_class = "\"parser_class\":";
const std::string json_stations_data_marker = "stations";
const std::string json_station_uid = "\"station_uid\":";
const std::string json_station_uids = "\"station_uids\":";
const std::string json_measurement_size_marker = "size";
const std::string json_measurement_time_marker = "times";
const std::string json_available_metrics_marker = "available_metrics";
const std::string json_available_periods_marker = "available_periods";
const std::string json_empty = "{}";
const std::string json_acquistion_server_type = "A";
const std::string json_storage_and_aggregation_server_type = "S&A";

// ~~ stormy::common::rest::const;
}}}}