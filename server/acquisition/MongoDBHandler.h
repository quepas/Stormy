#pragma once

#include <string>
#include "../../common/MeteoData.h"

namespace Stormy
{
	class MongoDBHandler
	{
		public:
			MongoDBHandler(std::string dbAddress = "");
			~MongoDBHandler();

			void connect(std::string dbAddress);
			void insertMeteoData(MeteoData* meteoData);

		private:
			bool connected;			
	};
}