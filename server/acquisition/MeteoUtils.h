#pragma once

#include <string>

namespace Stormy
{
	class MeteoUtils
	{
		public:
			static double extractTemperature(std::string text);			
			static long convertMinutesToMiliseconds(long minutes);
			static long convertSecondsToMiliseconds(long seconds);
	};
}