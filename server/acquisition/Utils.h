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

			// date & time
			static bool checkIfStandardDate(std::string date);	// YYYY-MM-DD
			static bool checkIfStandardTime(std::string time);	// HH:MM
		public:
			static bool checkTextWithRegex(std::string text, std::string regex);
	};
}