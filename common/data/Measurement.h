#pragma once

#include <string>
#include <boost/any.hpp>
#include <Poco/Timestamp.h>

#include "Metrics.h"
#include "Station.h"

namespace Stormy {
	namespace Data {
		struct Measurement
		{			
			Measurement() 
				:	metrics(new Metrics()), station(nullptr),
					timestamp(), value()	{}
			~Measurement() {
				delete metrics;
				delete station;
			}

			Poco::Timestamp timestamp;
			Metrics* metrics;
			Station* station;
			boost::any value;
		};
	}
}