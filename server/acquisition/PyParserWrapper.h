#pragma once

#include <string>
#include "MeteoData.h"

namespace Stormy
{
	class PyParserWrapper
	{
		public:
			PyParserWrapper(std::string _pyParserModuleName);
			~PyParserWrapper();

			MeasurementPtr parseFromStation(StationPtr station);
		private:
			MeasurementPtr parseFromURL(std::string url);

			std::string pyParserModuleName;
	};
}
