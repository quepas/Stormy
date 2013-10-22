#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace Stormy
{
	class GetResourceRequest : public HTTPRequestHandler
	{
		public:
			GetResourceRequest();
			~GetResourceRequest();

			void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
		private:
	};
}