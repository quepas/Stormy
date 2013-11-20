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
using namespace mongo;

MongoDBHandler::MongoDBHandler( string dbAddress /*= "localhost"*/ )
	:	connected(false),
		connection(DBClientConnection())
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
	} catch (const DBException& e) {
		cout << "[ERROR]: " << e.what() << endl;
		connected = false;
	}
}

void MongoDBHandler::clearMeteosData()
{
	if(!connected) return;
	connection.dropCollection(MeteoUtils::getMeteoDb());
}

void MongoDBHandler::insertMeteoData( MeasurementPtr measurement )
{
	if(!connected || !measurement) return;

	BSONObjBuilder bsonBuilder;
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

void MongoDBHandler::insertStationsData( const StationPtrVector& data )
{
	if(!connected) return;
	for(auto it = data.begin(); it != data.end(); ++it)
		insertStationData(*it);
}

void MongoDBHandler::insertStationData( StationPtr data )
{
	if(!connected || !data) return;
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(Const::mongoId, data -> stationId);
	bsonBuilder.append(Const::name, data -> name);
	bsonBuilder.append(Const::parserClass, data -> parserClass);
	bsonBuilder.append(Const::refreshTime, data -> refreshTime);
	bsonBuilder.append(Const::url, data -> url);
	connection.insert(MeteoUtils::getStatioDb(), bsonBuilder.obj());
}

StationPtrVector MongoDBHandler::getStationsData()
{
	auto result = StationPtrVector();
	if(!connected) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(MeteoUtils::getStatioDb(), BSONObj());
	while( cursor -> more() ) {
		BSONObj current = cursor -> next();
		StationPtr station(new Station());
		station -> stationId = current.getStringField(Const::mongoId.c_str());
		station -> name = current.getStringField(Const::name.c_str());
		station -> parserClass = current.getStringField(Const::parserClass.c_str());
		station -> refreshTime = current.getIntField(Const::refreshTime.c_str());
		station -> url = current.getStringField(Const::url.c_str());
		result.push_back(station);
	}
	return result;
}

MeasurementPtr MongoDBHandler::getCurrentMeteoTypeData( string stationId, string typeId )
{
	auto result = MeasurementPtr(new Measurement());
	if(!connected) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(MeteoUtils::getMeteoDb() + "." +
			Const::stationIdPrefix + stationId, Query().sort("_id", 0), 1);
	if(cursor -> more()) {
		BSONObj current = cursor -> next();
		result -> timestamp = Timestamp::fromEpochTime(
			lexical_cast<long>(current.getStringField(Const::mongoId.c_str())));
		if(current.hasField(typeId)) {
			result -> data[typeId] = string(current.getStringField(typeId.c_str()));
		}
	}
	return result;
}

MeasurementPtrVector MongoDBHandler::getCurrentMeteoTypeDatas( string stationId, string typeId )
{
	auto result = MeasurementPtrVector();
	if(!connected) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(MeteoUtils::getMeteoDb() + "." +
			Const::stationIdPrefix + stationId, BSONObj());
	while(cursor -> more()) {
		BSONObj current = cursor -> next();
		MeasurementPtr measurement(new Measurement());
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

MeasurementPtrVector MongoDBHandler::getMeteoData(string stationId)
{
	auto result = MeasurementPtrVector();
	if(!connected) return result;
	
	TypePtrVector types = getTypesData();
	auto_ptr<DBClientCursor> cursor =
		connection.query(MeteoUtils::getMeteoDb() + "." +
			Const::stationIdPrefix + stationId, BSONObj());
	while( cursor -> more() ) {
		BSONObj current = cursor -> next();
		set<string> availableFields;
		current.getFieldNames(availableFields);

		MeasurementPtr measurement(new Measurement());
		for(auto it = types.begin(); it != types.end(); ++it) {
			string id = (*it) -> id;
			if(availableFields.find(id) != availableFields.end()) {
				TypePtr type = TypeConfiguration::getTypeById(id, types);
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

MeasurementPtrVector MongoDBHandler::getMeteoDataNewerThan( string stationId, string timestamp )
{
	auto result = MeasurementPtrVector();
	if(!connected) return result;
	
	TypePtrVector types = getTypesData();
	auto_ptr<DBClientCursor> cursor =
		connection.query(MeteoUtils::getMeteoDb() + "." +
		Const::stationIdPrefix + stationId, BSONObj());
	while( cursor -> more() ) {
		BSONObj current = cursor -> next();
		set<string> availableFields;
		current.getFieldNames(availableFields);

		MeasurementPtr measurement(new Measurement());
		for(auto it = types.begin(); it != types.end(); ++it) {
			string id = (*it) -> id;
			if(availableFields.find(id) != availableFields.end()) {
				TypePtr type = TypeConfiguration::getTypeById(id, types);
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

TypePtrVector MongoDBHandler::getTypesData()
{
	TypePtrVector result;
	if(!connected) return result;

	auto_ptr<DBClientCursor> cursor =
		connection.query(MeteoUtils::getTypeDb(), BSONObj());
	while( cursor -> more() ) {
		BSONObj current = cursor -> next();
		TypePtr type(new Type());
		type -> id = current.getStringField(Const::id.c_str());
		type -> equivalents.push_back(current.getStringField(Const::equivalents.c_str()));
		type -> valueType = current.getStringField(Const::type.c_str());
		type -> valueUnit = current.getStringField(Const::unit.c_str());
		type -> valueFormat = current.getStringField(Const::format.c_str());
		type -> isMeteo = current.getBoolField(Const::isMeteo.c_str());		
		result.push_back(type);
	}
	return result;
}

bool MongoDBHandler::insertTypesData( const TypePtrVector& data )
{
	if(!connected || data.empty()) return false;
	Utils::forEach(data, [&](TypePtr type) {		
		BSONObjBuilder bsonBuilder;
		bsonBuilder.append(Const::id, type -> id);
		bsonBuilder.append(Const::equivalents, type -> equivalents[0]);
		bsonBuilder.append(Const::type, type -> valueType);
		bsonBuilder.append(Const::unit, type -> valueUnit);
		bsonBuilder.append(Const::format, type -> valueFormat);
		bsonBuilder.append(Const::isMeteo, type -> isMeteo);
		connection.insert(MeteoUtils::getTypeDb(), bsonBuilder.obj());
	});
	return true;
}

bool MongoDBHandler::clearTypesData()
{
	if(!connected) return false;
	connection.dropCollection(MeteoUtils::getTypeDb());
	return true;
}
