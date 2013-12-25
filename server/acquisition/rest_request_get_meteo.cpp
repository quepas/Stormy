#include "rest_request_get_meteo.h"

#include "MongoDBHandler.h"
#include "../../common/util.h"
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

GetMeteo::GetMeteo(
  string station_uid, 
	string type_id /* ="" */, 
  string timestamp /* = ""*/)
	:	station_uid_(station_uid), type_id_(type_id), timestamp_(timestamp)
{

}

GetMeteo::~GetMeteo()
{

}

void GetMeteo::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response)
{
	ostream& ostr = response.send();

	Stormy::MongoDBHandler& dbHandler = Stormy::MongoDBHandler::get();	
	if(station_uid_ != constant::none) {
		if(type_id_.empty()) {
			if(timestamp_.empty()) {				
				Stormy::MeasurementPtrVector meteo = dbHandler.getMeteoData(station_uid_);
				ostr << json::cookbook::PrepareCombinedMeasurements(meteo);
			} else {				
				Stormy::MeasurementPtrVector meteo = dbHandler.getMeteoDataNewerThan(station_uid_, timestamp_);
				ostr << json::cookbook::PrepareCombinedMeasurements(meteo);
			}			
		} else {
			Stormy::MeasurementPtrVector singleMeteo = dbHandler.getCurrentMeteoTypeDatas(station_uid_, type_id_);
			ostr << json::cookbook::PrepareMeasurements(singleMeteo);
		}
	} else {
		ostr << constant::emptyJSON;
	}
}
// ~~ stormy::rest::request::GetMeteo
}}}
