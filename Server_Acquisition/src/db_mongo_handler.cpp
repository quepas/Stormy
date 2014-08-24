#include "db_mongo_handler.h"

#include <Poco/MongoDB/Cursor.h>
#include <Poco/MongoDB/Document.h>
#include <Poco/MongoDB/ResponseMessage.h>

#include <Poco/Exception.h>

#include "acquisition_config_metrics.h"
#include "db_constant.h"
#include "util.h"

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
  : logger_(Logger::get("db/MongoHandler")),
    Handler("MongoDB")
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
    connected_ = true;
  } catch (const Exception& e) {
    logger_.error("[db/MongoHandler] Exception: " + e.displayText());
    connected_ = false;
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
  if(!connected_ || measurement.empty()) return;

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
  if(!connected_) return;
  auto delete_request = database_->createDeleteRequest(constant::collection_station);
  connection_->sendRequest(*delete_request);
  CheckLastErrors();
}

void MongoHandler::insertStationsData(
  const std::vector<entity::Station>& stations)
{
  if(!connected_) return;
  for(auto it = stations.begin(); it != stations.end(); ++it)
    insertStationData(*it);
}

void MongoHandler::insertStationData(entity::Station station)
{
  if(!connected_) return;
  auto insert_request = database_->createInsertRequest(constant::collection_station);
  insert_request->addNewDocument()
    .add(constant::mongo_id, station.uid)
    .add(constant::name, station.name)
    .add(constant::parser_class, station.parser_class)
    .add(constant::refresh_time, static_cast<int>(station.refresh_time))  // TODO: int/uint32_t?
    .add(constant::url, station.url);
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
}

vector<entity::Station> MongoHandler::getStationsData()
{
  auto result = vector<entity::Station>();
  if (!connected_) return result;

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
  if(!connected_) return result;

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
  if(!connected_ || metric.empty()) return false;

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
  if(!connected_) return false;
  auto delete_request = database_->createDeleteRequest(constant::collection_metrics);
  connection_->sendRequest(*delete_request);
  CheckLastErrors();
  return true;
}

void MongoHandler::ExpireData()
{
  if(!ValidateConnection()) return;
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
  if(!ValidateConnection()) return result;

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

uint32_t MongoHandler::CountMeasureSetsForStationByUID(string uid)
{
  auto count_request = database_->createCountRequest(constant::station_uid_prefix + uid);
  ResponseMessage response;
  connection_->sendRequest(*count_request, response);
  uint32_t result = 0;
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
  if(!ValidateConnection()) return result;

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

      for (auto& name : available_keys) {
        logger_.information("Key: " + name);
      }
      
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);

  /*while (cursor->more()) {
    auto current_measure_set = cursor->next();
    set<string> available_fields;
    current_measure_set.getFieldNames(available_fields);
    auto measure_set = vector<entity::Measurement>();
    time_t current_ts = 0;
    for (auto it = available_fields.begin(); it != available_fields.end(); ++it) {
      entity::Measurement measure;
      measure.code = *it;
      measure.station_uid = station_uid;
      auto field = current_measure_set.getField(measure.code);

      if (*it != constant::mongo_id) {
        if (field.isNumber()) {
          measure.value_number = field.numberDouble();
          measure.is_numeric = true;
        }
        else {
          measure.value_text = field.String();
          measure.is_numeric = false;
        }
        measure_set.push_back(measure);
      } else {
        current_ts = field.numberLong();
      }
    }
    for (auto m_it = measure_set.begin(); m_it != measure_set.end(); ++m_it) {
      m_it->timestamp = *std::gmtime(&current_ts);
    }
    result.insert(make_pair(current_ts, measure_set));
  }*/
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
