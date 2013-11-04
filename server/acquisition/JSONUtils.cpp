#include "JSONUtils.h"

#include <mongo/client/dbclient.h>
#include <boost/any.hpp>
#include "MeteoConst.h"
#include "Utils.h"

using namespace Stormy;
using namespace Meteo;

using mongo::BSONObj;
using mongo::BSONObjBuilder;
using boost::any_cast;
using boost::any;

std::string JSONUtils::prepareJSONForStation( Station* station )
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(Const::id), station -> stationId);
	bsonBuilder.append(wrapAsJSONString(Const::name), station -> name);
	bsonBuilder.append(wrapAsJSONString(Const::url), station -> url);
	bsonBuilder.append(wrapAsJSONString(Const::refreshTime), station -> refreshTime);
	return bsonBuilder.obj().toString();
}

std::string JSONUtils::prepareJSONForStations( const std::vector<Station*>& stations )
{
	std::string content = "{\"stations\":[";	
	Utils::forEach(stations, [&](Station* station) {
		content += prepareJSONForStation(station) + ",";
	});
	if(stations.size() > 0)
		content.pop_back();	// remove unnecessary comma
	content += "]}";
	return content;
}

std::string JSONUtils::prepareJSONForMeasurement( Measurement* measurement )
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(Const::id), 
		Utils::getStringFromAny(measurement -> data[Const::mongoId]));
	Utils::forEach(measurement -> data, [&](std::pair<std::string, any> pair) {
		if(pair.first != Const::mongoId) {
			bsonBuilder.append(wrapAsJSONString(pair.first), Utils::getStringFromAny(pair.second));
		}
	});
	return bsonBuilder.obj().toString();
}

std::string JSONUtils::prepareJSONForMeasurements( const std::vector<Meteo::Measurement*>& measurements )
{
	std::string content = "{\"meteo\":[";
	Utils::forEach(measurements, [&](Measurement* measurement) {		
		content += prepareJSONForMeasurement(measurement) + ",";
	});
	if(measurements.size() > 0)
		content.pop_back();	// remove unnecessary comma
	content += "]}";
	return content;
}

std::string JSONUtils::wrapAsJSONString( std::string label )
{
	return "\"" + label + "\"";
}

std::string JSONUtils::prepareJSONForAvailableType( Type* type )
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(Const::id), type -> id);
	bsonBuilder.append(wrapAsJSONString(Const::name), type -> equivalents[0]);
	return bsonBuilder.obj().toString();
}

std::string JSONUtils::prepareJSONForAvailableTypes( const std::vector<Meteo::Type*>& types )
{
	std::string content = "{\"availableTypes\":[";
	Utils::forEach(types, [&](Type* type) {
		content += prepareJSONForAvailableType(type) + ",";
	});
	if(types.size() > 0)
		content.pop_back();	// remove unnecessary coma
	content += "]}";
	return content;
}
