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

			std::vector<Type*> getConfiguration() {
				return types;
			}

			std::string getTypeIdByEquivalent(std::string equivalent);
			std::string getFirstEquivalentById(std::string id);
			Type* getFullTypeById(std::string id);
		private:
			bool load(std::string filepath);

			std::vector<Type*> types;
		};
	}	
}