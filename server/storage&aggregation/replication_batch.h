#pragma once

#include "../../common/entity_measurement.h"
#include "../../common/entity_metrics.h"
#include "../../common/entity_station.h"
#include "aggregation_entity_aggregate.h"

#include <vector>

namespace stormy {
  namespace replication {

struct DevoidBatch
{
  std::vector<long> measurement_ids;
  std::vector<long> metrics_ids;
  std::vector<long> station_ids;
  std::vector<long> aggregate_ids;
};

struct Batch
{
  std::vector<common::entity::Measurement> measurements;
  std::vector<common::entity::Metrics> metrics;
  std::vector<common::entity::Station> stations;
  std::vector<aggregation::entity::Aggregate> aggregates;
};
// ~~ stormy::replication::Batch
}}