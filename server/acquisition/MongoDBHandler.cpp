#include "MongoDBHandler.h"

using namespace Stormy;

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

void MongoDBHandler::insertMeteoData( MeteoData* meteoData )
{
	
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

void Stormy::MongoDBHandler::clearStationsData()
{
	if(!connected) return;
	connection.dropCollection("test.stations");		
}

void Stormy::MongoDBHandler::insertStationsData( std::vector<MeteoStation*>& data )
{
	if(!connected) return;
	for(auto it = data.begin(); it != data.end(); ++it)
		insertStationData(*it);
}

void Stormy::MongoDBHandler::insertStationData( MeteoStation* data )
{
	if(!connected && !data) return;	

	mongo::BSONObjBuilder bsonBuilder;
	bsonBuilder.append("_id", data -> id);
	bsonBuilder.append("name", data -> name);
	bsonBuilder.append("parserClass", data -> parserClass);
	bsonBuilder.append("refreshTime", data -> refreshTime);
	bsonBuilder.append("url", data -> url);
	connection.insert("test.stations", bsonBuilder.obj());
}

std::vector<MeteoStation*> Stormy::MongoDBHandler::getStationsData()
{
	auto result = std::vector<MeteoStation*>();
	if(!connected) return result;	

	std::auto_ptr<mongo::DBClientCursor> cursor = 
		connection.query("test.stations", mongo::BSONObj());
	while( cursor -> more() ) {
		mongo::BSONObj current = cursor -> next();
		MeteoStation* station = new MeteoStation();
		station -> id = current.getStringField("_id");
		station -> name = current.getStringField("name");
		station -> parserClass = current.getStringField("parserClass");
		station -> refreshTime = current.getIntField("refreshTime");
		station -> url = current.getStringField("url");
		result.push_back(station);
	}
	return result;
}
