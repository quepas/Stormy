#pragma once

#include <string>
#include <vector>
#include "../../common/Types.h"

namespace Stormy
{
	class JSONUtils 
	{
	public:			
		static StationPtrVector extractStationsFromJSON(std::string content);
		static MeasurementPtrVector	extractMeasurementsFromJSON(std::string content);
	};
}