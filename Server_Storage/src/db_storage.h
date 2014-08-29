#pragma once

#include <string>
#include <cstdint>
#include <soci.h>
#include <Poco/Logger.h>

#include "settings.hpp"
#include "common/util.h"
#include "common/entity_station.h"
#include "common/entity_metrics.h"
#include "common/entity_measurement.h"

namespace stormy {
  namespace db {
    
class Storage
{
public:
  Storage(DatabaseSetting setting);
  ~Storage();

  std::tm GetOldestStationMeasureTime(std::string uid);
  std::tm GetNewestStationMeasureTime(std::string uid);
  
  std::vector<std::tm> SelectDistinctMeasureTSForStationBetweenTS(
    std::string station_uid,
    std::tm from,
    std::tm to);

  std::map<std::time_t, std::vector<common::entity::Measurement>>
    GetMeasureSetsForStationBetweenTS(
      std::string station_uid,
      std::time_t from,
      std::time_t to);
  std::map<std::time_t, std::vector<common::entity::Measurement>>
    GetMeasureSetsForStationAndTS(
      std::string station_uid, 
      std::time_t time);
  std::map<std::time_t, std::vector<common::entity::Measurement>>
    GetAllMeasureSetsForStation(
      std::string station_uid);

  void InsertMeasureSets(
    const std::map<std::time_t, std::vector<common::entity::Measurement>>& measure_sets);
  void InsertMeasureAsText(
    const common::entity::Measurement& measure);
  void InsertMeasureAsNumeric(
    const common::entity::Measurement& measure);
  uint64_t CountAllMeasurements();
  uint64_t CountStationMeasurements(std::string uid);
  

	std::vector<common::entity::Station> GetStations();
  uint32_t CountAllStations();
  void InsertStation(common::entity::Station station);
  void InsertStations(const std::vector<common::entity::Station>& stations);
  bool CheckIfStationExsist(std::string uid);
  common::entity::Station GetStationByUID(std::string uid);
  std::tm GetStationLastUpdate(std::string station_uid);

  void UpdateStationLastUpdateIfNeeded(
    std::string station_uid, 
    std::tm last_update);

  std::vector<common::entity::Metrics> GetMetrics();
  std::vector<std::string> GetMetricsCodes();
  bool InsertSingleMetrics(common::entity::Metrics metrics);
  bool InsertMetrics(const std::vector<common::entity::Metrics>& metrics);
  bool CheckIfMetricsExsist(const std::string& code);

private:
  void connect();

  DatabaseSetting setting_;
  soci::session sql_;
  Poco::Logger& logger_;
};
// ~~ stormy::db::Storage
}}
