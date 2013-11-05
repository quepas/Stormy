#pragma once

#include <Poco/Util/ServerApplication.h>

using Poco::Util::Application;
using Poco::Util::ServerApplication;

namespace Stormy
{
	class HttpServer : public ServerApplication
	{
		public:
			HttpServer(unsigned int _port = 8080);
			~HttpServer();

		protected:

			void initialized(Application& self);
			void unitialized();
			int main(const std::vector<std::string>& args);

		private:
			unsigned int port;
	};
}