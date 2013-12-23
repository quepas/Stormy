#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/any.hpp>
#include <sstream>

#define TRY try {
#define CATCH                   \
  } catch(const std::exception& ex){ \
    logger_.error(ex.what());   \
  }
#define CATCH_MSG(msg)                                  \
  } catch(const std::exception& ex) {                        \
    logger_.error(std::string(msg) + ": " + ex.what()); \
  }

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
			static bool asBool(std::string text);

			// date & time
			static bool checkIfStandardDate(std::string date);	// YYYY-MM-DD
			static bool checkIfStandardTime(std::string time);	// HH:MM
      static std::time_t LocaltimeNow();

			// stl
			template<typename T, typename V>
			static void forEach(const T& data, V func);			
			template<typename T, typename V>
			static T getLast(V seq);
			template<typename T, typename V>
			static bool contains(T seq, V key);

			// const
			static std::string numberPattern;
			static std::string standardDatePattern;
			static std::string standardTimePattern;
			static std::string hexMD5Pattern;
		public:
			static bool checkTextWithRegex(std::string text, std::string regex);
	};

	template<typename T, typename V>
	bool Utils::contains( T seq, V key )
	{
		return find(seq.begin(), seq.end(), key) != seq.end();
	}

	template<typename T, typename V>
	void Utils::forEach( const T& data, V func )
	{
		std::for_each(data.begin(), data.end(), func);
	}

	template<typename T, typename V>
	T Utils::getLast( V seq )
	{
		if(seq.empty()) return T();
		return *(--seq.end());
	}

}