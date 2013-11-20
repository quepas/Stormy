#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "MeteoData.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace Stormy
{
	class GetMeteoRequest : public HTTPRequestHandler
	{
		public:
			GetMeteoRequest(std::string _stationId, 
				std::string _typeId = "", std::string _timestamp = "");
			~GetMeteoRequest();

			void handleRequest(HTTPServerRequest& request,
				HTTPServerResponse& response);
		private:
			std::string prepareMeteoHTML(Meteo::Measurement* meteo);
			std::string stationId, typeId, timestamp;
	};
}