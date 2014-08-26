#include "rest_request_get_metrics.h"

#include "db_storage.h"
#include "rest_cookbook.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using namespace stormy::common::rest;
using std::string;
using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetMetrics::GetMetrics(string uri, DatabaseSetting storage_setting)
  : uri_parser_(uri),
    storage_setting_(storage_setting)
{

}

GetMetrics::~GetMetrics()
{

}

void GetMetrics::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response)
{
  db::Storage database_storage_(storage_setting_);
  ostream& ostr = response.send();
  auto path_segments = uri_parser_.getPathSegments();
  auto metrics = database_storage_.GetMetrics();

  if (path_segments.size() == 1) {    
    ostr << cookbook::PrepareMetricsCodes(metrics);
  } else if (path_segments.size() == 2) {
    for (auto it = metrics.begin(); it != metrics.end(); ++it) {
      if (it->code == path_segments[1]) {
        ostr << cookbook::PrepareMetricsInfo(*it);
        return;
      }
    }
    ostr << cookbook::PrepareError(
      "Bad request. Unknown metrics code.", 
      "Use existing metrics code.");
  } else {
    ostr << cookbook::PrepareError(
      "Bad request. Too much URI segments", 
      "Create proper metrics request.");
}
}
// ~~ stormy::rest::request::GetMetrics
}}}
