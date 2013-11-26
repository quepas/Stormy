#pragma once

#include <vector>

#include "data/Measurement.h"
#include "data/Station.h"
#include "data/Metrics.h"

namespace Stormy
{	
	typedef std::shared_ptr<Data::Measurement> MeasurementPtr;
	typedef std::vector<MeasurementPtr> MeasurementPtrVector;

	typedef std::shared_ptr<Data::Station> StationPtr;
	typedef std::vector<StationPtr> StationPtrVector;

	typedef std::shared_ptr<Data::Metrics> MetricsPtr;
	typedef std::vector<MetricsPtr> MetricsPtrVector;

	typedef unsigned int uint32;
	typedef unsigned short int uint16;
	typedef unsigned long ulong;
}