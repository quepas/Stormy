#include "GetMetricsRequest.h"

#include "TypeConfiguration.h"
#include "JSONUtils.h"
#include "rest_constant.h"

using namespace Stormy;
using namespace Stormy::REST;
using namespace Stormy::Meteo;
using namespace std;

GetMetricsRequest::GetMetricsRequest( string _stationId /*= ""*/ )
	:	stationId(_stationId)
{

}

GetMetricsRequest::~GetMetricsRequest()
{

}

void GetMetricsRequest::handleRequest( HTTPServerRequest& request, 
	HTTPServerResponse& response )
{
	ostream& ostr = response.send();
	TypeConfiguration* typesCfg =
		new TypeConfiguration("config/meteo_data_type_config.yaml");

	if(stationId.empty()) {
		ostr << JSONUtils::prepareJSONForAvailableTypes(
			typesCfg->getConfiguration());
	} else {
		ostr << stormy::rest::constant::emptyJSON;
	}
	
}



