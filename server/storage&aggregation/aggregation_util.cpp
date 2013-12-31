#include "aggregation_util.h"

using namespace stormy::common;

using std::map;
using std::string;
using std::time_t;
using std::vector;

namespace stormy {
  namespace aggregation {
    namespace util {

map<string, vector<entity::Measurement>> 
  ConvertMeasureSetsToMetricsSets(
    const map<time_t, vector<entity::Measurement>>& measure_sets)
{
  map<string, vector<entity::Measurement>> metrics_sets;
  for (auto ms_it = measure_sets.begin(); ms_it != measure_sets.end(); ++ms_it) {
    auto measure_set = ms_it->second;
    for (auto it = measure_set.begin(); it != measure_set.end(); ++it) {
      metrics_sets[it->code].push_back(*it);
    }
  }
  return metrics_sets;
}
// ~~ stormy::aggregation::util
}}}
