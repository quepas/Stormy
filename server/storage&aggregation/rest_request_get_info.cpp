#include "rest_request_get_info.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "rest_json_cookbook.h"

using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetInfo::GetInfo(db::Storage* database_storage)
  : database_storage_(database_storage)
{

}

GetInfo::~GetInfo()
{

}

void GetInfo::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response)
{  
  ostream& stream_response = response.send();
  stream_response << 
    json::Cookbook::PrepareServerInfo(
      database_storage_->GetMetrics(), 
      database_storage_->GetPeriods());
}
// ~~ stormy::rest::request::GetInfo
}}}