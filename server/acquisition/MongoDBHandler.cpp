#include "MongoDBHandler.h"

#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
#include <Poco/Timestamp.h>
#include <Poco/NumberParser.h>

#include "TypeConfiguration.h"
#include "MeteoUtils.h"
#include "MeteoConst.h"
#include "../../common/Utils.h"

using namespace Stormy;
using namespace Meteo;
using namespace Poco;
using namespace std;

MongoDBHandler::MongoDBHandler( string dbAddress /*= "localhost"*/ )
	:	connected(false),
		connection(mongo::DBClientConnection())
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
		connected = true;
	} catch (const mongo::DBException& e) {
		cout << "[ERROR]: " << e.what() << endl;
		connected = false;
	}
}

void Stormy::MongoDBHandler::clearMeteosData()
{
	if(!connected) return;
	connection.dropCollection(MeteoUtils::getMeteoDb());
}

void MongoDBHandler::insertMeteoData( Measurement* measurement )
{
	if(!connected || !measurement) return;

	mongo::BSONObjBuilder bsonBuilder;
	auto data = measurement -> data;
	for(auto it = data.begin(); it != data.end(); ++it)
	{
		string key = it -> first;
		string value = Utils::getStringFromAny(it -> second);
		bsonBuilder.append(key, value);
	}
	string stationId = measurement -> station -> stationId;
	connection.insert(MeteoUtils::getMeteoDb() + "." + Const::stationIdPrefix
		+ stationId, bsonBuilder.obj());
}

void MongoDBHandler::clearStationsData()
{
	if(!connected) return;
	connection.dropCollection(MeteoUtils::getStatioDb());
}

void MongoDBHandler::insertStationsData( vector<Station*>& data )
{
	if(!connected) return;
	for(auto it = data.begin(); it != data.end(); ++it)
		insertStationData(*it);
}

void MongoDBHandler::insertStationData( Station* data )
{
	if(!connected || !data) return;
	mongo::BSONObjBuilder bsonBuilder;
	bsonBuilder.append(Const::mongoId, data -> stationId);
	bsonBuilder.append(Const::name, data -> name);
	bsonBuilder.append(Const::parserClass, data -> parserClass);
	bsonBuilder.append(Const::refreshTime, data -> refreshTime);
	bsonBuilder.append(Const::url, data -> url);
	connection.insert(MeteoUtils::getStatioDb(), bsonBuilder.obj());
}

vector<Station*> MongoDBHandler::getStationsData()
{
	auto result = vector<Station*>();
	if(!connected) return result;

	auto_ptr<mongo::DBClientCursor> cursor =
		connection.query(MeteoUtils::getStatioDb(), mongo::BSONObj());
	while( cursor -> more() ) {
		mongo::BSONObj current = cursor -> next();
		Station* station = new Station();
		station -> stationId = current.getStringField(Const::mongoId.c_str());
		station -> name = current.getStringField(Const::name.c_str());
		station -> parserClass = current.getStringField(Const::parserClass.c_str());
		station -> refreshTime = current.getIntField(Const::refreshTime.c_str());
		station -> url = current.getStringField(Const::url.c_str());
		result.push_back(station);
	}
	return result;
}

Measurement* MongoDBHandler::getCurrentMeteoTypeData( string stationId, string typeId )
{
	auto result = new Measurement();
	if(!connected) return result;

	auto_ptr<mongo::DBClientCursor> cursor =
		connection.query(MeteoUtils::getMeteoDb() + "." +
			Const::stationIdPrefix + stationId, mongo::Query().sort("_id", 0), 1);
	if(cursor -> more()) {
		mongo::BSONObj current = cursor -> next();
		result -> timestamp = Timestamp::fromEpochTime(
			lexical_cast<long>(current.getStringField(Const::mongoId.c_str())));
		if(current.hasField(typeId)) {
			result -> data[typeId] = string(current.getStringField(typeId.c_str()));
		}
	}
	return result;
}

vector<Measurement*> MongoDBHandler::getCurrentMeteoTypeDatas( string stationId, string typeId )
{
	auto result = vector<Measurement*>();
	if(!connected) return result;

	auto_ptr<mongo::DBClientCursor> cursor =
		connection.query(MeteoUtils::getMeteoDb() + "." +
			Const::stationIdPrefix + stationId, mongo::BSONObj());
	while(cursor -> more()) {
		mongo::BSONObj current = cursor -> next();
		Measurement* measurement = new Measurement();
		measurement -> timestamp = Timestamp::fromEpochTime(
			lexical_cast<long>(current.getStringField(Const::mongoId.c_str())));
		if(current.hasField(typeId)) {
			measurement -> data[typeId] =
				string(current.getStringField(typeId.c_str()));
			result.push_back(measurement);
		}
	}
	return result;
}

vector<Measurement*> MongoDBHandler::getMeteoData(string stationId)
{
	auto result = vector<Measurement*>();
	if(!connected) return result;

	TypeConfiguration* typesCfg =
		new TypeConfiguration("config/meteo_data_type_config.yaml");
	vector<Type*> types = typesCfg -> getConfiguration();
	auto_ptr<mongo::DBClientCursor> cursor =
		connection.query(MeteoUtils::getMeteoDb() + "." +
			Const::stationIdPrefix + stationId, mongo::BSONObj());
	while( cursor -> more() ) {
		mongo::BSONObj current = cursor -> next();
		set<string> availableFields;
		current.getFieldNames(availableFields);

		Measurement* measurement = new Measurement();
		for(auto it = types.begin(); it != types.end(); ++it) {
			string id = (*it) -> id;
			if(availableFields.find(id) != availableFields.end()) {
				Type* type = typesCfg -> getFullTypeById(id);
				string value = current.getStringField(id.c_str());
				if(type -> valueType == Const::number)
					measurement -> data[id]= MeteoUtils::extractTemperature(value);
				else if(type ->valueType == Const::text)
					measurement -> data[id]= value;
			}
		}
		measurement -> data[Const::mongoId] =
			string(current.getStringField(Const::mongoId.c_str()));
		if(current.hasField(Const::reasonKey.c_str())) {
			measurement -> data[Const::reasonKey] =
				string(current.getStringField(Const::reasonKey.c_str()));
		}
		result.push_back(measurement);
	}
	return result;
}

vector<Meteo::Measurement*> Stormy::MongoDBHandler::getMeteoDataNewerThan( string stationId, string timestamp )
{
	auto result = vector<Measurement*>();
	if(!connected) return result;

	TypeConfiguration* typesCfg =
		new TypeConfiguration("config/meteo_data_type_config.yaml");
	vector<Type*> types = typesCfg -> getConfiguration();
	auto_ptr<mongo::DBClientCursor> cursor =
		connection.query(MeteoUtils::getMeteoDb() + "." +
		Const::stationIdPrefix + stationId, mongo::BSONObj());
	while( cursor -> more() ) {
		mongo::BSONObj current = cursor -> next();
		set<string> availableFields;
		current.getFieldNames(availableFields);

		Measurement* measurement = new Measurement();
		for(auto it = types.begin(); it != types.end(); ++it) {
			string id = (*it) -> id;
			if(availableFields.find(id) != availableFields.end()) {
				Type* type = typesCfg -> getFullTypeById(id);
				string value = current.getStringField(id.c_str());
				if(type -> valueType == Const::number)
					measurement -> data[id]= MeteoUtils::extractTemperature(value);
				else if(type ->valueType == Const::text)
					measurement -> data[id]= value;
			}
		}
		string timestampFromId = current.getStringField(Const::mongoId.c_str());
		measurement -> data[Const::mongoId] = timestampFromId;			
		if(current.hasField(Const::reasonKey.c_str())) {
			measurement -> data[Const::reasonKey] =
				string(current.getStringField(Const::reasonKey.c_str()));
		}
		if(NumberParser::parseUnsigned64(timestampFromId) > NumberParser::parseUnsigned64(timestamp))								
			result.push_back(measurement);		
	}
	return result;
}

