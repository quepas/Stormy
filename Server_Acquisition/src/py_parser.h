#pragma once

#include <string>
#include <vector>
#include <Poco/Logger.h>

#include "acquisition_config_metrics.h"
#include "entity_station.h"
#include "entity_measurement.h"

namespace stormy {
  namespace py {

class Parser
{
public:
  Parser(std::string parser_class);
  ~Parser();

  std::vector<common::entity::Measurement> 
    ParseFromStation(common::entity::Station station);
private:
  acquisition::config::Metrics metrics_config_;
  std::string parser_class_;
  Poco::Logger& logger_;
};
// ~~ stormy::py::Parser
}}
