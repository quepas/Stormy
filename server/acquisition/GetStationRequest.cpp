#include "GetStationRequest.h"

#include "MongoDBHandler.h"
#include "Utils.h"

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

	MongoDBHandler dbHandler("localhost");
	std::vector<Station*> stations = dbHandler.getStationsData();
	std::string content;
	for(auto it = stations.begin(); it != stations.end(); ++it)
		content += prepareStationHTML(*it);	

	/*ostr << "<html><head><title>Stations</title></head><body><h1>Stations</h1>"
		 << content << "</body></html>";*/
	ostr << "{\"content\": [{\"name\": \"Patryk\"},{\"name\": \"Kiepas\"}]}\r\n";
}

std::string Stormy::GetStationRequest::prepareStationHTML( Station* station )
{
	std::string header = "<h2>" + station -> name + "</h2>";
	std::string content =
		"<ul><li>Id: " + station -> id + "</li>" +
		"<li>Name: " + station -> name + "</li>" +
		"<li>Refresh time: " + 
		lexical_cast<std::string>(station -> refreshTime) + 
		"</li>" + "<li>URL: " + station -> url + "</li></ul>";
	return header + content;
}
