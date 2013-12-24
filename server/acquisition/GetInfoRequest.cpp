#include "GetInfoRequest.h"

#include "TypeConfiguration.h"
#include "rest_constant.h"
#include "JSONUtils.h"

using namespace Stormy;
using namespace Meteo;
using Meteo::TypeConfiguration;

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
		ostr << JSONUtils::prepareJSONForInfo("A", typesCfg -> getConfiguration());
	} else {
		ostr << stormy::rest::constant::emptyJSON;
	}
}
