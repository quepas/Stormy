#pragma once

#include "data/Measurement.h"
#include "data/Station.h"

namespace Stormy
{	
	typedef std::vector<std::shared_ptr<Data::Measurement>> MeasurementPtrVector;
	typedef std::vector<std::shared_ptr<Data::Station>> StationPtrVector;

	typedef std::shared_ptr<Data::Station> StationPtr;
	typedef std::shared_ptr<Data::Measurement> MeasurementPtr;
}