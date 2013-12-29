#include "db_mongo_handler.h"

#include <Poco/NumberFormatter.h>

#include "acquisition_config_metrics.h"
#include "acquisition_util.h"
#include "db_constant.h"
#include "../../common/util.h"

using namespace stormy::common;
using std::auto_ptr;
using std::time_t;
using std::gmtime;
using std::mktime;
using std::vector;
using std::make_pair;
using std::map;
using std::set;
using std::string;
using Poco::NumberFormatter;
using Poco::Logger;
using mongo::DBClientConnection;
using mongo::DBClientCursor;
using mongo::DBException;
using mongo::BSONObj;
using mongo::BSONObjBuilder;
using mongo::LT;
using mongo::LTE;
using mongo::GTE;

namespace stormy {
  namespace db {

MongoHandler::MongoHandler(string dbAddress /*= "localhost"*/)
  : logger_(Logger::get("db/MongoHandler")),	
    connection(DBClientConnection()),
    Handler("MongoDB")
{
	if(!dbAddress.empty())
		connect(dbAddress);
}

MongoHandler::~MongoHandler()
{

}

void MongoHandler::connect(string dbAddress)
{
	try {
		connection.connect(dbAddress);
		logger_.information("[db/MongoHandler] Connected to MongoDB database: " + dbAddress);
		connected_ = true;      
	} catch (const DBException& e) {
		logger_.error("[db/MongoHandler] Exception: " + string(e.what()));
		connected_ = false;
	}
}

void MongoHandler::insertMeteoData(vector<entity::Measurement> measurement)
{
	if(!connected_ || measurement.empty()) return;

  string station_uid = measurement[0].station_uid;
  tm timestamp = measurement[0].timestamp;  

	BSONObjBuilder bsonBuilder;
  for (auto it = measurement.begin(); it != measurement.end(); ++it) {
    string code = it->code;    
    if(it->is_numeric)
      bsonBuilder.append(code, it->value_number);
    else
      bsonBuilder.append(code, it->value_text);
  }
  bsonBuilder.append(constant::mongo_id, mktime(&timestamp));	
	connection.insert(constant::db_meteo + "." + constant::station_uid_prefix + 
      station_uid, bsonBuilder.obj());
}

void MongoHandler::clearStationsData()
{
	if(!connected_) return;
	connection.dropCollection(constant::db_station);
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
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(constant::mongo_id, station.uid);
	bsonBuilder.append(constant::name, station.name);
	bsonBuilder.append(constant::parser_class, station.parser_class);
	bsonBuilder.append(constant::refresh_time, station.refresh_time);
	bsonBuilder.append(constant::url, station.url);
	connection.insert(constant::db_station, bsonBuilder.obj());
}

vector<entity::Station> MongoHandler::getStationsData()
{
	auto result = vector<entity::Station>();
	if (!connected_) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(constant::db_station, BSONObj());
	while (cursor->more()) {    
		BSONObj current = cursor->next();
		entity::Station station;
		station.uid = current.getStringField(constant::mongo_id.c_str());
		station.name = current.getStringField(constant::name.c_str());
		station.parser_class = current.getStringField(
      constant::parser_class.c_str());
		station.refresh_time = current.getIntField(
      constant::refresh_time.c_str());
		station.url = current.getStringField(constant::url.c_str());
		result.push_back(station);
	}
	return result;
}

vector<entity::Metrics> MongoHandler::GetMetrics()
{
	vector<entity::Metrics> result;
	if(!connected_) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(constant::db_metrics, BSONObj());
	while( cursor -> more() ) {    
		BSONObj current = cursor -> next();
		entity::Metrics metrics;
		metrics.code = current.getStringField(constant::mongo_id.c_str());
		metrics.equivalents = current.getStringField(
      constant::equivalents.c_str());
		metrics.type = current.getStringField(constant::type.c_str());
		metrics.unit = current.getStringField(constant::unit.c_str());
		metrics.format = current.getStringField(constant::format.c_str());
		metrics.is_meteo = current.getBoolField(constant::is_meteo.c_str());		
		result.push_back(metrics);
	}
	return result;
}

bool MongoHandler::InsertMetrics(
  const vector<entity::Metrics>& metrics_vec)
{
	if(!connected_ || metrics_vec.empty()) return false;
	stormy::common::Each(metrics_vec, [&](entity::Metrics metrics) {		
		BSONObjBuilder bsonBuilder;
		bsonBuilder.append(constant::mongo_id, metrics.code);
		bsonBuilder.append(constant::equivalents, metrics.equivalents);
		bsonBuilder.append(constant::type, metrics.type);
		bsonBuilder.append(constant::unit, metrics.unit);
		bsonBuilder.append(constant::format, metrics.format);
		bsonBuilder.append(constant::is_meteo, metrics.is_meteo);
		connection.insert(constant::db_metrics, bsonBuilder.obj());
	});
	return true;
}

bool MongoHandler::RemoveMetrics()
{
	if(!connected_) return false;
	connection.dropCollection(constant::db_metrics);
	return true;
}

void MongoHandler::ExpireData()
{
  if(!ValidateConnection()) return;
  auto stations_uid = FetchStationsUID();
  time_t expiration_time = common::LocaltimeNow() - expiration_seconds();  
  
  for (auto it = stations_uid.begin(); it != stations_uid.end(); ++it) {   
    connection.remove(
      constant::db_meteo + "." + constant::station_uid_prefix + *it, 
      QUERY(constant::mongo_id << LT << 
              NumberFormatter::format(expiration_time)));    
  }
}

vector<string> MongoHandler::FetchStationsUID()
{
  auto result = vector<string>();
  if(!ValidateConnection()) return result;

  auto_ptr<DBClientCursor> cursor = 
    connection.query(constant::db_station, BSONObj());

  while (cursor->more()) {
    auto current_value = cursor->next();
    string station_uid = current_value
      .getStringField(constant::mongo_id.c_str());
    result.push_back(station_uid);
  }
  return result;
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

uint32_t MongoHandler::CountMeasureSetsForStationByUID(string uid)
{
  return connection.count(constant::db_meteo + "." +
    constant::station_uid_prefix + uid);
}

map<time_t, vector<entity::Measurement>> 
  MongoHandler::GetMeasureSetsForStationBetweenTS(
    string station_uid, 
    time_t from, 
    time_t to)
{
  auto result = map<time_t, vector<entity::Measurement>>();
  if(!ValidateConnection()) return result;

  auto_ptr<DBClientCursor> cursor = connection.query(constant::db_meteo + 
    "." + constant::station_uid_prefix + station_uid,
    QUERY(constant::mongo_id << GTE << from << LTE << to));

  while (cursor->more()) {
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
  }
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
// ~~ stormy::db::MongoHandler
}}
