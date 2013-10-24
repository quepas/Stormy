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
			static double stringToDouble(std::string);
	};
}