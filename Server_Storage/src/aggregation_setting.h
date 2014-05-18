#pragma once

#include <cstdint>
#include <Poco/NumberFormatter.h>
#include <boost/lexical_cast.hpp>

namespace stormy {
  namespace aggregation {

struct Setting
{
	std::string name;		
	std::string interval;	// postgresql specifed
	uint32_t task_refresh;		// in minutes
	bool turn_on;

	std::string ToString() {
		std::string result;
		result.append("[name: " + name + ", ");
		result.append("interval: " + interval + ", ");
		result.append("task_refresh: " + 
  		Poco::NumberFormatter::format(task_refresh) + ", ");
		result.append("turn_on: " + 
			boost::lexical_cast<std::string>(turn_on) + "]");
		return result;
	}
};
// ~~ stormy::aggregation::Setting
}}
