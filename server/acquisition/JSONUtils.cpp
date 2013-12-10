#include "JSONUtils.h"

#include <mongo/client/dbclient.h>
#include <boost/any.hpp>
#include "MeteoConst.h"
#include "../../common/Utils.h"
#include "RESTConst.h"

using namespace Stormy;
using namespace Meteo;
using namespace std;

using mongo::BSONObj;
using mongo::BSONObjBuilder;
using boost::any_cast;
using boost::any;

string JSONUtils::prepareJSONForStation( StationPtr station )
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(Const::id), station -> stationId);
	bsonBuilder.append(wrapAsJSONString(Const::name), station -> name);
	bsonBuilder.append(wrapAsJSONString(Const::url), station -> url);
	bsonBuilder.append(wrapAsJSONString(Const::refreshTime), station -> refreshTime);
	return bsonBuilder.obj().toString();
}

string JSONUtils::prepareJSONForStations( const StationPtrVector& stations )
{
	string content = "{\"stations\":[";
	Utils::forEach(stations, [&](StationPtr station) {
		content += prepareJSONForStation(station) + ",";
	});
	if(stations.size() > 0)
		content.pop_back();	// remove unnecessary comma
	content += "]}";
	return content;
}

string JSONUtils::prepareJSONForMeasurement( MeasurementPtr measurement )
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(Const::id),
		Utils::getStringFromAny(measurement -> data[Const::mongoId]));
	Utils::forEach(measurement -> data, [&](pair<string, any> pair) {
		if(pair.first != Const::mongoId) {
			bsonBuilder.append(wrapAsJSONString(pair.first), Utils::getStringFromAny(pair.second));
		}
	});
	return bsonBuilder.obj().toString();
}

string JSONUtils::prepareJSONForMeasurements( const MeasurementPtrVector& measurements )
{
	string content = "{\"meteo\":[";
	Utils::forEach(measurements, [&](MeasurementPtr measurement) {
		content += prepareJSONForMeasurement(measurement) + ",";
	});
	if(measurements.size() > 0)
		content.pop_back();	// remove unnecessary comma
	content += "]}";
	return content;
}

string JSONUtils::wrapAsJSONString( string label )
{
	return "\"" + label + "\"";
}

string JSONUtils::prepareJSONForAvailableType( TypePtr type )
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(Const::code), type -> id);
	bsonBuilder.append(wrapAsJSONString(Const::name), type -> equivalents[0]);
	bsonBuilder.append(wrapAsJSONString(Const::type), type -> valueType);
	bsonBuilder.append(wrapAsJSONString(Const::unit), type -> valueUnit);
	string format = type -> valueFormat;
	if(!format.empty())
		bsonBuilder.append(wrapAsJSONString(Const::format), type -> valueFormat);
	return bsonBuilder.obj().toString();
}

string JSONUtils::prepareJSONForAvailableTypes( const TypePtrVector& types )
{
	string content = "{\"metrics\":[";
	Utils::forEach(types, [&](TypePtr type) {
		if(type -> isMeteo)
			content += prepareJSONForAvailableType(type) + ",";
	});
	if(types.size() > 0)
		content.pop_back();	// remove unnecessary coma
	content += "]}";
	return content;
}

string JSONUtils::prepareJSONForSingleMeasurement( MeasurementPtr measurement )
{
	if(!measurement || measurement -> data.empty()) return REST::Const::emptyJSON;
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(Const::timestamp),
		lexical_cast<string>(measurement -> timestamp.epochTime()));
	auto pair = measurement -> data.begin();
	bsonBuilder.append(wrapAsJSONString(Const::data), Utils::getStringFromAny(pair -> second));
	return bsonBuilder.obj().toString();
}

string JSONUtils::prepareJSONForSingleMeasurements( const MeasurementPtrVector& measurements )
{
	string content = "{\"measurements\":[";
	Utils::forEach(measurements, [&](MeasurementPtr measurement) {
		if(!measurement -> data.empty())
			content += prepareJSONForSingleMeasurement(measurement) + ",";
	});
	if(measurements.size() > 0)
		content.pop_back();	// remove unnecessary coma
	content += "]}";
	return content;
}

string JSONUtils::prepareJSONForInfo(const std::string& server_type, const TypePtrVector& types)
{
  string server_info = "{" + wrapAsJSONString("server") + ":{";
  server_info += wrapAsJSONString("type") + ":";
  server_info += wrapAsJSONString(server_type) + "},";

  string content = server_info + "\"metrics\":[";
  Utils::forEach(types, [&](TypePtr type) {
    if(type -> isMeteo)
      content += prepareJSONForAvailableType(type) + ",";
  });
  if(types.size() > 0)
    content.pop_back();	// remove unnecessary coma
  content += "]}";
  return content;
}
