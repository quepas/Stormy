#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;

namespace Stormy
{
	class GetResourceRequestFactory : public HTTPRequestHandlerFactory
	{
		public:
			GetResourceRequestFactory();
			~GetResourceRequestFactory();
			
			HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

		private:
	};
}