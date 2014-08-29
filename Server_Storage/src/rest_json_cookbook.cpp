#include "rest_json_cookbook.h"
#include "common/rest_cookbook.h"
#include "common/rest_constant.h"

#include <json/json.h>

using namespace stormy::common;
using namespace stormy::common::rest;

using std::string;
using std::map;
using std::vector;

namespace stormy {
  namespace rest {
    namespace json {

string Cookbook::PrepareAggregateSets(
  const map<time_t, vector<aggregation::entity::Aggregate>>& aggregate_sets)
{
  return constant::JSON_EMPTY;
}

string Cookbook::PreparePeriods(
  const vector<aggregation::entity::Period>& periods)
{
  Json::Value root;
  for (unsigned idx = 0; idx < periods.size(); ++idx) {
    root[constant::PERIODS][idx] = periods[idx].name;
  }
  return cookbook::ToStyledString(root);
}

string Cookbook::PrepareOperations(
  const vector<aggregation::entity::Operation>& operations)
{
  Json::Value root;
  for (unsigned idx = 0; idx < operations.size(); ++idx) {
    root[constant::OPERATIONS][idx] = operations[idx].name;
  }
  return cookbook::ToStyledString(root);
}

}}}
// ~~ stormy::rest::json::Cookbook
