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

		std::string formatData() {
			std::string result;
			result.append("\tid: " + id);
			result.append("\n\tname: " + name);
			result.append("\n\thost: " + host);
			result.append("\n\tport: " + lexical_cast<std::string>(port));
			return result;
		}
	};
}