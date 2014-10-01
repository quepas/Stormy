#include "db_mongo_handler.hpp"
#include "db_constant.hpp"
#include "common/util.h"

#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>
#include <Poco/Exception.h>
#include <Poco/Types.h>
#include <Poco/MongoDB/Array.h>
#include <Poco/MongoDB/Cursor.h>
#include <Poco/MongoDB/Document.h>
#include <Poco/MongoDB/ResponseMessage.h>

using Poco::DateTime;
using Poco::DateTimeParser;
using Poco::Int64;
using Poco::Exception;
using Poco::Logger;
using Poco::MongoDB::Array;
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

void MongoHandler::InsertMeteo(const MeteoData& meteo_data)
{
  if (!is_connected_) return;
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
  document.add(MONGO_ID, meteo_data.datetime.epoch_time);
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
}

bool MongoHandler::IsMeteoExists(const MeteoData& meteo_data)
{
  Cursor cursor(db_name_, COLLECTION_METEO + "." + meteo_data.station_id);
  cursor.query().selector().add(MONGO_ID, meteo_data.datetime.epoch_time);
  auto response = cursor.next(*connection_);
  return response.documents().begin() != response.documents().end();
}

void MongoHandler::InsertStations(const vector<StationData>& stations)
{
  if (!is_connected_) return;
  auto insert_request = database_->createInsertRequest(COLLECTION_STATION);
  for (auto& station : stations) {
    insert_request->addNewDocument()
      .add(MONGO_ID, station.id)
      .add(NAME, station.name)
      .add(PARSER_SCRIPT, station.parse_script)
      .add(UPDATE_TIME, static_cast<int>(station.update_time))
      .add(URL, station.url)
      .add(TIME_ZONE, station.time_zone);
  }
  connection_->sendRequest(*insert_request);
  CheckLastErrors();
}

vector<StationData> MongoHandler::GetStations()
{
  auto result = vector<StationData>();
  if (!is_connected_) return result;

  Cursor cursor(db_name_, COLLECTION_STATION);
  ResponseMessage& response = cursor.next(*connection_);

  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      StationData station;
      station.id = document->get<string>(MONGO_ID);
      station.name = document->get<string>(NAME);
      station.parse_script = document->get<string>(PARSER_SCRIPT);
      station.update_time = document->get<int>(UPDATE_TIME);
      station.url = document->get<string>(URL);
      station.time_zone = document->get<int>(TIME_ZONE);
      result.push_back(station);
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return result;
}

vector<MeteoElement> MongoHandler::GetMeteoElements()
{
  vector<MeteoElement> meteo_elements;
  if(!is_connected_) return meteo_elements;

  Cursor cursor(db_name_, COLLECTION_METEO_ELEMENT);
  ResponseMessage& response = cursor.next(*connection_);
  do {
    CheckLastErrors();
    for (auto& document : response.documents()) {
      MeteoElement element;
      element.id = document->get<string>(MONGO_ID);
      element.type = document->get<string>(TYPE);
      Array::Ptr labels_array = document->get<Array::Ptr>(LABELS);
      for (unsigned idx = 0; idx < labels_array->size(); ++idx) {
        element.labels.push_back(labels_array->get<string>(idx));
      }
      meteo_elements.push_back(element);
    }
    response = cursor.next(*connection_);
  } while (response.cursorID() != 0);
  return meteo_elements;
}

bool MongoHandler::InsertMeteoElements(const vector<MeteoElement>& meteo_elements)
{
  if (!is_connected_ || meteo_elements.empty()) return false;

  auto insert_request = database_->createInsertRequest(COLLECTION_METEO_ELEMENT);
  for (auto& element : meteo_elements) {
    int counter = 0;
    Array::Ptr labels_array = new Array();
    for (auto& label : element.labels) {
      labels_array->add(to_string(counter++), label);
    }
    auto& current_document = insert_request->addNewDocument()
      .add(MONGO_ID, element.id)
      .add(TYPE, element.type)
      .add(LABELS, labels_array);
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
  result.id = "";

  for (auto it = stations.begin(); it != stations.end(); ++it) {
    if (uid == it->id)
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
  return ClearCollection(COLLECTION_METEO_ELEMENT);
}

}}
// ~~ stormy::db::MongoHandler
