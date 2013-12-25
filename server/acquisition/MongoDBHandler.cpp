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

using std::time_t;
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

void MongoDBHandler::insertMeteoData( MeasurementPtr measurement )
{
	if(!connected_ || !measurement.get()) return;

	BSONObjBuilder bsonBuilder;
	auto data = measurement -> data;
	for(auto it = data.begin(); it != data.end(); ++it)
	{
		string key = it -> first;
		string value = stormy::common::ToString(it -> second);
		bsonBuilder.append(key, value);
	}
	string stationId = measurement -> station -> stationId;
	connection.insert(stormy::acquisition::util::GetMeteoDb() + "." + stormy::acquisition::constant::stationIdPrefix
		+ stationId, bsonBuilder.obj());
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

StationPtrVector MongoDBHandler::getStationsData()
{
	auto result = StationPtrVector();
	if(!connected_) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetStationDb(), BSONObj());
	while( cursor -> more() ) {    
		BSONObj current = cursor -> next();
		StationPtr station(new Station());
		station -> stationId = current.getStringField(stormy::acquisition::constant::mongoId.c_str());
		station -> name = current.getStringField(stormy::acquisition::constant::name.c_str());
		station -> parserClass = current.getStringField(stormy::acquisition::constant::parserClass.c_str());
		station -> refreshTime = current.getIntField(stormy::acquisition::constant::refreshTime.c_str());
		station -> url = current.getStringField(stormy::acquisition::constant::url.c_str());
		result.push_back(station);
	}
	return result;
}

MeasurementPtr MongoDBHandler::getCurrentMeteoTypeData( string stationId, string typeId )
{
	auto result = MeasurementPtr(new Measurement());
	if(!connected_) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetMeteoDb() + "." +
			stormy::acquisition::constant::stationIdPrefix + stationId, Query().sort("_id", 0), 1);
	if(cursor -> more()) {
		BSONObj current = cursor -> next();
		result -> timestamp = Timestamp::fromEpochTime(
			NumberParser::parse64(current.getStringField(stormy::acquisition::constant::mongoId.c_str())));
		if(current.hasField(typeId)) {
			result -> data[typeId] = string(current.getStringField(typeId.c_str()));
		}
	}
	return result;
}

MeasurementPtrVector MongoDBHandler::getCurrentMeteoTypeDatas( string stationId, string typeId )
{
	auto result = MeasurementPtrVector();
	if(!connected_) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetMeteoDb() + "." +
			stormy::acquisition::constant::stationIdPrefix + stationId, BSONObj());
	while(cursor -> more()) {
		BSONObj current = cursor -> next();
		MeasurementPtr measurement(new Measurement());
		measurement -> timestamp = Timestamp::fromEpochTime(
			NumberParser::parse64(current.getStringField(stormy::acquisition::constant::mongoId.c_str())));
		if(current.hasField(typeId)) {
			measurement -> data[typeId] =
				string(current.getStringField(typeId.c_str()));
			result.push_back(measurement);
		}
	}
	return result;
}

MeasurementPtrVector MongoDBHandler::getMeteoData(string stationId)
{
	auto result = MeasurementPtrVector();
	if(!connected_) return result;
	
	TypePtrVector types = getTypesData();
	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetMeteoDb() + "." +
			stormy::acquisition::constant::stationIdPrefix + stationId, BSONObj());
	while( cursor -> more() ) {
		BSONObj current = cursor -> next();
		set<string> availableFields;
		current.getFieldNames(availableFields);

		MeasurementPtr measurement(new Measurement());
		for(auto it = types.begin(); it != types.end(); ++it) {
			string id = (*it) -> id;
			if(availableFields.find(id) != availableFields.end()) {
				TypePtr type = stormy::acquisition::config::Metrics::GetMetricsById(id, types);
				string value = current.getStringField(id.c_str());
				if(type -> valueType == stormy::acquisition::constant::number)
					measurement -> data[id]= stormy::acquisition::util::ExtractTemperature(value);
				else if(type ->valueType == stormy::acquisition::constant::text)
					measurement -> data[id]= value;
			}
		}
		measurement -> data[stormy::acquisition::constant::mongoId] =
			string(current.getStringField(stormy::acquisition::constant::mongoId.c_str()));
		if(current.hasField(stormy::acquisition::constant::reasonKey.c_str())) {
			measurement -> data[stormy::acquisition::constant::reasonKey] =
				string(current.getStringField(stormy::acquisition::constant::reasonKey.c_str()));
		}
		result.push_back(measurement);
	}
	return result;
}

MeasurementPtrVector MongoDBHandler::getMeteoDataNewerThan( string stationId, string timestamp )
{
	auto result = MeasurementPtrVector();
	if(!connected_) return result;
	
	TypePtrVector types = getTypesData();
	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetMeteoDb() + "." +
		stormy::acquisition::constant::stationIdPrefix + stationId, BSONObj());
	while( cursor -> more() ) {
		BSONObj current = cursor -> next();
		set<string> availableFields;
		current.getFieldNames(availableFields);

		MeasurementPtr measurement(new Measurement());
		for(auto it = types.begin(); it != types.end(); ++it) {
			string id = (*it) -> id;
			if(availableFields.find(id) != availableFields.end()) {
				TypePtr type = stormy::acquisition::config::Metrics::GetMetricsById(id, types);
				string value = current.getStringField(id.c_str());
				if(type -> valueType == stormy::acquisition::constant::number)
					measurement -> data[id]= stormy::acquisition::util::ExtractTemperature(value);
				else if(type ->valueType == stormy::acquisition::constant::text)
					measurement -> data[id]= value;
			}
		}
		string timestampFromId = current.getStringField(stormy::acquisition::constant::mongoId.c_str());
		measurement -> data[stormy::acquisition::constant::mongoId] = timestampFromId;			
		if(current.hasField(stormy::acquisition::constant::reasonKey.c_str())) {
			measurement -> data[stormy::acquisition::constant::reasonKey] =
				string(current.getStringField(stormy::acquisition::constant::reasonKey.c_str()));
		}
		if(NumberParser::parseUnsigned64(timestampFromId) > NumberParser::parseUnsigned64(timestamp))								
			result.push_back(measurement);		
	}
	return result;
}

TypePtrVector MongoDBHandler::getTypesData()
{
	TypePtrVector result;
	if(!connected_) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(stormy::acquisition::util::GetTypeDb(), BSONObj());
	while( cursor -> more() ) {    
		BSONObj current = cursor -> next();
		TypePtr type(new Type());
		type -> id = current.getStringField(stormy::acquisition::constant::id.c_str());
		type -> equivalents.push_back(current.getStringField(stormy::acquisition::constant::equivalents.c_str()));
		type -> valueType = current.getStringField(stormy::acquisition::constant::type.c_str());
		type -> valueUnit = current.getStringField(stormy::acquisition::constant::unit.c_str());
		type -> valueFormat = current.getStringField(stormy::acquisition::constant::format.c_str());
		type -> isMeteo = current.getBoolField(stormy::acquisition::constant::isMeteo.c_str());		
		result.push_back(type);
	}
	return result;
}

bool MongoDBHandler::insertTypesData( const TypePtrVector& data )
{
	if(!connected_ || data.empty()) return false;
	stormy::common::Each(data, [&](TypePtr type) {		
		BSONObjBuilder bsonBuilder;
		bsonBuilder.append(stormy::acquisition::constant::id, type -> id);
		bsonBuilder.append(stormy::acquisition::constant::equivalents, type -> equivalents[0]);
		bsonBuilder.append(stormy::acquisition::constant::type, type -> valueType);
		bsonBuilder.append(stormy::acquisition::constant::unit, type -> valueUnit);
		bsonBuilder.append(stormy::acquisition::constant::format, type -> valueFormat);
		bsonBuilder.append(stormy::acquisition::constant::isMeteo, type -> isMeteo);
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
