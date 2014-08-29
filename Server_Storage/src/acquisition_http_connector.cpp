#include "acquisition_http_connector.h"

#include <Poco/Timespan.h>

#include "common/util.h"
#include "common/net_util.hpp"
#include "acquisition_json_util.h"

using namespace stormy::common;

using std::string;
using std::to_string;
using std::vector;
using std::map;
using std::time_t;
using Poco::Logger;
using Poco::Timespan;

namespace stormy {
  namespace acquisition {

HTTPConnector::HTTPConnector(const string& host, unsigned port)
  : logger_(Logger::get("main")),
    host_(host),
    port_(port)
{

}

string HTTPConnector::FetchDataAsStringAt(string resource) const
{
  string resource_url = net::PrepareHttpUrl(host_, port_, resource);
  return net::FetchWebsite(resource_url);
}

vector<entity::Station> HTTPConnector::FetchStations() const
{
  vector<entity::Station> stations;
  string resource = "/station";
  string json_response = FetchDataAsStringAt(resource);
  auto stations_uids = json::ExtractSimpleListElements(json_response, "stations");
  for (auto& uid : stations_uids) {
    resource = "/station/" + uid;
    json_response = FetchDataAsStringAt(resource);
    stations.push_back(json::ExtractStation(json_response));
  }
  return stations;
}

map<time_t, vector<entity::Measurement>>
  HTTPConnector::FetchMeasureSets(string station_uid, time_t from_time) const
{
  string resource = "/meteo/" + station_uid + "?from=" + to_string(from_time);
  string json_response = FetchDataAsStringAt(resource);
  return json::ExtractMeasureSets(json_response, station_uid);
}

vector<entity::Metrics> HTTPConnector::FetchMetrics() const
{
  vector<entity::Metrics> metrics;
  string resource = "/metrics";
  string json_response = FetchDataAsStringAt(resource);
  auto metrics_codes = json::ExtractSimpleListElements(json_response, "metrics");
  for (auto& code : metrics_codes) {
    resource = "/metrics/" + code;
    json_response = FetchDataAsStringAt(resource);
    metrics.push_back(json::ExtractMetrics(json_response));
  }
  return metrics;
}

}}
// ~~ stormy::acquisition::HTTPConnector
