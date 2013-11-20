#pragma once

#include <string>
#include <Poco/NumberFormatter.h>

#include "../../common/Types.h"
#include "../../common/Utils.h"

namespace Stormy
{
	struct AcquisitionServer
	{
		std::string id;
		std::string name;
		std::string host;
		unsigned int port;
		uint32 acquisitionInterval;	// seconds

		std::string toString() {
			std::string result;
			result.append("[id: " + id + ", ");
			result.append("name: " + name + ", ");
			result.append("host: " + host + ", ");
			result.append("port: " + Poco::NumberFormatter::format(port) + ", ");
			result.append("acqInterval: " + Poco::NumberFormatter::format(acquisitionInterval) + "]");
			return result;
		}
	};
}