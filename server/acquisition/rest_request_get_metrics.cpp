#include "rest_request_get_metrics.h"

#include "TypeConfiguration.h"
#include "rest_json_cookbook.h"
#include "rest_constant.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using std::string;
using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetMetrics::GetMetrics(string station_uid /*= ""*/)
	:	station_uid_(station_uid)
{

}

GetMetrics::~GetMetrics()
{

}

void GetMetrics::handleRequest(
  HTTPServerRequest& request, 
	HTTPServerResponse& response)
{
	ostream& ostr = response.send();
	auto typesCfg =
		new Stormy::Meteo::TypeConfiguration("config/meteo_data_type_config.yaml");

	if(station_uid_.empty()) {
		ostr << json::cookbook::PrepareMetrics(
			typesCfg->getConfiguration());
	} else {
		ostr << stormy::rest::constant::emptyJSON;
	}
	
}
// ~~ stormy::rest::request::GetMetrics
}}}
