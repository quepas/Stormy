#include "GetMeteoRequest.h"

#include "MongoDBHandler.h"
#include "../../common/Utils.h"
#include "JSONUtils.h"
#include "TypeConfiguration.h"
#include "RESTConst.h"

#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

using namespace Stormy;
using namespace Meteo;
using REST::Const;
using namespace std;

GetMeteoRequest::GetMeteoRequest(string _stationId, 
	string _typeId /* ="" */, string _timestamp /* = ""*/)
	:	stationId(_stationId), typeId(_typeId), timestamp(_timestamp)
{

}

GetMeteoRequest::~GetMeteoRequest()
{

}

void GetMeteoRequest::handleRequest( HTTPServerRequest& request, HTTPServerResponse& response )
{
	ostream& ostr = response.send();

	MongoDBHandler& dbHandler = MongoDBHandler::get();	
	if(stationId != REST::Const::none) {
		if(typeId.empty()) {
			if(timestamp.empty()) {				
				MeasurementPtrVector meteo = dbHandler.getMeteoData(stationId);
				ostr << JSONUtils::prepareJSONForMeasurements(meteo);
			} else {				
				MeasurementPtrVector meteo = dbHandler.getMeteoDataNewerThan(stationId, timestamp);
				ostr << JSONUtils::prepareJSONForMeasurements(meteo);
			}			
		} else {
			MeasurementPtrVector singleMeteo = dbHandler.getCurrentMeteoTypeDatas(stationId, typeId);
			ostr << JSONUtils::prepareJSONForSingleMeasurements(singleMeteo);
		}
	} else {
		ostr << REST::Const::emptyJSON;
	}
}
