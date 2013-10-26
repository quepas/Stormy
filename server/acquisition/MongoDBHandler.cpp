#include "MongoDBHandler.h"

#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>

using namespace Stormy;
using Meteo::Station;
using Meteo::Measurement;

MongoDBHandler::MongoDBHandler( std::string dbAddress /*= ""*/ )
	:	connected(false),
		currentDB(""),
		currentCollection(""),
		connection(mongo::DBClientConnection())
{
	if(!dbAddress.empty())	
		connect(dbAddress);
}

MongoDBHandler::~MongoDBHandler()
{
	
}

void MongoDBHandler::connect( std::string dbAddress )
{
	try {		
		connection.connect(dbAddress);
		std::cout << "[INFO]: Connected to databases on address: " << dbAddress << std::endl;
		connected = true;
	} catch (const mongo::DBException& e) {
		std::cout << "[ERROR]: " << e.what() << std::endl;
		connected = false;
	}
}


void Stormy::MongoDBHandler::clearMeteosData()
{
	if(!connected) return;
	connection.dropCollection("test.meteo");		
}

void MongoDBHandler::insertMeteoData( Measurement* measurement )
{
	if(!connected || !measurement) return;	

	mongo::BSONObjBuilder bsonBuilder;
	auto data = measurement -> data;	
	for(auto it = data.begin(); it != data.end(); ++it) 
	{		
		std::string key = it -> first;
		std::string value;
		boost::any anyValue = it -> second;
		
		if(anyValue.type() == typeid(double))
			value = boost::lexical_cast<std::string>(
				boost::any_cast<double>(it -> second));		
		else
			value = boost::any_cast<std::string>(it ->second);
		bsonBuilder.append(key, value);						
	}	
	connection.insert("test.meteo", bsonBuilder.obj());
}

void MongoDBHandler::setDbAndCollection( std::string dbName, std::string collectionName )
{
	currentDB = dbName;
	currentCollection = collectionName;
}

std::string MongoDBHandler::createDbCollectionName()
{
	if(currentDB.empty() || currentCollection.empty())
		return "_none";
	return currentDB + "." + currentCollection;
}

void MongoDBHandler::clearStationsData()
{
	if(!connected) return;
	connection.dropCollection("test.stations");		
}

void MongoDBHandler::insertStationsData( std::vector<Station*>& data )
{
	if(!connected) return;
	for(auto it = data.begin(); it != data.end(); ++it)
		insertStationData(*it);
}

void MongoDBHandler::insertStationData( Station* data )
{
	if(!connected || !data) return;	

	mongo::BSONObjBuilder bsonBuilder;
	bsonBuilder.append("_id", data -> id);
	bsonBuilder.append("name", data -> name);
	bsonBuilder.append("parserClass", data -> parserClass);
	bsonBuilder.append("refreshTime", data -> refreshTime);
	bsonBuilder.append("url", data -> url);
	connection.insert("test.stations", bsonBuilder.obj());
}

std::vector<Station*> MongoDBHandler::getStationsData()
{
	auto result = std::vector<Station*>();
	if(!connected) return result;	

	std::auto_ptr<mongo::DBClientCursor> cursor = 
		connection.query("test.stations", mongo::BSONObj());
	while( cursor -> more() ) {
		mongo::BSONObj current = cursor -> next();
		Station* station = new Station();
		station -> id = current.getStringField("_id");
		station -> name = current.getStringField("name");
		station -> parserClass = current.getStringField("parserClass");
		station -> refreshTime = current.getIntField("refreshTime");
		station -> url = current.getStringField("url");
		result.push_back(station);
	}
	return result;
}

std::vector<Measurement*> MongoDBHandler::getMeteoData()
{
	auto result = std::vector<Measurement*>();
	if(!connected) return result;
	
	std::auto_ptr<mongo::DBClientCursor> cursor =
		connection.query("test.meteo", mongo::BSONObj());
	/*while( cursor -> more() ) {
		mongo::BSONObj current = cursor -> next();
		MeteoData* meteoData = new MeteoData();
		// for tests
		SingleMeteoData* airTemp = new SingleMeteoData();
		airTemp->text = new std::string(current.getStringField("AIR_TEMPERATURE"));
		meteoData -> data.insert(std::make_pair(AIR_TEMPERATURE, airTemp));
		SingleMeteoData* airHum = new SingleMeteoData();
		airHum -> text = new std::string(current.getStringField("AIR_HUMIDITY"));
		meteoData -> data.insert(std::make_pair(AIR_HUMIDITY, airHum));
		result.push_back(meteoData);
	}*/
	return result;
}
