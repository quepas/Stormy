#pragma once

#include <string>

namespace Stormy
{
	namespace Data
	{
		struct Station
		{
			std::string id;
			std::string url;
			std::string name;
			std::string parserClass;
			long refreshTime;
		};
	}
}