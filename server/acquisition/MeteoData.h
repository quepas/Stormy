#pragma once

#include <map>
#include <vector>
#include <string>

#include <Poco/Timestamp.h>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>

#include "MeteoConst.h"

namespace Stormy
{
	namespace Meteo
	{
		struct Station
		{
			std::string stationId;
			std::string url;
			std::string name;
			std::string parserClass;
			long refreshTime;
		};

		struct Measurement
		{
			Measurement()
				:	timestamp(),
					station(new Station()),
					data()	{}

			Measurement(std::string reason)
				:	timestamp(),
					station(new Station()),
					data() {
				data[Const::reasonKey] = reason;
			}

			Poco::Timestamp timestamp;
			Station* station;
			std::map<std::string, boost::any> data;
		};

		struct Type
		{
			std::string id;
			std::string valueType;
			std::string valueUnit;
			std::string valueFormat;
			std::vector<std::string> equivalents;
			bool isMeteo;
		};
	}

	typedef std::shared_ptr<Meteo::Measurement> MeasurementPtr;
	typedef std::vector<MeasurementPtr> MeasurementPtrVector;

	typedef std::shared_ptr<Meteo::Station> StationPtr;
	typedef std::vector<StationPtr> StationPtrVector;

	typedef std::shared_ptr<Meteo::Type> TypePtr;
	typedef std::vector<TypePtr> TypePtrVector;
}