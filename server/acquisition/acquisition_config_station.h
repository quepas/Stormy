#pragma once

#include "yaml-cpp/yaml.h"

#include "MeteoData.h"

namespace stormy {
  namespace acquisition {
    namespace config {

class Station
{
public:
  Station(std::string filepath);
  ~Station();

  Stormy::StationPtrVector getConfiguration() { 
    return configuration_; 
  }

private:
  bool load(std::string filepath);
  Stormy::StationPtrVector configuration_;
};
// ~~ stormy::acquisition::config::Station
}}}
