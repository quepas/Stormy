#include "GetStationRequest.h"

#include "MongoDBHandler.h"
#include "../../common/util.h"
#include "JSONUtils.h"

using namespace Stormy;
using Meteo::Station;

Stormy::GetStationRequest::GetStationRequest()
{

}

Stormy::GetStationRequest::~GetStationRequest()
{

}

void Stormy::GetStationRequest::handleRequest( HTTPServerRequest& request, HTTPServerResponse& response )
{
	std::ostream& ostr = response.send();
	StationPtrVector stations = MongoDBHandler::get().getStationsData();
	ostr << JSONUtils::prepareJSONForStations(stations);
}
