#pragma once

#include <ctime>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include <Poco/Logger.h>

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
  std::vector<common::entity::Station> FetchStations() const;
  std::map<std::time_t, std::vector<common::entity::Measurement>>
    FetchMeasureSets(std::string station_uid, std::time_t from_time) const;  
  std::vector<common::entity::Metrics> FetchMetrics() const;

private:
  Poco::Logger& logger_;
  const std::string host_;
  const uint32_t port_;
};
// ~~ stormy::acquisition::HTTPConnector
}}
