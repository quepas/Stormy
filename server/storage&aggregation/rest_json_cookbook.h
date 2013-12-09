#pragma once

#include "../../common/entity_station.h"

#include <ctime>
#include <map>
#include <string>
#include <vector>

namespace stormy {
  namespace rest {
    namespace json {

class Cookbook 
{
public:
  static std::string PrepareServerInfo();
  static std::string PrepareBadResponse(const std::string& uri);
  static std::string PrepareStation(const common::entity::Station& station);
  static std::string PrepareStations(
    const std::vector<common::entity::Station>& stations);
  static std::string PrepareTypedMeasurement(
    const std::map<std::time_t, std::string>& measurements);
  static std::string PrepareEmpty();

private:
  Cookbook() {};
  Cookbook(const Cookbook&) {}
  Cookbook& operator=(const Cookbook&) {}  

  static std::string WrapAsJSONString(const std::string& text);  
};
// ~~ stormy::rest::json::Mapper
}}}