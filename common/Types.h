#pragma once

#include "data/Measurement.h"
#include "data/Station.h"
#include "data/Metrics.h"

namespace Stormy
{	
	typedef std::vector<std::shared_ptr<Data::Measurement>> MeasurementPtrVector;
	typedef std::vector<std::shared_ptr<Data::Station>> StationPtrVector;
	typedef std::vector<std::shared_ptr<Data::Metrics>> MetricsPtrVector;

	typedef std::shared_ptr<Data::Measurement> MeasurementPtr;
	typedef std::shared_ptr<Data::Station> StationPtr;
	typedef std::shared_ptr<Data::Metrics> MetricsPtr;

	typedef unsigned int uint32;	
}