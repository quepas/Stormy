#pragma once

#include <string>

namespace Stormy
{
	class AcquisitionHTTPConnector
	{
		public:
			AcquisitionHTTPConnector();
			~AcquisitionHTTPConnector();

			static std::string getDataAt(std::string host, unsigned port, std::string resource);			
		private:

	};
}