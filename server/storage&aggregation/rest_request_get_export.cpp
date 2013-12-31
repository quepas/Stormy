#include "rest_request_get_export.h"

#include "../../common/rest_constant.h"
#include "../../common/rest_cookbook.h"
#include "../../common/entity_station.h"
#include "rest_csv_cookbook.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/NumberParser.h>

using namespace stormy::common;
using namespace stormy::common::rest;

using boost::split;
using boost::is_any_of;
using std::find;
using std::ostream;
using std::vector;
using std::time_t;
using std::string;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::NumberParser;
using Poco::UInt64;

namespace stormy {
  namespace rest {
    namespace request {

GetExport::GetExport(string uri,  db::Storage* database_storage)
  : database_storage_(database_storage),
    uri_parser_(uri)
{

}

GetExport::~GetExport()
{

}

void GetExport::handleRequest(
  HTTPServerRequest& request,
  HTTPServerResponse& response)
{
  auto path_segments = uri_parser_.getPathSegments();
  auto query_segments = uri_parser_.getQuerySegments();
  ostream& ostr = response.send();

  // api: /export
  if (path_segments.size() == 1) {
    auto stations = database_storage_->GetStations();
    vector<entity::Station> stations_with_any_meteo;

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      uint32_t count = database_storage_->CountStationMeasurements(it->uid);
      if (count > 0) 
        stations_with_any_meteo.push_back(*it);
    }
    ostr << cookbook::PrepareStationUIDsWithAnyMeteo(stations_with_any_meteo);
  }
  // api: /export/:station_uid
  else if (path_segments.size() == 2) {
    if (query_segments.empty()) {
      auto measurements = database_storage_->
        GetAllMeasureSetsForStation(path_segments[1]);
      auto valid_metrics = database_storage_->GetMetricsCodes();
      ostr << csv::PrepareMeteo(measurements, valid_metrics);
    } else {
      auto from_index = query_segments.find(constant::from_parameter);
      auto to_index = query_segments.find(constant::to_parameter);
      auto metrics_index = query_segments.find(constant::metrics_parameter);

      time_t from_ts = 0, to_ts = common::LocaltimeNow();
      vector<string> metrics;
      UInt64 temporary_ts;
      if (from_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          from_index->second, temporary_ts)) {
            from_ts = temporary_ts;
        }
      }
      if (to_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          to_index->second, temporary_ts)) {
            to_ts = temporary_ts;
        }
      }
      if (metrics_index != query_segments.end()) {
        vector<string> splitted_metrics;
        split(splitted_metrics, metrics_index->second , is_any_of(","));
        auto valid_metrics = database_storage_->GetMetricsCodes();

        for (auto it = splitted_metrics.begin(); it != splitted_metrics.end(); ++it) {
          if (find(valid_metrics.begin(), valid_metrics.end(), *it) != valid_metrics.end() && !it->empty()) {
            metrics.push_back(*it);
          }
        }
      }
      auto measurements = database_storage_->
        GetMeasureSetsForStationBetweenTS(path_segments[1], from_ts, to_ts);
      ostr << csv::PrepareMeteo(measurements, metrics);
    }
  }
}
// ~~ stormy::rest::request::GetInfo
}}}
