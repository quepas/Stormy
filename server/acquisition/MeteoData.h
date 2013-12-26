#pragma once

#include <map>
#include <vector>
#include <string>

#include <Poco/Timestamp.h>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>

#include "acquisition_constant.h"

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
				data[stormy::acquisition::constant::reasonKey] = reason;
			}

			~Measurement() {
				delete station;
			}

			Poco::Timestamp timestamp;
			Station* station;
			std::map<std::string, boost::any> data;
		};
	}

	typedef std::shared_ptr<Meteo::Measurement> MeasurementPtr;
	typedef std::vector<MeasurementPtr> MeasurementPtrVector;
}