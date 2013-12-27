#include "MongoDBHandler.h"

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

void MongoDBHandler::clearMeteosData()
{
	if(!connected_) return;
	connection.dropCollection(stormy::acquisition::util::GetMeteoDb());
}

void MongoDBHandler::insertMeteoData(std::vector<entity::Measurement> measurement)
{
	if(!connected_ || measurement.empty()) return;

  string station_uid = measurement[0].station_uid;
  tm timestamp = measurement[0].timestamp;

	BSONObjBuilder bsonBuilder;
  for (auto it = measurement.begin(); it != measurement.end(); ++it) {
    string key = it->code;
    string value = it->value_text;
    bsonBuilder.append(key, value);
  }
  bsonBuilder.append(
    stormy::acquisition::constant::mongoId, 
    NumberFormatter::format(mktime(&timestamp)));
	
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

vector<entity::Measurement> MongoDBHandler::getCurrentMeteoTypeData(
  string station_uid, 
  string metrics_code)
{
	auto result = vector<entity::Measurement>();
	if (!connected_) return result;

	auto_ptr<DBClientCursor> cursor = connection.query(
      stormy::acquisition::util::GetMeteoDb() + 
        "." +
			  stormy::acquisition::constant::stationIdPrefix + 
        station_uid, 
      Query().sort("_id", 0), 
      1);
	if (cursor -> more()) {
    entity::Measurement measure;   
		BSONObj current = cursor->next();
    time_t time = NumberParser::parse64(current
      .getStringField(stormy::acquisition::constant::mongoId.c_str()));
    measure.timestamp = *std::gmtime(&time);
      
		if(current.hasField(metrics_code)) {
      measure.code = metrics_code;
      measure.station_uid = station_uid;
      measure.value_text = string(current.getStringField(metrics_code.c_str()));
      result.push_back(measure);
		}    
	}
	return result;
}

vector<entity::Measurement> MongoDBHandler::getCurrentMeteoTypeDatas(
  string station_uid, 
  string metrics_code)
{
	auto result = vector<entity::Measurement>();
	if (!connected_) return result;

	auto_ptr<DBClientCursor> cursor = connection.query(
    stormy::acquisition::util::GetMeteoDb() + 
      "." +
      stormy::acquisition::constant::stationIdPrefix + 
      station_uid, 
    BSONObj());
	while (cursor->more()) {
		BSONObj current = cursor->next();
    entity::Measurement measure;
    time_t time = NumberParser::parse64(current
      .getStringField(stormy::acquisition::constant::mongoId.c_str()));
    measure.timestamp = *std::gmtime(&time);

		if (current.hasField(metrics_code)) {      
      measure.code = metrics_code;
      measure.station_uid = station_uid;
      measure.value_text = string(current.getStringField(metrics_code.c_str()));
			result.push_back(measure);
		}
	}
	return result;
}

vector<entity::Measurement> MongoDBHandler::getMeteoData(string station_uid)
{
	auto result = vector<entity::Measurement>();
	if (!connected_) return result;
	
	vector<entity::Metrics> types = getTypesData();
	auto_ptr<DBClientCursor> cursor = connection.query(
    stormy::acquisition::util::GetMeteoDb() + 
      "." +
		  stormy::acquisition::constant::stationIdPrefix + 
      station_uid, 
    BSONObj());
	while (cursor->more()) {
		BSONObj current = cursor->next();
		set<string> availableFields;
		current.getFieldNames(availableFields);

		entity::Measurement measure;
		for (auto it = types.begin(); it != types.end(); ++it) {
			string id = it->code;
      measure.code = id;
			if(availableFields.find(id) != availableFields.end()) {
				entity::Metrics metrics = 
          stormy::acquisition::config::Metrics::GetMetricsById(id, types);
				string value = current.getStringField(id.c_str());
				if(metrics.type == stormy::acquisition::constant::number)
					measure.value_number = NumberParser::parse64(value);
				else if(metrics.type == stormy::acquisition::constant::text)
					measure.value_text = value;
			}
		}
    time_t time = NumberParser::parse64(current
      .getStringField(stormy::acquisition::constant::mongoId.c_str()));
    measure.timestamp = *std::gmtime(&time);
    measure.station_uid = station_uid;
		
    /*
		if(current.hasField(stormy::acquisition::constant::reasonKey.c_str())) {
			measurement -> data[stormy::acquisition::constant::reasonKey] =
				string(current.getStringField(stormy::acquisition::constant::reasonKey.c_str()));
		}*/
		result.push_back(measure);
	}
	return result;
}

vector<entity::Measurement> MongoDBHandler::getMeteoDataNewerThan(
  string station_uid, 
  string requested_measure_ts)
{
	auto result = vector<entity::Measurement>();
	if(!connected_) return result;
	
	vector<entity::Metrics> types = getTypesData();
	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetMeteoDb() + "." +
		stormy::acquisition::constant::stationIdPrefix + station_uid, BSONObj());
	while( cursor -> more() ) {
		BSONObj current = cursor -> next();
		set<string> availableFields;
		current.getFieldNames(availableFields);

		entity::Measurement measure;
		for(auto it = types.begin(); it != types.end(); ++it) {
			string id = it->code;
      measure.code = id;
			if(availableFields.find(id) != availableFields.end()) {
				entity::Metrics type = stormy::acquisition::config::Metrics::GetMetricsById(id, types);
				string value = current.getStringField(id.c_str());
        if(type.type == stormy::acquisition::constant::number)
          measure.value_number = NumberParser::parse64(value);
        else if(type.type == stormy::acquisition::constant::text)
          measure.value_text = value;
			}
		}
    time_t fetched_measure_ts = NumberParser::parse64(current
      .getStringField(stormy::acquisition::constant::mongoId.c_str()));
    measure.timestamp = *std::gmtime(&fetched_measure_ts);
    measure.station_uid = station_uid;
    /*
		if(current.hasField(stormy::acquisition::constant::reasonKey.c_str())) {
			measurement -> data[stormy::acquisition::constant::reasonKey] =
				string(current.getStringField(stormy::acquisition::constant::reasonKey.c_str()));
		}*/
		if(fetched_measure_ts > NumberParser::parseUnsigned64(requested_measure_ts))								
			result.push_back(measure);		
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
