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
	if(!connected) return;
	mongo::BSONObjBuilder bsonBuilder;
	bsonBuilder.append("name", "hihihi");
	bsonBuilder.append("surname", "hjehje");
	bsonBuilder.append("age", 33);
	mongo::BSONObj bsonObject = bsonBuilder.obj();	
	std::string dbColName = createDbCollectionName();
	if(dbColName != "_none")
		connection.insert(dbColName, bsonObject);
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
