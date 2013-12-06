#include "rest_json_mapper.h"

// TODO: use something lightest for creating json
#include <mongo/client/dbclient.h>

using std::string;
using mongo::BSONObjBuilder;

namespace stormy {
  namespace rest {
    namespace json {

string Mapper::prepareServerInfo()
{
  BSONObjBuilder bson_builder;
  // TODO: use real data
  bson_builder.append(wrapAsJSONString("type"), "storage&acquisition");
  bson_builder.append(wrapAsJSONString("name"), "Stormy Main Server #1");
  bson_builder.append(wrapAsJSONString("status"), "UP");
  return bson_builder.obj().toString();
}

string Mapper::prepareBadResponse(string uri)
{
  BSONObjBuilder bson_builder;
  // TODO: make this more useful
  bson_builder.append(wrapAsJSONString("type"), "bad request");
  bson_builder.append(wrapAsJSONString("reason"), "unknown uri");
  bson_builder.append(wrapAsJSONString("uri"), uri);
  return bson_builder.obj().toString();
}

string Mapper::wrapAsJSONString(string text)
{
  return "\"" + text + "\"";
}
// ~~ stormy::rest::json::Mapper
}}}