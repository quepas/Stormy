#pragma once

#include <string>
#include "MeteoData.h"

namespace stormy {
  namespace acquisition {
    namespace config {

class Metrics
{
public:
  Metrics(std::string filepath);
  ~Metrics();

  Stormy::TypePtrVector Configuration() {
    return configuration_;
  }

  std::string GetMetricsIdByEquivalent(std::string equivalent);
  std::string GetFirstEquivalentByMetricsId(std::string id);
  Stormy::TypePtr GetMetricsById(std::string id);

  static Stormy::TypePtr GetMetricsById(
    std::string id, 
    Stormy::TypePtrVector types);
private:
  bool Load(std::string filepath);
  Stormy::TypePtrVector configuration_;
};
// ~~ stormy::acquisition::config::Metrics
}}}
