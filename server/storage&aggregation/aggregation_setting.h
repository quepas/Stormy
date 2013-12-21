#pragma once

#include <Poco/NumberFormatter.h>
#include "../../common/Types.h"
#include "../../common/Utils.h"

namespace Stormy
{
	struct AggregationSetting
	{
		std::string name;		
		std::string interval;	// postgresql specifed
		uint32 taskRefresh;		// in minutes
		bool turnOn;

		std::string toString() {
			std::string result;
			result.append("[name: " + name + ", ");
			result.append("interval: " + interval + ", ");
			result.append("taskRefresh: " + 
				Poco::NumberFormatter::format(taskRefresh) + ", ");
			result.append("turnOn: " + 
				lexical_cast<std::string>(turnOn) + "]");
			return result;
		}
	};
}