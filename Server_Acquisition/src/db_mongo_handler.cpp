#include "db_mongo_handler.hpp"

#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>
#include <Poco/Exception.h>
#include <Poco/String.h>
#include <Poco/Types.h>
#include <Poco/MongoDB/Cursor.h>
#include <Poco/MongoDB/Document.h>
#include <Poco/MongoDB/ResponseMessage.h>

#include "db_constant.hpp"
#include "common/util.h"

using Poco::DateTime;
using Poco::DateTimeParser;
using Poco::cat;
using Poco::Int64;
using Poco::MongoDB::Cursor;
using Poco::MongoDB::Connection;
using Poco::MongoDB::Database;
using Poco::MongoDB::Document;
using Poco::MongoDB::ResponseMessage;
using std::string;
using std::to_string;

using namespace stormy::common;
using std::time_t;
using std::gmtime;
using std::mktime;
using std::vector;
using std::make_pair;
using std::map;

using Poco::Exception;
using Poco::Logger;

namespace stormy {
  namespace db {

MongoHandler::MongoHandler(string dbAddress /*= "localhost"*/, unsigned int port /*= 27017*/)
  : logger_(Logger::get("main"))
{
  if(!dbAddress.empty())
    Connect("test2", dbAddress, port);
}

MongoHandler::~MongoHandler()
{
  delete database_;
  connection_->disconnect();
  delete connection_;
}

void MongoHandler::Connect(std::string db_name, std::string db_address, unsigned int port)
{
  db_name_ = db_name;
  try {
    connection_ = new Connection(db_address, port);
    database_ = new Database(db_name_);
    logger_.information(
      "[db/MongoHandler] Connected to MongoDB database: "
        + db_address
        + ":"
        + to_string(port));
    is_connected_ = true;
  } catch (const Exception& e) {
    logger_.error("[db/MongoHandler] Exception: " + e.displayText());
    is_connected_ = false;
  }
}

void MongoHandler::CheckLastErrors()
{
  string last_error_msg = database_->getLastError(*connection_);
  if (!last_error_msg.empty())
  {
    logger_.error("Last Error: " + last_error_msg);
  }
}

void MongoHandler::InsertMeteo(const MeteoData_& meteo_data)
{
  if (!is_connected_ || meteo_data.empty()) return;

  auto insert_request = database_->createInsertRequest(
    COLLECTION_METEO
      + "."
      + meteo_data[0].station_uid);

  auto& document = insert_request->addNewDocument();
  for (auto& data : meteo_data) {
    if (data.is_numeric) {
      document.add(data.code, data.value_number);
    } else {
      document.add(data.code, data.value_text);
    }
  }
  tm timestamp = meteo_data[0].timestamp;
  document.add(MONGO_ID, mktime(&timestamp) + 3600);
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
}

void MongoHandler::InsertMeteo(const MeteoData& meteo_data)
{
  if (!is_connected_) return;
  DateTime date_time;
  int time_zone;
  if (!DateTimeParser::tryParse(meteo_data.datetime, date_time, time_zone)) {
    logger_.warning("[db/MongoHandler] Invalid datetime. Skipping insert.");
    return;
  }
  auto insert_request = database_->createInsertRequest(
    COLLECTION_METEO + "."
      + meteo_data.station_id);

  auto& document = insert_request->addNewDocument();
  for (auto& entry : meteo_data.entries) {
    if (!entry.str_data.empty()) {
      document.add(entry.element_id, entry.str_data);
    }
    else {
      document.add(entry.element_id, entry.data);
    }
  }
  document.add(MONGO_ID, date_time.timestamp().epochTime());
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
}

bool MongoHandler::IsMeteoExists(const MeteoData& meteo_data)
{
  Cursor cursor(db_name_, COLLECTION_METEO + "." + meteo_data.station_id);
  DateTime date_time;
  int time_zone;
  if (!DateTimeParser::tryParse(meteo_data.datetime, date_time, time_zone)) {
    logger_.warning("[db/MongoHandler] Wrong date time. Skipping existence check.");
  }
  cursor.query().selector().add(MONGO_ID, date_time.timestamp().epochTime());
  auto response = cursor.next(*connection_);
  return response.documents().begin() != response.documents().end();
}

void MongoHandler::InsertStations(const StationSettings& stations)
{
  if (!is_connected_) return;
  auto insert_request = database_->createInsertRequest(COLLECTION_STATION);
  for (auto& station : stations) {
    insert_request->addNewDocument()
      .add(MONGO_ID, station.id)
      .add(NAME, station.name)
      .add(PARSER_SCRIPT, station.parse_script)
      .add(UPDATE_TIME, static_cast<int>(station.update_time))
      .add(URL, station.url);
  }
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
}

vector<entity::Station> MongoHandler::GetStations()
{
  auto result = vector<entity::Station>();
  if (!is_connected_) return result;

  Cursor cursor(db_name_, COLLECTION_STATION);
  ResponseMessage& response = cursor.next(*connection_);

  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      entity::Station station;
      station.uid = document->get<string>(MONGO_ID);
      station.name = document->get<string>(NAME);
      station.parser_class = document->get<string>(PARSER_SCRIPT);
      station.refresh_time = document->get<int>(UPDATE_TIME);
      station.url = document->get<string>(URL);
      result.push_back(station);
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return result;
}

vector<entity::Metrics> MongoHandler::GetMetrics()
{
  vector<entity::Metrics> result;
  if(!is_connected_) return result;

  Cursor cursor(db_name_, COLLECTION_METRICS);
  ResponseMessage& response = cursor.next(*connection_);

  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      entity::Metrics metrics;
      metrics.code = document->get<string>(MONGO_ID);
      metrics.equivalents = document->get<string>(LABELS);
      metrics.type = document->get<string>(TYPE);
      metrics.unit = document->get<string>(UNIT);
      metrics.format = document->get<string>(FORMAT);
      metrics.is_meteo = document->get<bool>(IS_METEO);
      result.push_back(metrics);
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return result;
}

bool MongoHandler::InsertMetrics(const MetricsSettings& metrics)
{
  if (!is_connected_ || metrics.empty()) return false;

  auto insert_request = database_->createInsertRequest(COLLECTION_METRICS);
  for (auto& metric : metrics) {
    string labels = cat(string(";"), metric.labels.begin(), metric.labels.end());
    insert_request->addNewDocument()
      .add(MONGO_ID, metric.name)
      .add(TYPE, metric.type)
      .add(UNIT, metric.unit)
      .add(FORMAT, metric.format)
      .add(IS_METEO, metric.is_meteo)
      .add(LABELS, labels);
  }
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
  return true;
}

void MongoHandler::ExpireData()
{
  if(!is_connected_) return;
  auto stations_uid = FetchStationsUID();
  time_t expiration_time = common::LocaltimeNow() - expiration_seconds();

  for (auto& uid : stations_uid) {
    auto delete_request = database_->createDeleteRequest(uid);
    delete_request->selector()
      .addNewDocument("query")
        .addNewDocument(MONGO_ID)
          .add("$lt", to_string(expiration_time));
    connection_->sendRequest(*delete_request);
    CheckLastErrors();
  }
}

vector<string> MongoHandler::FetchStationsUID()
{
  auto result = vector<string>();
  if (!is_connected_) return result;

  Cursor cursor(db_name_, COLLECTION_STATION);
  cursor.query().returnFieldSelector().add(MONGO_ID, 1);
  auto response = cursor.next(*connection_);

  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      result.push_back(document->get<string>(MONGO_ID));
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return result;
}

unsigned int MongoHandler::CountMeteo(string uid)
{
  auto count_request = database_->createCountRequest(uid);
  ResponseMessage response;
  connection_->sendRequest(*count_request, response);
  unsigned int result = 0;
  if (response.hasDocuments()) {
    result = response.documents()[0]->get<int>("n");
  }
  return result;
}

MeteoDataMap MongoHandler::GetMeteoBetween(string station_uid, time_t from, time_t to)
{
  MeteoDataMap result;
  if (!is_connected_) return result;

  Cursor cursor(db_name_, COLLECTION_METEO + "." + station_uid);
  cursor.query().selector()
    .addNewDocument(MONGO_ID)
      .add("$gte", from)
      .add("$lte", to);

  auto& response = cursor.next(*connection_);
  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      vector<string> available_keys;
      document->elementNames(available_keys);

      time_t current_ts = 0;
      MeteoData_ measure_vec;
      for (auto& key : available_keys) {
        entity::Measurement measure;
        measure.code = key;
        measure.station_uid = station_uid;

        if (key == MONGO_ID) {
          current_ts = document->get<Int64>(key);
        }
        else {
          if (document->isType<double>(key)) {
            measure.value_number = document->get<double>(key);
            measure.is_numeric = true;
          }
          else if (document->isType<string>(key)) {
            measure.value_text = document->get<string>(key);
            measure.is_numeric = false;
          }
          measure_vec.push_back(measure);
        }
      }
      tm current_time = *gmtime(&current_ts);
      for (auto& measure : measure_vec) {
        measure.timestamp = current_time;
      }
      result.insert(make_pair(current_ts, measure_vec));
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return result;
}

MeteoDataMap MongoHandler::GetMeteo(string station_uid, time_t ts)
{
  return GetMeteoBetween(station_uid, ts, ts);
}

MeteoDataMap MongoHandler::GetAllMeteo(string station_uid)
{
  return GetMeteoBetween(station_uid, 0, LocaltimeNow());
}

StationData MongoHandler::GetStationByUID(string uid)
{
  // TODO: reimplement this!
  auto stations = GetStations();
  StationData result;
  result.uid = "";

  for (auto it = stations.begin(); it != stations.end(); ++it) {
    if (uid == it->uid)
      return *it;
  }
  return result;
}

bool MongoHandler::ClearCollection(string collection_name)
{
  if (!is_connected_) return false;
  auto delete_request = database_->createDeleteRequest(collection_name);
  connection_->sendRequest(*delete_request);
  CheckLastErrors();
  return true;
}

bool MongoHandler::ClearStations()
{
  return ClearCollection(COLLECTION_STATION);
}

bool MongoHandler::ClearMetrics()
{
  return ClearCollection(COLLECTION_METRICS);
}

}}
// ~~ stormy::db::MongoHandler
