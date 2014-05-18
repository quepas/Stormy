#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <Poco/Logger.h>

namespace stormy {
  namespace common {
    namespace yaml {

class BaseConfig
{
public:
  BaseConfig(std::string path);
  ~BaseConfig();

  bool HasField(std::string field, uint16_t index = 0);
  std::vector<std::string> GetFields(uint16_t index = 0);
  uint16_t Size();

  std::string AsString(std::string field, uint16_t index = 0);
  int AsInt(std::string field, uint16_t index = 0);
  double AsDouble(std::string field, uint16_t index = 0);
  bool AsBool(std::string field, uint16_t index = 0);

protected:
  Poco::Logger& logger_;

private:
  std::vector<std::map<std::string, std::string>> data_;

  void OpenFromFile(std::string path);
  void ReadAndAddEntry(YAML::Node entry);
  bool CheckIndex(uint16_t index);
  std::string GetValue(std::string key, uint16_t index = 0);
};
// ~~ stormy::common::yaml::Config
}}}