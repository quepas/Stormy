#include "acquisition_http_connector.h"

#include <iostream>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/NumberFormatter.h>

#include "../../common/Utils.h"
#include "JSONUtils.h"

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

vector<shared_ptr<Stormy::Data::Station>> 
  HTTPConnector::FetchStationsAt() const
{
	string resource = "/station";
	string content = FetchDataAsStringAt(resource);
	return Stormy::JSONUtils::extractStationsFromJSON(content);
}

vector<shared_ptr<Stormy::Data::Measurement>> 
  HTTPConnector::FetchMeasurementsForStationAt(string stationId) const
{
	string resource = "/meteo/" + stationId;
	string content = FetchDataAsStringAt(resource);
	auto measurements = Stormy::JSONUtils::extractMeasurementsFromJSON(content);

	Stormy::Utils::forEach(measurements, [&](Stormy::MeasurementPtr entry) {
		entry -> station = new Stormy::Data::Station(stationId);
	});

	return measurements;
}

Stormy::MeasurementPtrVector HTTPConnector::FetchMeasurementsForStationNewerThanAt(
  string stationId, Timestamp timestamp) const
{
	string resource = "/meteo/" + stationId + "/" + 
		NumberFormatter::format(timestamp.epochMicroseconds());
	string content = FetchDataAsStringAt(resource);
	auto measurements = Stormy::JSONUtils::extractMeasurementsFromJSON(content);

	Stormy::Utils::forEach(measurements, [&](Stormy::MeasurementPtr entry) {
		entry -> station = new Stormy::Data::Station(stationId);
	});
	return measurements;
}

Stormy::MetricsPtrVector HTTPConnector::FetchMetricsAt() const
{
	string resource = "/metrics";
	string content = FetchDataAsStringAt(resource);
	return Stormy::JSONUtils::extractMetricsFromJSON(content);
}
// ~~ stormy::acquisition::HTTPConnector
}}
