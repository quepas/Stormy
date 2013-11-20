#include "AcquisitionHTTPConnector.h"

#include <iostream>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>

#include "../../common/Utils.h"
#include "JSONUtils.h"

using namespace Stormy;
using namespace Stormy::Data;
using namespace Poco;
using namespace Poco::Net;
using namespace std;

AcquisitionHTTPConnector::AcquisitionHTTPConnector()
{

}

AcquisitionHTTPConnector::~AcquisitionHTTPConnector()
{

}

string AcquisitionHTTPConnector::getDataAsStringAt( string host, unsigned port, string resource )
{
	string content;
	TRY			
	cout << "[HTTPConnector] Try to reach: " << host 
		<< ":" << port << resource << endl;
	HTTPClientSession session;
	session.setHost(host);
	session.setPort(port);	
	session.setTimeout(Timespan(5000));
	HTTPRequest request(HTTPRequest::HTTP_POST, resource);
	session.sendRequest(request);		

	HTTPResponse response;
	istream& receiveStream = session.receiveResponse(response);
	StreamCopier::copyToString(receiveStream, content);

	if(content.empty()) {
		cout << "[HTTPConnector] No data at " << host
			<< ":" << port << resource << endl;
	} else {
		cout << "[HTTPConnector] Fetch data from " << host
			<< ":" << port << resource << endl;
	}
	CATCH_MSG("[HTTPConnector] getDataAt(): ")
	return content;
}

vector<shared_ptr<Station>> AcquisitionHTTPConnector::getStationsAt( 
	string host, unsigned port )
{
	string resource = "/station";
	string content = getDataAsStringAt(host, port, resource);
	return JSONUtils::extractStationsFromJSON(content);
}

vector<shared_ptr<Measurement>> AcquisitionHTTPConnector::getMeasurementsForStationAt( 
	string host, unsigned port, string stationId )
{
	string resource = "/meteo/" + stationId;
	string content = getDataAsStringAt(host, port, resource);
	return JSONUtils::extractMeasurementsFromJSON(content);
}

MetricsPtrVector AcquisitionHTTPConnector::getMetricsAt( string host, uint32 port )
{
	string resource = "/metrics";
	string content = getDataAsStringAt(host, port, resource);
	return JSONUtils::extractMetricsFromJSON(content);
}
