#pragma once

#include <Poco/NumberFormatter.h>
#include "../../common/Types.h"

namespace Stormy
{
	struct AggregationSettings
	{
		std::string name;		
		std::string interval;	// postgresql specifed
		uint32 taskRefresh;		// in minutes

		std::string toString() {
			std::string result;
			result.append("[name: " + name + ", ");
			result.append("interval: " + interval + ", ");
			result.append("taskRefresh: " + 
				Poco::NumberFormatter::format(taskRefresh) + "]");
			return result;
		}
	};
}