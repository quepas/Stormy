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

			Meteo::Measurement* parseFromURL(std::string url);
			Meteo::Measurement* parseFromStation(Meteo::Station* station);
		private:			
			std::string pyParserModuleName;
	};
}
