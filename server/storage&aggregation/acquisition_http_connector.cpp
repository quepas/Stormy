#include "acquisition_http_connector.h"

#include <iostream>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>
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

Logger& HTTPConnector::logger_ = Logger::get("aggregation/HTTPConnector");

string HTTPConnector::getDataAsStringAt(string host, unsigned port, string resource)
{
	string content;
	TRY				
	HTTPClientSession session;
	session.setHost(host);
	session.setPort(port);	
	session.setTimeout(Timespan(5000));
	HTTPRequest request(HTTPRequest::HTTP_POST, resource);
	session.sendRequest(request);		

	HTTPResponse response;
	istream& receiveStream = session.receiveResponse(response);
	StreamCopier::copyToString(receiveStream, content);	
	CATCH_MSG(std::string("[HTTPConnector] Exception while reaching ")
		+ host + ":" + NumberFormatter::format(port) + resource)
	return content;
}

vector<shared_ptr<Stormy::Data::Station>> HTTPConnector::getStationsAt(
	string host, unsigned port)
{
	string resource = "/station";
	string content = getDataAsStringAt(host, port, resource);
	return Stormy::JSONUtils::extractStationsFromJSON(content);
}

vector<shared_ptr<Stormy::Data::Measurement>> HTTPConnector::getMeasurementsForStationAt(
	string host, unsigned port, string stationId)
{
	string resource = "/meteo/" + stationId;
	string content = getDataAsStringAt(host, port, resource);
	auto measurements = Stormy::JSONUtils::extractMeasurementsFromJSON(content);

	Stormy::Utils::forEach(measurements, [&](Stormy::MeasurementPtr entry) {
		entry -> station = new Stormy::Data::Station(stationId);
	});

	return measurements;
}

Stormy::MeasurementPtrVector HTTPConnector::getMeasurementsForStationNewerThanAt(
	string host, uint32_t port, string stationId, Timestamp timestamp)
{
	string resource = "/meteo/" + stationId + "/" + 
		NumberFormatter::format(timestamp.epochMicroseconds());
	string content = getDataAsStringAt(host, port, resource);
	auto measurements = Stormy::JSONUtils::extractMeasurementsFromJSON(content);

	Stormy::Utils::forEach(measurements, [&](Stormy::MeasurementPtr entry) {
		entry -> station = new Stormy::Data::Station(stationId);
	});
	return measurements;
}

Stormy::MetricsPtrVector HTTPConnector::getMetricsAt(string host, uint32_t port)
{
	string resource = "/metrics";
	string content = getDataAsStringAt(host, port, resource);
	return Stormy::JSONUtils::extractMetricsFromJSON(content);
}
// ~~ stormy::acquisition::HTTPConnector
}}
