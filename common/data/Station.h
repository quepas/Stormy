#pragma once

#include <string>

namespace Stormy {
	namespace Data {
		struct Station
		{
			Station() {}
			Station(std::string _uid) 
				:	uid(_uid)	{}

			std::string uid;			
			std::string name;
			std::string url;
			long refresh_time;
			std::tm last_update;
			std::string parserClass;			
		};
	}
}