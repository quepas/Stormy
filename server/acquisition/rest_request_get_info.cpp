#include "rest_request_get_info.h"

#include "TypeConfiguration.h"
#include "rest_constant.h"
#include "rest_json_cookbook.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using std::string;
using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetInfo::GetInfo(string _stationId /*= ""*/)
	:	station_uid_(_stationId)
{

}

GetInfo::~GetInfo()
{

}

void GetInfo::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response)
{
	ostream& ostr = response.send();

	if(station_uid_.empty()) {
		auto typesCfg =
			new Stormy::Meteo::TypeConfiguration("config/meteo_data_type_config.yaml");
		ostr << json::cookbook::PrepareInfo("A", typesCfg -> getConfiguration());
	} else {
		ostr << constant::emptyJSON;
	}
}
// ~~ stormy::rest::request::GetInfo
}}}
