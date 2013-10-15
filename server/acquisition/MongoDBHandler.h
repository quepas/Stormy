#pragma once

#include <string>

namespace Stormy
{
	class MongoDBHandler
	{
		public:
			MongoDBHandler(std::string dbAddress = "");
			~MongoDBHandler();

			void connect(std::string dbAddress);

		private:
			bool connected;			
	};
}