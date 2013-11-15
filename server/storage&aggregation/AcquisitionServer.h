#pragma once

#include <string>
#include "../../common/Utils.h"

namespace Stormy
{
	struct AcquisitionServer
	{
		std::string id;
		std::string name;
		std::string host;
		unsigned int port;

		std::string toString() {
			std::string result;
			result.append("[id: " + id + ", ");
			result.append("name: " + name + ", ");
			result.append("host: " + host + ", ");
			result.append("port: " + lexical_cast<std::string>(port) + "]");
			return result;
		}
	};
}