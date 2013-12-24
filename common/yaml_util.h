#pragma once

#include <algorithm>
#include <string>
#include <yaml-cpp/yaml.h>

namespace stormy {
  namespace common {
    namespace yaml {

bool IsDefined(YAML::iterator it, std::string field);
std::string GetString(YAML::iterator it, std::string field);
double GetNumber(YAML::iterator it, std::string field);
long GetLongNumber(YAML::iterator it, std::string field);
bool GetBool(YAML::iterator it, std::string field);

template<typename T>
  static void Each(YAML::Node& node, T func) {
    std::for_each(node.begin(), node.end(), func);
  }
// ~~ stormy::common::yaml::util
}}}
