#include "MongoDBHandler.h"
#include <mongo/client/dbclient.h>

using namespace Stormy;

MongoDBHandler::MongoDBHandler( std::string dbAddress /*= ""*/ )
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
		mongo::DBClientConnection connection;
		connection.connect(dbAddress);
		std::cout << "[INFO]: Connected to databases on address: " << dbAddress << std::endl;
		connected = true;
	} catch (const mongo::DBException& e) {
		std::cout << "[ERROR]: " << e.what() << std::endl;
		connected = false;
	}
}
