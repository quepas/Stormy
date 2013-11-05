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

			Meteo::Measurement* parseFromStation(Meteo::Station* station);
		private:
			Meteo::Measurement* parseFromURL(std::string url);

			std::string pyParserModuleName;
	};
}
