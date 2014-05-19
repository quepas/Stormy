#pragma once

#include "entity_measurement.h"
#include "entity_metrics.h"
#include "entity_station.h"
#include "aggregation_entity_aggregate.h"

#include <vector>

namespace stormy {
  namespace replication {

struct BareBatch
{
  std::vector<long> measurement_ids;
  std::vector<long> metrics_ids;
  std::vector<long> station_ids;
  std::vector<long> aggregate_ids;
};

struct FullBatch
{
  std::vector<common::entity::Measurement> measurements;
  std::vector<common::entity::Metrics> metrics;
  std::vector<common::entity::Station> stations;
  std::vector<aggregation::entity::Aggregate> aggregates;
};

class Batch
{
public:
  template<typename T>
  void Add(const T& value);

private:
  virtual void AddMeasurement(common::entity::Measurement value);
  virtual void AddMetrics(common::entity::Metrics value);
  virtual void AddStation(common::entity::Station value);
  virtual void AddAggregate(aggregation::entity::Aggregate value);
};
// ~~ stormy::replication::Batch
}}