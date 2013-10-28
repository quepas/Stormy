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
	bsonBuilder.append(Const::id, station -> stationId);
	bsonBuilder.append(Const::name, station -> name);
	bsonBuilder.append(Const::url, station -> url);
	bsonBuilder.append(Const::refreshTime, station -> refreshTime);
	return bsonBuilder.obj().toString();
}

std::string JSONUtils::prepareJSONForStations( const std::vector<Station*>& stations )
{
	std::string content = "{";	
	Utils::forEach(stations, [&](Station* station) {
		content += prepareJSONForStation(station);
	});
	content += "}";
	return content;
}

std::string Stormy::JSONUtils::prepareJSONForMeasurement( Meteo::Measurement* measurement )
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(Const::id, any_cast<std::string>(measurement -> data[Const::mongoId]));
	Utils::forEach(measurement -> data, [&](std::pair<std::string, any> pair) {
		if(pair.first != Const::mongoId) {
			bsonBuilder.append(pair.first, Utils::getStringFromAny(pair.second));
		}
	});
	return bsonBuilder.obj().toString();
}

std::string Stormy::JSONUtils::prepareJSONForMeasurements( const std::vector<Meteo::Measurement*>& measurements )
{
	std::string content = "{";
	Utils::forEach(measurements, [&](Measurement* measurement) {
		content += prepareJSONForMeasurement(measurement);
	});
	content += "}";
	return content;
}
