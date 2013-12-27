#pragma once

#include <string>
#include <vector>
#include <Poco/Logger.h>

#include "../../common/entity_station.h"
#include "../../common/entity_measurement.h"

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
  std::vector<common::entity::Measurement> 
    ParseFromURL(std::string url);

  std::string parser_class_;
  Poco::Logger& logger_;
};
// ~~ stormy::py::Parser
}}
