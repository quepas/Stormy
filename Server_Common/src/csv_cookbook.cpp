#include "common/csv_cookbook.hpp"
#include "common/rest_constant.h"

#include <boost/range/algorithm/copy.hpp>
#include <boost/algorithm/string/join.hpp>

using namespace stormy::common;
using namespace stormy::common::rest;

using boost::join;
using boost::copy;
using std::back_inserter;
using std::string;
using std::to_string;
using std::map;
using std::vector;

namespace stormy {
    namespace csv {

string PrepareMeteo(
  const map<time_t, vector<entity::Measurement>>& measure_sets,
  const vector<string>& metrics)
{
  vector<string> rows;
  rows.push_back(PrepareHeader(metrics));
  for (auto& ms_entry : measure_sets) {
    vector<string> row_parts;
    auto measure_set = ms_entry.second;
    row_parts.push_back(to_string(ms_entry.first));

    for (auto& metrics_item : metrics) {
      bool inserted = false;
      for (auto& meteo_element : measure_set) {
        if (meteo_element.code == metrics_item) {
          if (meteo_element.is_numeric)
            row_parts.push_back(to_string(meteo_element.value_number));
          else
            row_parts.push_back(meteo_element.value_text);
          inserted = true;
          break;
        }
      }
      if (!inserted)
        row_parts.push_back(constant::csv_missing_value);
    }
    rows.push_back(join(row_parts, constant::csv_delimiter));
  }
  return join(rows, constant::csv_new_line);
}

string PrepareHeader(const vector<string>& metrics)
{
  string header = constant::csv_timestamp;
  if (!metrics.empty()) {
    vector<string> metrics_plus_ts;
    metrics_plus_ts.push_back(header);
    copy(metrics, back_inserter(metrics_plus_ts));
    header = join(metrics_plus_ts, constant::csv_delimiter);
  }
  return header;
}

}}
// ~~ stormy::csv::Cookbook
