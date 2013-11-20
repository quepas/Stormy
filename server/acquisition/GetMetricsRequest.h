#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace Stormy
{
	namespace REST
	{
		class GetMetricsRequest : public HTTPRequestHandler
		{
		public:
			GetMetricsRequest(std::string _stationId = "");
			~GetMetricsRequest();

			void handleRequest(HTTPServerRequest& request, 
				HTTPServerResponse& response);
		private:
			std::string stationId;
		};
	}
}