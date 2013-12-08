#include "rest_json_cookbook.h"

#include "../../common/rest_constant.h"

// TODO: use something lightest for creating json
#include <mongo/client/dbclient.h>

using namespace stormy::common::rest;
using stormy::common::entity::Station;
using std::string;
using std::vector;
using mongo::BSONObjBuilder;

namespace stormy {
  namespace rest {
    namespace json {

string Cookbook::prepareServerInfo()
{
  BSONObjBuilder bson_builder;
  // TODO: use real data
  bson_builder.append(wrapAsJSONString("type"), "storage&acquisition");
  bson_builder.append(wrapAsJSONString("name"), "Stormy Main Server #1");
  bson_builder.append(wrapAsJSONString("status"), "UP");
  bson_builder.append(wrapAsJSONString("locale"), "+1");
  bson_builder.append(wrapAsJSONString("metrics"), "[code - description]");
  return bson_builder.obj().toString();
}

string Cookbook::prepareBadResponse(string uri)
{
  BSONObjBuilder bson_builder;
  // TODO: make this more useful
  bson_builder.append(wrapAsJSONString("type"), "bad request");
  bson_builder.append(wrapAsJSONString("reason"), "unknown uri");
  bson_builder.append(wrapAsJSONString("uri"), uri);
  return bson_builder.obj().toString();
}

string Cookbook::wrapAsJSONString(string text)
{
  return "\"" + text + "\"";
}

string Cookbook::prepareStation(Station station)
{
  BSONObjBuilder bson_builder;
  bson_builder.append(wrapAsJSONString(constant::json_uid), station.uid);
  bson_builder.append(wrapAsJSONString(constant::json_name), station.name);
  bson_builder.append(wrapAsJSONString(constant::json_url), station.url);
  bson_builder.append(
    wrapAsJSONString(constant::json_refresh_time), station.refresh_time);
  return bson_builder.obj().toString();
}

string Cookbook::prepareStations(vector<Station> stations)
{
  string content = "{\"" + constant::json_stations_data_marker + "\":[";
  for (auto it = stations.begin(); it != stations.end(); ++it) {
    content += prepareStation(*it) + ",";
  } 
  if(stations.size() > 0)
    content.pop_back();	// remove unnecessary comma
  content += "]}";
  return content;
}
// ~~ stormy::rest::json::Mapper
}}}