#include "acquisition_http_connector.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/NumberFormatter.h>

#include "../../common/util.h"
#include "acquisition_json_util.h"

using namespace stormy::common;

using std::istream;
using std::string;
using std::shared_ptr;
using std::vector;
using std::map;
using std::time_t;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Logger;
using Poco::NumberFormatter;
using Poco::StreamCopier;
using Poco::Timespan;
using Poco::Timestamp;

namespace stormy {
  namespace acquisition {

HTTPConnector::HTTPConnector(const string host, const uint32_t port)
  : logger_(Logger::get("aggregation/HTTPConnector")),
    host_(host),
    port_(port)
{

}

string HTTPConnector::FetchDataAsStringAt(string resource) const
{
	string content;
	TRY				
	HTTPClientSession session;
	session.setHost(host_);
	session.setPort(port_);	
	session.setTimeout(Timespan(5000));
	HTTPRequest request(HTTPRequest::HTTP_POST, resource);
	session.sendRequest(request);		

	HTTPResponse response;
	istream& receiveStream = session.receiveResponse(response);
	StreamCopier::copyToString(receiveStream, content);	
	CATCH_MSG(string("[acquisition/HTTPConnector] Exception while reaching ")
		+ host_ + ":" + NumberFormatter::format(port_) + resource)
	return content;
}

vector<entity::Station>
  HTTPConnector::FetchStationsAt() const
{
  vector<entity::Station> stations;
	string resource = "/station";
	string json_response = FetchDataAsStringAt(resource);
  auto stations_uids = 
    json::ExtractSimpleListElements(json_response, "stations");

  for (auto it = stations_uids.begin(); it != stations_uids.end(); ++it) {
    resource = "/station/" + *it;
    json_response = FetchDataAsStringAt(resource);
    stations.push_back(json::ExtractStation(json_response));
  }
  return stations;
}

map<time_t, vector<entity::Measurement>>
  HTTPConnector::FetchMeasurementsForStationAt(string station_uid) const
{
	string resource = "/meteo/" + station_uid + "?from=0";
	string json_response = FetchDataAsStringAt(resource);
	auto measurements = json::ExtractMeasureSets(json_response, station_uid);
	return measurements;
}

map<time_t, vector<entity::Measurement>>
  HTTPConnector::FetchMeasurementsForStationNewerThanAt(
    string station_uid, Timestamp timestamp) const
{
	string resource = "/meteo/" + station_uid + "?from=" +
		NumberFormatter::format(timestamp.epochMicroseconds());
	string json_response = FetchDataAsStringAt(resource);
	auto measurements = json::ExtractMeasureSets(json_response, station_uid);
	return measurements;
}

vector<entity::Metrics> HTTPConnector::FetchMetricsAt() const
{
	vector<entity::Metrics> metrics;
  string resource = "/metrics";
	string json_response = FetchDataAsStringAt(resource);
  auto metrics_codes = 
    json::ExtractSimpleListElements(json_response, "metrics");

  for (auto it = metrics_codes.begin(); it != metrics_codes.end(); ++it) {
    resource = "/metrics/" + *it;
    json_response = FetchDataAsStringAt(resource);
    metrics.push_back(json::ExtractMetrics(json_response));
  }	
  return metrics;
}
// ~~ stormy::acquisition::HTTPConnector
}}
