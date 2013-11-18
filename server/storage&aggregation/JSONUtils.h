#pragma once

#include <string>
#include <vector>
#include "../../common/data/Station.h"

namespace Stormy
{
	class JSONUtils
	{
		public:
			static std::vector<Data::Station*> extractStationsFromJSON(std::string content);
	};
}