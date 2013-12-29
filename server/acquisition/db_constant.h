#pragma once

namespace stormy {
  namespace db {
    namespace constant {

const std::string mongo_id = "_id";
const std::string mongo_db_name = "test";
const std::string mongo_meteo_coll = "meteo";
const std::string mongo_station_coll = "station";
const std::string mongo_metrics_coll = "type";
const std::string id = "id";
const std::string code = "code";
const std::string station_uid_prefix = "S";
const std::string url = "url";
const std::string parser_class = "parserClass";
const std::string refresh_time = "refreshTime";
const std::string name = "name";
const std::string date = "date";
const std::string time = "time";
const std::string timestamp = "timestamp";
const std::string number = "number";
const std::string text = "text";
const std::string type = "type";
const std::string unit = "unit";
const std::string format = "format";
const std::string equivalents = "equivalents";
const std::string is_meteo = "isMeteo";

const std::string db_meteo = mongo_db_name + "." + mongo_meteo_coll;
const std::string db_station = mongo_db_name + "." + mongo_station_coll;
const std::string db_type = mongo_db_name + "." + mongo_metrics_coll;
// ~~ stormy::db::constant
}}}
