#pragma once

#include "../../common/entity_station.h"

#include <string>
#include <vector>

namespace stormy {
  namespace rest {
    namespace json {

class Cookbook 
{
public:
  static std::string prepareServerInfo();
  static std::string prepareBadResponse(std::string uri);
  static std::string prepareStation(common::entity::Station station);
  static std::string prepareStations(
    std::vector<common::entity::Station> stations);

private:
  Cookbook() {};
  Cookbook(const Cookbook&) {}
  Cookbook& operator=(const Cookbook&) {}  

  static std::string wrapAsJSONString(std::string text);
};
// ~~ stormy::rest::json::Mapper
}}}