#include "StorageDatabaseConfig.h"

using namespace Stormy;
using namespace std;

StorageDatabaseConfig::StorageDatabaseConfig( string path )
	:	YAMLConfig(path),
		configuration(new StorageDatabase())
{
	mapIntoConfiguration();
}

StorageDatabaseConfig::~StorageDatabaseConfig()
{

}

void StorageDatabaseConfig::mapIntoConfiguration()
{
	configuration = new StorageDatabase();
	if(hasField("name")) {
		configuration -> name = asString("name");
	}
	if(hasField("host")) {
		configuration -> host = asString("host");
	} else {
		std::string defaultHost = "localhost";
		cout << "[StorageDB] No host data. Using default (" 
			 << defaultHost << ")." << endl;		
	}
	if(hasField("port")) {
		configuration -> port = asInt("port");
	} else {
		cout << "[StorageDB] No port data. Using default (" 
			 << StorageDatabase::defaultPort << ")." << endl;
		configuration -> port = StorageDatabase::defaultPort;
	}
	if(hasField("database")) {
		configuration -> database = asString("database");
	} else {		
		cout << "[StorageDB] No database name." << endl;
	}
	if(hasField("user")) {
		configuration -> user = asString("user");
	} else {
		cout << "[StorageDB] No user name." << endl;
	}
	if(hasField("password")) {
		configuration -> password  = asString("password");
	} else {
		cout << "[StorageDB] No password." << endl;
	}
}
