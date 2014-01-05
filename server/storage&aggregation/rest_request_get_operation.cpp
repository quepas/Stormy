#include "rest_request_get_operation.h"

#include "db_aggregate.h"
#include "rest_json_cookbook.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

using std::ostream;

namespace stormy {
  namespace rest {
    namespace request {

GetOperation::GetOperation(common::db::Setting aggregate_setting)
  : aggregate_setting_(aggregate_setting)
{

}

GetOperation::~GetOperation()
{

}

void GetOperation::handleRequest(
  HTTPServerRequest& request,
  HTTPServerResponse& response)
{
  db::Aggregate aggregate_storage(aggregate_setting_);
  auto operations = aggregate_storage.GetOperations();
  ostream& ostr = response.send();
  
  ostr << json::Cookbook::PrepareOperations(operations);
}
// ~~ stormy::rest::request::GetOperation
}}}
