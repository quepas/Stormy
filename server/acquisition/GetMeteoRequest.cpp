#include "GetMeteoRequest.h"

#include "MongoDBHandler.h"
#include "Utils.h"

using namespace Stormy;

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
	std::vector<MeteoData*> meteo = dbHandler.getMeteoData();
	std::string content;
	for(auto it = meteo.begin(); it != meteo.end(); ++it) {
		content += prepareMeteoHTML(*it);
	}

	ostr << "<html><head><title>Meteo</title></head><body><h1>Meteo</h1>"
		 << content << "</body></html>";
}

std::string GetMeteoRequest::prepareMeteoHTML(MeteoData* meteo)
{
	std::string header = "<h2>Meteo station</h2>";
	std::string content = 
		"<ul><li>Air temperature: " + *(meteo -> data[AIR_TEMPERATURE]->text) + "</li>" +
		"<li>Air humidity: " + *(meteo -> data[AIR_HUMIDITY]->text) + "</li></ul>";
	return header + content;
}


