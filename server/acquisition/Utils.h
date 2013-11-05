#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/any.hpp>

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
			static bool checkIfHexMD5(std::string text);
			static std::string extractMD5FromText(std::string text);
			static std::string extractEndIdFromRestURI(std::string URI);
			static std::string getStringFromAny(boost::any any);

			// date & time
			static bool checkIfStandardDate(std::string date);	// YYYY-MM-DD
			static bool checkIfStandardTime(std::string time);	// HH:MM

			// stl
			template<typename T, typename V>
			static void forEach(const std::vector<T>& data, V func);
			template<typename T, typename U, typename V>
			static void forEach(const std::map<T, U>& data, V func);
			template<typename T, typename V>
			static T getLast(V seq);

			// const
			static std::string numberPattern;
			static std::string standardDatePattern;
			static std::string standardTimePattern;
			static std::string hexMD5Pattern;
		public:
			static bool checkTextWithRegex(std::string text, std::string regex);
	};

	template<typename T, typename V>
	void Utils::forEach( const std::vector<T>& data, V func )
	{
		std::for_each(data.begin(), data.end(), func);
	}

	template<typename T, typename U, typename V>
	void Stormy::Utils::forEach( const std::map<T, U>& data, V func )
	{
		std::for_each(data.begin(), data.end(), func);
	}
	
	template<typename T, typename V>
	T Stormy::Utils::getLast( V seq )
	{
		if(seq.empty()) return T();
		return *(--seq.end());
	}

}