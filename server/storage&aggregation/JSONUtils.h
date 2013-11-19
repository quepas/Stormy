#pragma once

#include <string>
#include <vector>
#include "../../common/data/Station.h"
#include "../../common/data/Measurement.h"

namespace Stormy
{
	class JSONUtils
	{
		public:			
			static std::vector<std::shared_ptr<Data::Station>> 
				extractStationsFromJSON(std::string content);
			static std::vector<std::shared_ptr<Data::Measurement>>
				extractMeasurementsFromJSON(std::string content);
	};
}