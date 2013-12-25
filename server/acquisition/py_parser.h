#pragma once

#include <string>
#include <Poco/Logger.h>

#include "../../common/entity_station.h"
#include "MeteoData.h"

namespace stormy {
  namespace py {

class Parser
{
public:
  Parser(std::string parser_class);
  ~Parser();

  Stormy::MeasurementPtr ParseFromStation(common::entity::Station station);
private:
  Stormy::MeasurementPtr ParseFromURL(std::string url);

  std::string parser_class_;
  Poco::Logger& logger_;
};
// ~~ stormy::py::Parser
}}
