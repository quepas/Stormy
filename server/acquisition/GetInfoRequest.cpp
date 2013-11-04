#include "GetInfoRequest.h"

using namespace Stormy;

Stormy::GetInfoRequest::GetInfoRequest( std::string _stationId /*= ""*/ )
	:	stationId(_stationId)
{

}

Stormy::GetInfoRequest::~GetInfoRequest()
{

}

void Stormy::GetInfoRequest::handleRequest( HTTPServerRequest& request, HTTPServerResponse& response )
{
	std::ostream& ostr = response.send();

	if(stationId.empty()) {
		TypeConfiguration* typesCfg = 
			new TypeConfiguration("config/meteo_data_type_config.yaml");


	}

	//ostr << data
}
