#include "acquisition_config.h"

#include "util.h"

using std::string;

namespace stormy {
  namespace acquisition {

Config::Config( string path )
	:	BaseConfig(path)		
{
	MapIntoConfiguration();
}

Config::~Config()
{
  for (auto& it = configuration_.begin(); 
       it != configuration_.end(); ++it) {
    delete *it;
  }
}

void Config::MapIntoConfiguration()
{
	for(unsigned int i = 0; i < Size(); ++i) {
		Setting* serverCfg = new Setting();
		
		if(HasField("name", i)) {			
			serverCfg -> name = AsString("name", i);
		}
		if(HasField("host", i)) {
			serverCfg -> host = AsString("host", i);
		} else {
			std::cout << "No host data in " << i + 1 
				<< "th configuration." << std::endl;
			continue;
		}
		if(HasField("port", i)) {
			serverCfg -> port = AsInt("port", i);
		} else {
			std::cout << "No port data in " << i + 1
				<< "th configuration." << std::endl;
			continue;
		}
		if(HasField("acquisitionInterval", i)) {
			serverCfg -> interval = AsInt("acquisitionInterval", i);
		} else {
			serverCfg -> interval = 30; // seconds
		}
		std::string strHost = serverCfg -> host;
		std::string strPort = AsString("port", i);
		serverCfg -> id = common::MD5(strHost + ":" + strPort);
		configuration_.push_back(serverCfg);
	}
}
// ~~ stormy::acquisition::Config
}}
