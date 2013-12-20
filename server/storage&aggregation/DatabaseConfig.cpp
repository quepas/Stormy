#include "DatabaseConfig.h"

using namespace Stormy;
using namespace std;

DatabaseConfig::DatabaseConfig( string path )
	:	Config(path),
		configuration(new Database())
{
	mapIntoConfiguration();
}

DatabaseConfig::~DatabaseConfig()
{

}

void DatabaseConfig::mapIntoConfiguration()
{
	configuration = new Database();
	if(HasField("name")) {
		configuration -> name = AsString("name");
	}
	if(HasField("host")) {
		configuration -> host = AsString("host");
	} else {
		std::string defaultHost = "localhost";
		cout << "[StorageDB] No host data. Using default (" 
			 << defaultHost << ")." << endl;	
    configuration -> host = defaultHost;
	}
	if(HasField("port")) {
		configuration -> port = AsInt("port");
	} else {
		cout << "[StorageDB] No port data. Using default (" 
			 << Database::defaultPort << ")." << endl;
		configuration -> port = Database::defaultPort;
	}
	if(HasField("database")) {
		configuration -> database = AsString("database");
	} else {		
		cout << "[StorageDB] No database name." << endl;
	}
	if(HasField("user")) {
		configuration -> user = AsString("user");
	} else {
		cout << "[StorageDB] No user name." << endl;
	}
	if(HasField("password")) {
		configuration -> password  = AsString("password");
	} else {
		cout << "[StorageDB] No password." << endl;
	}
}
