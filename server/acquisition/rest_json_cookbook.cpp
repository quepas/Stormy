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