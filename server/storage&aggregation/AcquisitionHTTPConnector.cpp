#include "AcquisitionHTTPConnector.h"

#include <iostream>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>

#include "../../common/Utils.h"

using namespace Stormy;
using namespace Poco;
using namespace Poco::Net;
using namespace std;

AcquisitionHTTPConnector::AcquisitionHTTPConnector()
{

}

AcquisitionHTTPConnector::~AcquisitionHTTPConnector()
{

}

string AcquisitionHTTPConnector::getDataAt( string host, unsigned port, string resource )
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
