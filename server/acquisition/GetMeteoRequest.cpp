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
				vector<Measurement*> meteo = dbHandler.getMeteoData(stationId);
				ostr << JSONUtils::prepareJSONForMeasurements(meteo);
			} else {				
				vector<Measurement*> meteo = dbHandler.getMeteoDataNewerThan(stationId, timestamp);
				ostr << JSONUtils::prepareJSONForMeasurements(meteo);
			}			
		} else {
			vector<Measurement*> singleMeteo = dbHandler.getCurrentMeteoTypeDatas(stationId, typeId);
			ostr << JSONUtils::prepareJSONForSingleMeasurements(singleMeteo);
		}
	} else {
		ostr << REST::Const::emptyJSON;
	}
}

string GetMeteoRequest::prepareMeteoHTML(Measurement* meteo)
{
	TypeConfiguration* typesCfg =
		new TypeConfiguration("config/meteo_data_type_config.yaml");

	string header = "<h2>Meteo station</h2>";
	string content = "<ul>";

	auto data = meteo -> data;
	for(auto it = data.begin(); it != data.end(); ++it) {
		string id = it -> first;
		TypePtr type = typesCfg->getFullTypeById(id);
		string rowContent = "<li>";
		rowContent.append(typesCfg -> getFirstEquivalentById(id));
		rowContent.append(": ");
		if(type -> valueType == "number")
			rowContent.append(boost::lexical_cast<string>(
				boost::any_cast<double>(it -> second)));
		else if(type ->valueType == "text")
			rowContent.append(boost::any_cast<string>(it -> second));
		rowContent.append("</li>");
		content.append(rowContent);
	}
	content.append("</ul>");
	return header + content;
}


