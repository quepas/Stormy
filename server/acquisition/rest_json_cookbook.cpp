#include "rest_json_cookbook.h"

#include "db_constant.h"
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
    .append(wrapAsJSONString(db::constant::id), station.uid)
    .append(wrapAsJSONString(db::constant::name), station.name)
    .append(wrapAsJSONString(db::constant::url), station.url)
	  .append(wrapAsJSONString(db::constant::refresh_time), 
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
  const std::vector<entity::Measurement>& measurement)
{
	BSONObjBuilder bsonBuilder;
/*	bsonBuilder.append(wrapAsJSONString(db::constant::id),
		ToString(measurement -> data[db::constant::mongoId]));
	Each(measurement -> data, [&](pair<string, any> pair) {
		if(pair.first != db::constant::mongoId) {
			bsonBuilder.append(
        wrapAsJSONString(pair.first), ToString(pair.second));
		}
	});*/
	return bsonBuilder.obj().toString();
}

string cookbook::PrepareCombinedMeasurements(
  const std::vector<entity::Measurement>& measurements)
{
	string content = "{\"meteo\":[";
	/*Each(measurements, [&](Stormy::MeasurementPtr measurement) {
		content += PrepareCombinedMeasurement(measurement) + ",";
	});
	if(measurements.size() > 0)
		content.pop_back();	// remove unnecessary comma*/
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
    wrapAsJSONString(db::constant::code), metrics.code);
	bsonBuilder.append(
    wrapAsJSONString(db::constant::name), metrics.equivalents);
	bsonBuilder.append(
    wrapAsJSONString(db::constant::type), metrics.type);
	bsonBuilder.append(
    wrapAsJSONString(db::constant::unit), metrics.unit);
	string format = metrics.format;
	if(!format.empty()) {
		bsonBuilder.append(
      wrapAsJSONString(db::constant::format), metrics.format);
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

string cookbook::PrepareMeasurement(
  const std::vector<entity::Measurement>& measurement)
{
	/*if(!measurement || measurement -> data.empty()) 
    return constant::emptyJSON;
	BSONObjBuilder bsonBuilder;
	bsonBuilder.append(wrapAsJSONString(db::constant::timestamp),
		NumberFormatter::format(measurement -> timestamp.epochTime()));
	auto pair = measurement -> data.begin();
	bsonBuilder.append(wrapAsJSONString(
    db::constant::data), 
    ToString(pair -> second));
	return bsonBuilder.obj().toString();*/
  return "{}";
}

string cookbook::PrepareMeasurements(
  const std::vector<entity::Measurement>& measurements)
{
	string content = "{\"measurements\":[";
	/*Each(measurements, [&](Stormy::MeasurementPtr measurement) {
		if(!measurement -> data.empty())
			content += PrepareMeasurement(measurement) + ",";
	});
	if(measurements.size() > 0)
		content.pop_back();	// remove unnecessary coma*/
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