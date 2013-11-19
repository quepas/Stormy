#pragma once

#include <string>

namespace Stormy {
	namespace Data {
		struct Station
		{
			std::string uid;			
			std::string name;
			std::string url;
			long refreshTime;
			std::string parserClass;			
		};
	}
}