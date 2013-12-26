#include "rest_json_cookbook.h"

#include "acquisition_constant.h"
#include "rest_constant.h"
#include "../../common/util.h"

#include <mongo/client/dbclient.h>
#include <boost/any.hpp>
#include <Poco/NumberFormatter.h>

using namespace stormy::common;
using boost::any;
using std::string;
using std::pair;
using std::vector;
using mongo::BSONObjBuilder;
using Poco::NumberFormatter;

namespace stormy {
  namespace rest {
    namespace json {
      namespace cookbook {

string PrepareStation(entity::Station station)
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder
    .append(wrapAsJSONString(acquisition::constant::id), station.uid)
    .append(wrapAsJSONString(acquisition::constant::name), station.name)
    .append(wrapAsJSONString(acquisition::constant::url), station.url)
	  .append(wrapAsJSONString(acquisition::constant::refreshTime), 
      station.refresh_time);
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareStations(const vector<entity::Station>& stations)
{
	string content = "{\"stations\":[";
	Each(stations, [&](entity::Station station) {
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
		ToString(measurement -> data[acquisition::constant::mongoId]));
	Each(measurement -> data, [&](pair<string, any> pair) {
		if(pair.first != acquisition::constant::mongoId) {
			bsonBuilder.append(
        wrapAsJSONString(pair.first), ToString(pair.second));
		}
	});
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareCombinedMeasurements(
  const Stormy::MeasurementPtrVector& measurements)
{
	string content = "{\"meteo\":[";
	Each(measurements, [&](Stormy::MeasurementPtr measurement) {
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

string cookbook::PrepareMetric(entity::Metrics metrics)
{
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::code), metrics.code);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::name), metrics.equivalents);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::type), metrics.type);
	bsonBuilder.append(
    wrapAsJSONString(acquisition::constant::unit), metrics.unit);
	string format = metrics.format;
	if(!format.empty()) {
		bsonBuilder.append(
      wrapAsJSONString(acquisition::constant::format), metrics.format);
  }
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareMetrics(const vector<entity::Metrics>& metrics_vec)
{
	string content = "{\"metrics\":[";
	Each(metrics_vec, [&](entity::Metrics metrics) {
		if(metrics.is_meteo)
			content += PrepareMetric(metrics) + ",";
	});
	if(metrics_vec.size() > 0)
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
    ToString(pair -> second));
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareMeasurements(
  const Stormy::MeasurementPtrVector& measurements)
{
	string content = "{\"measurements\":[";
	Each(measurements, [&](Stormy::MeasurementPtr measurement) {
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
  const vector<entity::Metrics>& metrics_vec)
{
  string server_info = "{" + wrapAsJSONString("server") + ":{";
  server_info += wrapAsJSONString("type") + ":";
  server_info += wrapAsJSONString(server_type) + "},";

  string content = server_info + "\"metrics\":[";
  Each(metrics_vec, [&](entity::Metrics metrics) {
    if (metrics.is_meteo)
      content += PrepareMetric(metrics) + ",";
  });
  if(metrics_vec.size() > 0)
    content.pop_back();	// remove unnecessary coma
  content += "]}";
  return content;
}
// ~~ rest::json::Cookbook
}}}}