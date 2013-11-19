#pragma once

#include <string>

namespace Stormy {
	namespace Data {
		struct Metrics
		{
			Metrics()	{}
			Metrics::Metrics(std::string _code)
				:	code(_code)	{}

			std::string code;
			std::string equivalents;
			std::string value_type;
			std::string value_unit;
			std::string value_format;
		};
	}
}