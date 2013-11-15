#include "AcquisitionServersConfig.h"

#include "../../common/Utils.h"

using namespace Stormy;

AcquisitionServersConfig::AcquisitionServersConfig( std::string path )
	:	YAMLConfig(path),
		configuration()
{
	mapIntoConfiguration();
}

AcquisitionServersConfig::~AcquisitionServersConfig()
{

}

void AcquisitionServersConfig::mapIntoConfiguration()
{
	for(unsigned int i = 0; i < Size(); ++i) {
		AcquisitionServer* serverCfg = new AcquisitionServer();
		
		if(hasField("name", i)) {			
			serverCfg -> name = asString("name", i);
		}
		if(hasField("host", i)) {
			serverCfg -> host = asString("host", i);
		} else {
			std::cout << "No host data in " << i + 1 
				<< "th configuration." << std::endl;
			continue;
		}
		if(hasField("port", i)) {
			serverCfg -> port = asInt("port", i);
		} else {
			std::cout << "No port data in " << i + 1
				<< "th configuration." << std::endl;
			continue;
		}
		std::string strHost = serverCfg -> host;
		std::string strPort = asString("port", i);
		serverCfg -> id = Utils::md5(strHost + ":" + strPort);
		configuration.push_back(serverCfg);
	}
}
