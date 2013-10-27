#pragma once

#include <string>

namespace Stormy
{
	template<typename Out, typename In>
	Out lexical_cast(In in)
	{
		std::stringstream ss;
		Out out;

		ss << in;
		ss >> out;

		return out;
	};

	class Utils
	{
		public:
			static double stringToDouble(std::string number);
			static bool isNumber(std::string number);
			static std::string md5(std::string text);			
	};
}