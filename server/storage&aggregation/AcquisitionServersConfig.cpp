#include "AcquisitionServersConfig.h"

#include "../../common/Utils.h"

using namespace Stormy;

AcquisitionServersConfig::AcquisitionServersConfig( std::string path )
	:	Config(path),
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
			serverCfg -> acquisitionInterval = AsInt("acquisitionInterval", i);
		} else {
			serverCfg -> acquisitionInterval = 30; // seconds
		}
		std::string strHost = serverCfg -> host;
		std::string strPort = AsString("port", i);
		serverCfg -> id = Utils::md5(strHost + ":" + strPort);
		configuration.push_back(serverCfg);
	}
}
