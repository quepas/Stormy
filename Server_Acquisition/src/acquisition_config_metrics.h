#pragma once

#include "entity_metrics.h"

#include <string>
#include <vector>

namespace stormy {
  namespace acquisition {
    namespace config {

class Metrics
{
public:
  Metrics(std::string filepath);
  ~Metrics();

  std::vector<common::entity::Metrics> Configuration() {
    return configuration_;
  }

  std::string GetMetricsIdByEquivalent(std::string equivalent);
  std::string GetFirstEquivalentByMetricsId(std::string id);
  common::entity::Metrics GetMetricsById(std::string id);

  static common::entity::Metrics GetMetricsById(
    std::string id, 
    std::vector<common::entity::Metrics> types);
private:
  bool Load(std::string filepath);
  std::vector<common::entity::Metrics> configuration_;
};
// ~~ stormy::acquisition::config::Metrics
}}}
