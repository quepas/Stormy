#pragma once

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
// api: /meteo/:station_uid
const std::string meteo_station_uid_request_pattern = "/meteo/[0-9a-f]{32}/?";
// api: /meteo/:station_uid/:timestamp
const std::string meteo_station_uid_ts_request_pattern = "/meteo/[0-9a-f]{32}/[0-9]{10}/?";
// api: /aggregate
const std::string aggregate_request_pattern = "/aggregate/?";
// api: /aggregate/:station_uid/
const std::string aggregate_station_uid_request_pattern = "/aggregate/[0-9a-f]{32}/?";
// api: /aggregate/:station_uid/:period_name
const std::string aggregate_station_period_request_pattern = "/aggregate/[0-9a-f]{32}/[a-zA-Z]+/?";
// api: /aggregate/:station_uid/:period_name
const std::string aggregate_station_period_time_request_pattern = "/aggregate/[0-9a-f]{32}/[a-zA-Z]+/[0-9]{10}/?";
// api: /metrics
const std::string metrics_request_pattern = "/metrics/?";
// api: /metrics/:metrics_code
const std::string metrics_info_request_pattern = "/metrics/[a-zA-Z0-9]+/?";
// api: /info
const std::string info_request_pattern = "/info/?";
// api: /period
const std::string period_request_pattern = "/period/?";
// api: /operation
const std::string operation_request_pattern = "/operation/?";
// api: /export
const std::string export_request_pattern = "/export/?";
// api: /export/:station_id
const std::string export_station_uid_request_pattern = "/export/[0-9a-f]{32}/?";
// api query: ?param_one=value1&param_two=value2 (...)
const std::string uri_query_vars_pattern = ".([a-zA-Z_]+=[a-zA-Z0-9,]+&?)*";
/*
 * api available parameters (as filters)
 */
// ?metrics=:metrics_code
const std::string metrics_parameter = "metrics";
// ?from=:timestamp
const std::string from_parameter = "from";
// ?to=:timestamp
const std::string to_parameter = "to";
// ?from_last_hours=:hours
const std::string from_last_hours_parameter = "from_last_hours";

/*
 *  CSV constants
 */
const std::string csv_delimiter = ";";
const std::string csv_new_line = "\n";
const std::string csv_timestamp = "timestamp";
const std::string csv_missing_value = "-";
/*
 *  JSON constants
 */
const std::string UID = "uid";
const std::string IS_METEO = "is_meteo";
const std::string NAME = "name";
const std::string CODE = "code";
const std::string DATA = "data";
const std::string URL = "url";
const std::string TYPE = "type";
const std::string METRICS = "metrics";
const std::string LABELS = "labels";
const std::string FORMAT = "format";
const std::string UNIT = "unit";
const std::string ERROR_ = "error";
const std::string MSG = "msg";
const std::string METEO = "meteo";
const std::string AGGREGATE = "aggregate";
const std::string TIMEZONE = "timezone";
const std::string TIME_NOW = "time_now";
const std::string TIMESTAMP = "timestamp";
const std::string SERVER = "server";
const std::string UPDATE_TIME = "update_time";
const std::string STATION = "station";
const std::string STATUS = "status";
const std::string JSON_EMPTY = "{}";
const std::string PERIODS = "periods";
const std::string OPERATIONS = "operations";

}}}}
// ~~ stormy::common::rest::const;
