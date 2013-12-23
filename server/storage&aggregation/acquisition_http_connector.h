#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <Poco/Logger.h>
#include <Poco/Timestamp.h>

#include "../../common/Types.h"

namespace stormy {
  namespace acquisition {

class HTTPConnector
{
public:    
  HTTPConnector(const std::string host, const uint32_t port);
  ~HTTPConnector() {};

  std::string FetchDataAsStringAt(std::string resource) const;
  Stormy::StationPtrVector FetchStationsAt() const;
  Stormy::MeasurementPtrVector FetchMeasurementsForStationAt(
    std::string stationId) const;
  Stormy::MeasurementPtrVector FetchMeasurementsForStationNewerThanAt(
    std::string stationId, 
    Poco::Timestamp timestamp) const;
  Stormy::MetricsPtrVector FetchMetricsAt() const;

private:
  Poco::Logger& logger_;
  const std::string host_;
  const uint32_t port_;
};
// ~~ stormy::acquisition::HTTPConnector
}}
