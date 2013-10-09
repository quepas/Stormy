#pragma once

#include <string>

namespace Stormy
{
	struct MeteoStation
	{		
		// generated if not passed md5(url+parserClass)
		std::string id;		
		// required
		std::string url;
		// optional
		std::string name;
		// required
		std::string parserClass;	
		//required (in seconds)
		double refreshTime;		
	};
}