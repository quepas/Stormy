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
  static std::string FetchDataAsStringAt
    (std::string host, uint32_t port, std::string resource);
  static Stormy::StationPtrVector FetchStationsAt(
    std::string host, uint32_t port);
  static Stormy::MeasurementPtrVector FetchMeasurementsForStationAt(
    std::string host, uint32_t port, std::string stationId);
  static Stormy::MeasurementPtrVector FetchMeasurementsForStationNewerThanAt(
    std::string host, uint32_t port, std::string stationId, 
    Poco::Timestamp timestamp);
  static Stormy::MetricsPtrVector FetchMetricsAt(
    std::string host, uint32_t port);

private:
  static Poco::Logger& logger_;

  HTTPConnector() {};
  ~HTTPConnector() {};
};
// ~~ stormy::acquisition::HTTPConnector
}}
