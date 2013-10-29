#include "GetMeteoRequest.h"

#include "MongoDBHandler.h"
#include "Utils.h"
#include "JSONUtils.h"
#include "TypeConfiguration.h"
#include "RESTConst.h"

#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

using namespace Stormy;
using namespace Meteo;
using REST::Const;

GetMeteoRequest::GetMeteoRequest(std::string _stationId)
	:	stationId(_stationId)
{

}

GetMeteoRequest::~GetMeteoRequest()
{

}

void GetMeteoRequest::handleRequest( HTTPServerRequest& request, HTTPServerResponse& response )
{
	std::ostream& ostr = response.send();

	MongoDBHandler dbHandler("localhost");
	if(stationId != Const::none) {	
		std::vector<Measurement*> meteo = dbHandler.getMeteoData(stationId);		
		ostr << JSONUtils::prepareJSONForMeasurements(meteo);
	} else {
		ostr << Const::emptyJSON;
	}
}

std::string GetMeteoRequest::prepareMeteoHTML(Measurement* meteo)
{
	TypeConfiguration* typesCfg = 
		new TypeConfiguration("config/meteo_data_type_config.yaml");

	std::string header = "<h2>Meteo station</h2>";
	std::string content = "<ul>";
	
	auto data = meteo -> data;
	for(auto it = data.begin(); it != data.end(); ++it) {
		std::string id = it -> first;
		Type* type = typesCfg->getFullTypeById(id);
		std::string rowContent = "<li>";
		rowContent.append(typesCfg -> getFirstEquivalentById(id));
		rowContent.append(": ");
		if(type -> valueType == "number")
			rowContent.append(boost::lexical_cast<std::string>(
				boost::any_cast<double>(it -> second)));
		else if(type ->valueType == "text")
			rowContent.append(boost::any_cast<std::string>(it -> second));
		rowContent.append("</li>");
		content.append(rowContent);
	}	
	content.append("</ul>");
	return header + content;
}


