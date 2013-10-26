#pragma once

#include <string>
#include "MeteoData.h"

namespace Stormy
{
	namespace Meteo
	{
		class TypeConfiguration
		{
		public:
			TypeConfiguration(std::string filepath);
			~TypeConfiguration();

			std::string getTypeIdByEquivalent(std::string equivalent);
			Type* getFullTypeById(std::string id);
		private:
			bool load(std::string filepath);

			std::vector<Type*> types;
		};
	}	
}