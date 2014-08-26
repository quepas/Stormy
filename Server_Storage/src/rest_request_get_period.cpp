#include "rest_request_get_period.h"

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

GetPeriod::GetPeriod(DatabaseSetting aggregate_setting)
  : aggregate_setting_(aggregate_setting)
{

}

GetPeriod::~GetPeriod()
{

}

void GetPeriod::handleRequest(
  HTTPServerRequest& request,
  HTTPServerResponse& response)
{
  db::Aggregate aggregate_storage(aggregate_setting_);
  auto periods = aggregate_storage.GetPeriods();
  ostream& ostr = response.send();

  ostr << json::Cookbook::PreparePeriods(periods);
}
// ~~ stormy::rest::request::GetPeriod
}}}