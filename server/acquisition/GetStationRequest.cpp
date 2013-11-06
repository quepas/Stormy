#include "GetStationRequest.h"

#include "MongoDBHandler.h"
#include "Utils.h"
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
	std::vector<Station*> stations = MongoDBHandler::get().getStationsData();
	ostr << JSONUtils::prepareJSONForStations(stations);
}

std::string Stormy::GetStationRequest::prepareStationHTML( Station* station )
{
	std::string header = "<h2>" + station -> name + "</h2>";
	std::string content =
		"<ul><li>Id: " + station -> stationId + "</li>" +
		"<li>Name: " + station -> name + "</li>" +
		"<li>Refresh time: " +
		lexical_cast<std::string>(station -> refreshTime) +
		"</li>" + "<li>URL: " + station -> url + "</li></ul>";
	return header + content;
}
