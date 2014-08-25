#include "db_mongo_handler.h"

#include <Poco/Exception.h>
#include <Poco/Types.h>
#include <Poco/MongoDB/Cursor.h>
#include <Poco/MongoDB/Document.h>
#include <Poco/MongoDB/ResponseMessage.h>

#include "db_constant.h"
#include "util.h"

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
  : logger_(Logger::get("db/MongoHandler"))
{
  if(!dbAddress.empty())
    Connect("test2", dbAddress, port);
}

MongoHandler::~MongoHandler()
{
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

void MongoHandler::InsertMeasurement(vector<entity::Measurement> measurement)
{
  if(!is_connected_ || measurement.empty()) return;

  auto insert_request = database_->createInsertRequest(
    constant::collection_meteo 
      + "." 
      + constant::station_uid_prefix 
      + measurement[0].station_uid);

  auto& document = insert_request->addNewDocument();
  for (auto& data : measurement) {
    if (data.is_numeric) {
      document.add(data.code, data.value_number);
    } else {
      document.add(data.code, data.value_text);
    }
  }
  document.add(constant::mongo_id, mktime(&measurement[0].timestamp) + 3600);
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
}

void MongoHandler::clearStationsData()
{
  if(!is_connected_) return;
  auto delete_request = database_->createDeleteRequest(constant::collection_station);
  connection_->sendRequest(*delete_request);
  CheckLastErrors();
}

void MongoHandler::InsertStationsData(
  const StationSettings& stations)
{
  if(!is_connected_) return;
  for(auto it = stations.begin(); it != stations.end(); ++it)
    InsertStationData(*it);
}

void MongoHandler::InsertStationData(StationSetting station)
{
  if(!is_connected_) return;
  auto insert_request = database_->createInsertRequest(constant::collection_station);
  insert_request->addNewDocument()
    .add(constant::mongo_id, MD5(station.url))
    .add(constant::name, station.name)
    .add(constant::parser_class, station.parse_script)
    .add(constant::refresh_time, static_cast<int>(station.update_time))
    .add(constant::url, station.url);
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
}

vector<entity::Station> MongoHandler::getStationsData()
{
  auto result = vector<entity::Station>();
  if (!is_connected_) return result;

  Cursor cursor(db_name_, constant::collection_station);
  ResponseMessage& response = cursor.next(*connection_);

  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      entity::Station station;
      station.uid = document->get<string>(constant::mongo_id);
      station.name = document->get<string>(constant::name);
      station.parser_class = document->get<string>(constant::parser_class);
      station.refresh_time = document->get<int>(constant::refresh_time);
      station.url = document->get<string>(constant::url);
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

  Cursor cursor(db_name_, constant::collection_metrics);
  ResponseMessage& response = cursor.next(*connection_);

  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      entity::Metrics metrics;
      metrics.code = document->get<string>(constant::mongo_id);
      metrics.equivalents = document->get<string>(constant::equivalents);
      metrics.type = document->get<string>(constant::type);
      metrics.unit = document->get<string>(constant::unit);
      metrics.format = document->get<string>(constant::format);
      metrics.is_meteo = document->get<bool>(constant::is_meteo);
      result.push_back(metrics);
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return result;
}

bool MongoHandler::InsertMetrics(const vector<entity::Metrics>& metric)
{
  if(!is_connected_ || metric.empty()) return false;

  auto insert_request = database_->createInsertRequest(constant::collection_metrics);
  for (auto& metric : metric) {
    insert_request->addNewDocument()
      .add(constant::mongo_id, metric.code)
      .add(constant::equivalents, metric.equivalents)
      .add(constant::type, metric.type)
      .add(constant::unit, metric.unit)
      .add(constant::format, metric.format)
      .add(constant::is_meteo, metric.is_meteo);
  }
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
  return true;
}

bool MongoHandler::RemoveMetrics()
{
  if(!is_connected_) return false;
  auto delete_request = database_->createDeleteRequest(constant::collection_metrics);
  connection_->sendRequest(*delete_request);
  CheckLastErrors();
  return true;
}

void MongoHandler::ExpireData()
{
  if(!is_connected_) return;
  auto stations_uid = FetchStationsUID();
  time_t expiration_time = common::LocaltimeNow() - expiration_seconds();

  for (auto& uid : stations_uid) {
    auto delete_request = database_->createDeleteRequest(constant::station_uid_prefix + uid);
    delete_request->selector()
      .addNewDocument("query")
        .addNewDocument(constant::mongo_id)
          .add("$lt", to_string(expiration_time));
    connection_->sendRequest(*delete_request);
    CheckLastErrors();
  }
}

vector<string> MongoHandler::FetchStationsUID()
{
  auto result = vector<string>();
  if (!is_connected_) return result;

  Cursor cursor(db_name_, constant::collection_station);
  cursor.query().returnFieldSelector().add(constant::mongo_id, 1);
  auto response = cursor.next(*connection_);

  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      result.push_back(document->get<string>(constant::mongo_id));
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return result;
}

unsigned int MongoHandler::CountMeasureSetsForStationByUID(string uid)
{
  auto count_request = database_->createCountRequest(constant::station_uid_prefix + uid);
  ResponseMessage response;
  connection_->sendRequest(*count_request, response);
  unsigned int result = 0;
  if (response.hasDocuments()) {
    result = response.documents()[0]->get<int>("n");
  }
  return result;
}

map<time_t, vector<entity::Measurement>>
  MongoHandler::GetMeasureSetsForStationBetweenTS(
    string station_uid,
    time_t from,
    time_t to)
{
  auto result = map<time_t, vector<entity::Measurement>>();
  if (!is_connected_) return result;

  Cursor cursor(db_name_, constant::collection_meteo + "." + constant::station_uid_prefix + station_uid);
  cursor.query().selector()
    .addNewDocument(constant::mongo_id)
      .add("$gte", from)
      .add("$lte", to);

  auto& response = cursor.next(*connection_);
  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      vector<string> available_keys;
      document->elementNames(available_keys);

      time_t current_ts = 0;
      vector<entity::Measurement> measure_vec;
      for (auto& key : available_keys) {
        entity::Measurement measure;
        measure.code = key;
        measure.station_uid = station_uid;

        if (key == constant::mongo_id) {
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
      tm current_time = *std::gmtime(&current_ts);
      for (auto& measure : measure_vec) {
        measure.timestamp = current_time;
      }
      result.insert(make_pair(current_ts, measure_vec));
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return result;
}

map<time_t, vector<entity::Measurement>> MongoHandler::
  GetMeasureSetsForStationAndTS(string station_uid, time_t ts)
{
  return GetMeasureSetsForStationBetweenTS(station_uid, ts, ts);
}

map<time_t, vector<entity::Measurement>> MongoHandler::
  GetAllMeasureSetsForStation(string station_uid)
{
  return GetMeasureSetsForStationBetweenTS(station_uid, 0, LocaltimeNow());
}

entity::Station MongoHandler::GetStationByUID(string uid)
{
  // TODO: reimplement this!
  auto stations = getStationsData();
  entity::Station result;
  result.uid = "";

  for (auto it = stations.begin(); it != stations.end(); ++it) {
    if (uid == it->uid)
      return *it;
  }
  return result;
}

}}
// ~~ stormy::db::MongoHandler
