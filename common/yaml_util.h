#pragma once

#include <algorithm>
#include <string>
#include <yaml-cpp/yaml.h>

namespace stormy {
  namespace common {
    namespace yaml {

class Util
{
public:
  static bool IsDefined(YAML::iterator it, std::string field);
  static std::string GetString(YAML::iterator it, std::string field);
  static double GetNumber(YAML::iterator it, std::string field);
  static long GetLongNumber(YAML::iterator it, std::string field);
  static bool GetBool(YAML::iterator it, std::string field);

  template<typename T>
  static void forEach(YAML::Node& node, T func) {
    std::for_each(node.begin(), node.end(), func);
  }
};
// ~~ stormy::common::yaml::Util
}}}
