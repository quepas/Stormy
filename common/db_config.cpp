#include "db_config.h"

#include <Poco/NumberFormatter.h>

using std::string;
using Poco::NumberFormatter;

namespace stormy {
  namespace common {
    namespace db {

Config::Config(string path)
	:	BaseConfig(path),
		configuration_()
{
	MapIntoConfiguration();
}

Config::~Config()
{

}

void Config::MapIntoConfiguration()
{
	if(HasField("name")) {
		configuration_.name = AsString("name");
	}
	if(HasField("host")) {
		configuration_.host = AsString("host");
	} else {
		string defaultHost = "localhost";
		logger_.warning("[db/Config] No host data. Using default (" 
			 + defaultHost + ").");	
    configuration_.host = defaultHost;
	}
	if(HasField("port")) {
		configuration_.port = AsInt("port");
	} else {
		logger_.warning("[db/Config] No port data. Using default (" 
			 + NumberFormatter::format(Setting::defaultPort) + ").");
		configuration_.port = Setting::defaultPort;
	}
	if(HasField("database")) {
		configuration_.database = AsString("database");
	} else {		
		logger_.error("[db/Config] No database name.");
	}
	if(HasField("user")) {
		configuration_.user = AsString("user");
	} else {
		logger_.error("[db/Config] No user name.");
	}
	if(HasField("password")) {
		configuration_.password  = AsString("password");
	} else {
		logger_.error("[db/Config] No password.");
	}
}
// ~~ stormy::common::db::Config
}}}
