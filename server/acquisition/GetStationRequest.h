#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "../../common/MeteoStation.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace Stormy
{
	class GetStationRequest : public HTTPRequestHandler
	{
		public:
			GetStationRequest();
			~GetStationRequest();

			void handleRequest(HTTPServerRequest& request, 
				HTTPServerResponse& response);	
		private:
			std::string prepareStationHTML(MeteoStation* station);
	};
}