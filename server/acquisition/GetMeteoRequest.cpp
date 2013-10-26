#include "GetMeteoRequest.h"

#include "MongoDBHandler.h"
#include "Utils.h"
#include <boost/any.hpp>

using namespace Stormy;
using namespace Meteo;

GetMeteoRequest::GetMeteoRequest()
{

}

GetMeteoRequest::~GetMeteoRequest()
{

}

void GetMeteoRequest::handleRequest( HTTPServerRequest& request, HTTPServerResponse& response )
{
	std::ostream& ostr = response.send();

	MongoDBHandler dbHandler("localhost");
	std::vector<Measurement*> meteo = dbHandler.getMeteoData();
	std::string content;
	for(auto it = meteo.begin(); it != meteo.end(); ++it) {
		content += prepareMeteoHTML(*it);
	}

	ostr << "<html><head><title>Meteo</title></head><body><h1>Meteo</h1>"
		 << content << "</body></html>";
}

std::string GetMeteoRequest::prepareMeteoHTML(Measurement* meteo)
{
	std::string header = "<h2>Meteo station</h2>";
	std::string content = 
		"<ul><li>Air temperature: " + boost::any_cast<std::string>(meteo -> data["airTemperature"]) + "</li>" +
		"<li>Air humidity: " + boost::any_cast<std::string>(meteo -> data["airHumidity"]) + "</li></ul>";
	return header + content;
}


