#include "MongoDBHandler.h"

#include <iostream>

#include <ctime>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
#include <Poco/Timestamp.h>
#include <Poco/NumberParser.h>
#include <Poco/NumberFormatter.h>

#include "acquisition_config_metrics.h"
#include "acquisition_util.h"
#include "acquisition_constant.h"
#include "../../common/util.h"

using namespace stormy::common;
using std::time_t;
using std::gmtime;
using std::mktime;
using std::vector;
using std::make_pair;
using std::map;
using Poco::NumberFormatter;

using namespace Stormy;
using namespace Meteo;
using namespace Poco;
using namespace std;
using namespace mongo;

MongoDBHandler::MongoDBHandler( string dbAddress /*= "localhost"*/ )
	:	connection(DBClientConnection()),
    Handler("MongoDB")
{
	if(!dbAddress.empty())
		connect(dbAddress);
}

MongoDBHandler::~MongoDBHandler()
{

}

void MongoDBHandler::connect( string dbAddress )
{
	try {
		connection.connect(dbAddress);
		cout << "Connected to MongoDB database: " << dbAddress << endl;
		connected_ = true;      
	} catch (const DBException& e) {
		cout << "[ERROR]: " << e.what() << endl;
		connected_ = false;
	}
}

void MongoDBHandler::insertMeteoData(std::vector<entity::Measurement> measurement)
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
  bsonBuilder.append(
    stormy::acquisition::constant::mongoId, 
    mktime(&timestamp));	
	connection.insert(
    stormy::acquisition::util::GetMeteoDb() + 
      "." + 
      stormy::acquisition::constant::stationIdPrefix + 
      station_uid, 
    bsonBuilder.obj());
}

void MongoDBHandler::clearStationsData()
{
	if(!connected_) return;
	connection.dropCollection(stormy::acquisition::util::GetStationDb());
}

void MongoDBHandler::insertStationsData(const std::vector<entity::Station>& stations)
{
	if(!connected_) return;
	for(auto it = stations.begin(); it != stations.end(); ++it)
		insertStationData(*it);
}

void MongoDBHandler::insertStationData(entity::Station station)
{
	if(!connected_) return;
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(stormy::acquisition::constant::mongoId, station.uid);
	bsonBuilder.append(stormy::acquisition::constant::name, station.name);
	bsonBuilder.append(stormy::acquisition::constant::parserClass, station.parser_class);
	bsonBuilder.append(stormy::acquisition::constant::refreshTime, station.refresh_time);
	bsonBuilder.append(stormy::acquisition::constant::url, station.url);
	connection.insert(stormy::acquisition::util::GetStationDb(), bsonBuilder.obj());
}

vector<entity::Station> MongoDBHandler::getStationsData()
{
	auto result = vector<entity::Station>();
	if (!connected_) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetStationDb(), BSONObj());
	while (cursor->more()) {    
		BSONObj current = cursor->next();
		entity::Station station;
		station.uid = current.getStringField(stormy::acquisition::constant::mongoId.c_str());
		station.name = current.getStringField(stormy::acquisition::constant::name.c_str());
		station.parser_class = current.getStringField(stormy::acquisition::constant::parserClass.c_str());
		station.refresh_time = current.getIntField(stormy::acquisition::constant::refreshTime.c_str());
		station.url = current.getStringField(stormy::acquisition::constant::url.c_str());
		result.push_back(station);
	}
	return result;
}

vector<entity::Metrics> MongoDBHandler::getTypesData()
{
	vector<entity::Metrics> result;
	if(!connected_) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetTypeDb(), BSONObj());
	while( cursor -> more() ) {    
		BSONObj current = cursor -> next();
		entity::Metrics metrics;
		metrics.code = current.getStringField(stormy::acquisition::constant::id.c_str());
		metrics.equivalents = current.getStringField(stormy::acquisition::constant::equivalents.c_str());
		metrics.type = current.getStringField(stormy::acquisition::constant::type.c_str());
		metrics.unit = current.getStringField(stormy::acquisition::constant::unit.c_str());
		metrics.format = current.getStringField(stormy::acquisition::constant::format.c_str());
		metrics.is_meteo = current.getBoolField(stormy::acquisition::constant::isMeteo.c_str());		
		result.push_back(metrics);
	}
	return result;
}

bool MongoDBHandler::insertTypesData(const vector<entity::Metrics>& metrics_vec)
{
	if(!connected_ || metrics_vec.empty()) return false;
	stormy::common::Each(metrics_vec, [&](entity::Metrics metrics) {		
		BSONObjBuilder bsonBuilder;
		bsonBuilder.append(stormy::acquisition::constant::id, metrics.code);
		bsonBuilder.append(stormy::acquisition::constant::equivalents, metrics.equivalents);
		bsonBuilder.append(stormy::acquisition::constant::type, metrics.type);
		bsonBuilder.append(stormy::acquisition::constant::unit, metrics.unit);
		bsonBuilder.append(stormy::acquisition::constant::format, metrics.format);
		bsonBuilder.append(stormy::acquisition::constant::isMeteo, metrics.is_meteo);
		connection.insert(stormy::acquisition::util::GetTypeDb(), bsonBuilder.obj());
	});
	return true;
}

bool MongoDBHandler::clearTypesData()
{
	if(!connected_) return false;
	connection.dropCollection(stormy::acquisition::util::GetTypeDb());
	return true;
}

void MongoDBHandler::ExpireData()
{
  if(!ValidateConnection()) return;
  auto stations_uid = FetchStationsUID();
  time_t expiration_time = stormy::common::LocaltimeNow() - expiration_seconds();  
  
  for (auto it = stations_uid.begin(); it != stations_uid.end(); ++it) {   
    connection.remove(
      stormy::acquisition::util::GetMeteoDb() + "." + stormy::acquisition::constant::stationIdPrefix + *it, 
      QUERY(stormy::acquisition::constant::mongoId << LT << NumberFormatter::format(expiration_time)));    
  }
}

vector<string> MongoDBHandler::FetchStationsUID()
{
  auto result = vector<string>();
  if(!ValidateConnection()) return result;

  auto_ptr<DBClientCursor> cursor = 
    connection.query(stormy::acquisition::util::GetStationDb(), BSONObj());

  while (cursor->more()) {
    auto current_value = cursor->next();
    string station_uid = current_value
      .getStringField(stormy::acquisition::constant::mongoId.c_str());
    result.push_back(station_uid);
  }
  return result;
}

entity::Station MongoDBHandler::GetStationByUID(string uid)
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

uint32_t MongoDBHandler::CountMeasureSetsForStationByUID(string uid)
{
  return connection.count(
    stormy::acquisition::util::GetMeteoDb() +
    "." +
    stormy::acquisition::constant::stationIdPrefix +
    uid);
}

map<time_t, vector<entity::Measurement>> 
  MongoDBHandler::GetMeasureSetsForStationBetweenTS(
    string station_uid, 
    time_t from, 
    time_t to)
{
  auto result = map<time_t, vector<entity::Measurement>>();
  if(!ValidateConnection()) return result;

  auto_ptr<DBClientCursor> cursor = connection.query(
    stormy::acquisition::util::GetMeteoDb() +
      "." +
      stormy::acquisition::constant::stationIdPrefix +
      station_uid,
    QUERY(stormy::acquisition::constant::mongoId << GTE << from << LTE << to));

  while (cursor->more()) {
    auto current_measure_set = cursor->next();
    set<string> available_fields;
    current_measure_set.getFieldNames(available_fields);
    auto measure_set = vector<entity::Measurement>();
    time_t current_ts = 0;
    for (auto it = available_fields.begin(); it != available_fields.end(); ++it) {
      entity::Measurement measure;
      measure.code = *it;
      auto field = current_measure_set.getField(measure.code);

      if (*it != stormy::acquisition::constant::mongoId) {        
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
    result.insert(make_pair(current_ts, measure_set));
  }
}

map<time_t, vector<entity::Measurement>> MongoDBHandler::
  GetMeasureSetsForStationAndTS(string station_uid, time_t ts)
{
  return GetMeasureSetsForStationBetweenTS(station_uid, ts, ts);
}

map<time_t, vector<entity::Measurement>> MongoDBHandler::
  GetAllMeasureSetsForStation(string station_uid)
{
  return GetMeasureSetsForStationBetweenTS(station_uid, 0, LocaltimeNow());
}
