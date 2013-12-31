#include "rest_csv_cookbook.h"

#include "../../common/rest_constant.h"

#include <boost/range/algorithm/copy.hpp>
#include <boost/algorithm/string/join.hpp>
#include <Poco/NumberFormatter.h>

using namespace stormy::common;
using namespace stormy::common::rest;

using boost::join;
using boost::copy;
using std::back_inserter;
using std::string;
using std::map;
using std::vector;
using Poco::NumberFormatter;

namespace stormy {
  namespace rest {
    namespace csv {

string PrepareMeteo(
  const map<time_t, vector<entity::Measurement>>& measure_sets, 
  const vector<string>& metrics)
{
  vector<string> rows;
  rows.push_back(PrepareHeader(metrics));
  for (auto ms_it = measure_sets.begin(); ms_it != measure_sets.end(); ++ms_it) {
    vector<string> row_parts;    
    auto measure_set = ms_it->second;
    row_parts.push_back(NumberFormatter::format(ms_it->first));

    for (auto m_it = metrics.begin(); m_it != metrics.end(); ++m_it) {
      bool inserted = false;
      for (auto it = measure_set.begin(); it != measure_set.end(); ++it) {
        if (it->code == *m_it) {
          if(it->is_numeric) 
            row_parts.push_back(NumberFormatter::format(it->value_number));
          else 
            row_parts.push_back(it->value_text);          
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

// ~~ stormy::rest::csv
}}}
