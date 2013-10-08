#pragma once

#include <string>

#include "../../common/MeteoData.h"

namespace Stormy
{
	class PyParserWrapper
	{
		public:
			PyParserWrapper(std::string _pyParserModuleName);
			~PyParserWrapper();

			MeteoData* parseFromURL(std::string url);
			MeteoData* parseFromText(std::wstring text);

			MeteoData* getData();
		private:
			MeteoData* data;
			std::string pyParserModuleName;
	};
}
