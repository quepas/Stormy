#include "rest_json_cookbook.h"

#include "acquisition_constant.h"
#include "rest_constant.h"
#include "../../common/util.h"

#include <mongo/client/dbclient.h>
#include <boost/any.hpp>
#include <Poco/NumberFormatter.h>

using boost::any;
using std::string;
using std::pair;
using mongo::BSONObjBuilder;
using Poco::NumberFormatter;

namespace stormy {
  namespace rest {
    namespace json {
      namespace cookbook {

string PrepareStation(Stormy::StationPtr station)
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::id), station -> stationId);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::name), station -> name);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::url), station -> url);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::refreshTime), 
    station -> refreshTime);
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareStations(const Stormy::StationPtrVector& stations)
{
	string content = "{\"stations\":[";
	common::Each(stations, [&](Stormy::StationPtr station) {
		content += PrepareStation(station) + ",";
	});
	if(stations.size() > 0)
		content.pop_back();	// remove unnecessary comma
	content += "]}";
	return content;
}

string cookbook::PrepareCombinedMeasurement(
  Stormy::MeasurementPtr measurement)
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(acquisition::constant::id),
		common::ToString(measurement -> data[acquisition::constant::mongoId]));
	common::Each(measurement -> data, [&](pair<string, any> pair) {
		if(pair.first != acquisition::constant::mongoId) {
			bsonBuilder.append(
        wrapAsJSONString(pair.first), common::ToString(pair.second));
		}
	});
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareCombinedMeasurements(
  const Stormy::MeasurementPtrVector& measurements)
{
	string content = "{\"meteo\":[";
	common::Each(measurements, [&](Stormy::MeasurementPtr measurement) {
		content += PrepareCombinedMeasurement(measurement) + ",";
	});
	if(measurements.size() > 0)
		content.pop_back();	// remove unnecessary comma
	content += "]}";
	return content;
}

string cookbook::wrapAsJSONString(string label)
{
	return "\"" + label + "\"";
}

string cookbook::PrepareMetric(Stormy::TypePtr type)
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::code), type -> id);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::name), type -> equivalents[0]);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::type), type -> valueType);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::unit), type -> valueUnit);
	string format = type -> valueFormat;
	if(!format.empty()) {
		bsonBuilder.append(
      wrapAsJSONString(acquisition::constant::format), type -> valueFormat);
  }
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareMetrics(const Stormy::TypePtrVector& types)
{
	string content = "{\"metrics\":[";
	common::Each(types, [&](Stormy::TypePtr type) {
		if(type -> isMeteo)
			content += PrepareMetric(type) + ",";
	});
	if(types.size() > 0)
		content.pop_back();	// remove unnecessary coma
	content += "]}";
	return content;
}

string cookbook::PrepareMeasurement(Stormy::MeasurementPtr measurement)
{
	if(!measurement || measurement -> data.empty()) 
    return constant::emptyJSON;
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(acquisition::constant::timestamp),
		NumberFormatter::format(measurement -> timestamp.epochTime()));
	auto pair = measurement -> data.begin();
	bsonBuilder.append(wrapAsJSONString(
    acquisition::constant::data), 
    common::ToString(pair -> second));
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareMeasurements(
  const Stormy::MeasurementPtrVector& measurements)
{
	string content = "{\"measurements\":[";
	common::Each(measurements, [&](Stormy::MeasurementPtr measurement) {
		if(!measurement -> data.empty())
			content += PrepareMeasurement(measurement) + ",";
	});
	if(measurements.size() > 0)
		content.pop_back();	// remove unnecessary coma
	content += "]}";
	return content;
}

string cookbook::PrepareInfo(
  const string& server_type, 
  const Stormy::TypePtrVector& types)
{
  string server_info = "{" + wrapAsJSONString("server") + ":{";
  server_info += wrapAsJSONString("type") + ":";
  server_info += wrapAsJSONString(server_type) + "},";

  string content = server_info + "\"metrics\":[";
  common::Each(types, [&](Stormy::TypePtr type) {
    if(type -> isMeteo)
      content += PrepareMetric(type) + ",";
  });
  if(types.size() > 0)
    content.pop_back();	// remove unnecessary coma
  content += "]}";
  return content;
}
// ~~ rest::json::Cookbook
}}}}