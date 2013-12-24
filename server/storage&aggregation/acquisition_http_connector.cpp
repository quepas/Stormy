#include "acquisition_http_connector.h"

#include <iostream>
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
	string resource = "/station";
	string content = FetchDataAsStringAt(resource);
	return json::ExtractStations(content);
}

vector<entity::Measurement>
  HTTPConnector::FetchMeasurementsForStationAt(string station_uid) const
{
	string resource = "/meteo/" + station_uid;
	string content = FetchDataAsStringAt(resource);
	auto measurements = json::ExtractMeasurements(content);

  for (auto it = measurements.begin(); it != measurements.end(); ++it) {
    it -> station_uid = station_uid;
  }
	return measurements;
}

vector<entity::Measurement> 
  HTTPConnector::FetchMeasurementsForStationNewerThanAt(
    string station_uid, Timestamp timestamp) const
{
	string resource = "/meteo/" + station_uid + "/" + 
		NumberFormatter::format(timestamp.epochMicroseconds());
	string content = FetchDataAsStringAt(resource);
	auto measurements = json::ExtractMeasurements(content);

  for (auto it = measurements.begin(); it != measurements.end(); ++it) {
    it -> station_uid = station_uid;
  }
	return measurements;
}

vector<entity::Metrics> HTTPConnector::FetchMetricsAt() const
{
	string resource = "/metrics";
	string content = FetchDataAsStringAt(resource);
	return json::ExtractMetrics(content);
}
// ~~ stormy::acquisition::HTTPConnector
}}
