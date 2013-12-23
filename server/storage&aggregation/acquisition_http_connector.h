#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <Poco/Logger.h>
#include <Poco/Timestamp.h>

#include "../../common/entity_station.h"
#include "../../common/entity_measurement.h"
#include "../../common/entity_metrics.h"

namespace stormy {
  namespace acquisition {

class HTTPConnector
{
public:    
  HTTPConnector(const std::string host, const uint32_t port);
  ~HTTPConnector() {};

  std::string FetchDataAsStringAt(std::string resource) const;
  std::vector<common::entity::Station> FetchStationsAt() const;
  std::vector<common::entity::Measurement> 
    FetchMeasurementsForStationAt(
      std::string station_uid) const;
  std::vector<common::entity::Measurement> 
    FetchMeasurementsForStationNewerThanAt(
      std::string station_uid, 
      Poco::Timestamp timestamp) const;
  std::vector<common::entity::Metrics> FetchMetricsAt() const;

private:
  Poco::Logger& logger_;
  const std::string host_;
  const uint32_t port_;
};
// ~~ stormy::acquisition::HTTPConnector
}}
