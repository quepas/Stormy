#pragma once

#include <vector>
#include "../../common/entity_station.h"

namespace stormy {
  namespace acquisition {
    namespace config {

class Station
{
public:
  Station(std::string filepath);
  ~Station();

  std::vector<common::entity::Station> getConfiguration() { 
    return configuration_; 
  }

private:
  bool load(std::string filepath);
  std::vector<common::entity::Station> configuration_;
};
// ~~ stormy::acquisition::config::Station
}}}
