#pragma once

#include <string>
#include <cstdint>
#include <soci.h>
#include <Poco/Timestamp.h>
#include <Poco/Logger.h>

#include "../../common/db_setting.h"
#include "../../common/util.h"

#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"
#include "../../common/entity_measurement.h"

#include "aggregation_entity_task.h"
#include "aggregation_entity_period.h"

namespace stormy {
  namespace db {
    
class Storage
{
public:
  Storage(common::db::Setting* storageDB);
  ~Storage();

  bool DeleteTask(uint16_t id);
  bool DeleteTask(std::string period_name, std::string station_uid);
  bool CreateTask(std::string period_name, std::string station_uid);
  bool UpdateTaskCurrentTime(uint32_t id, std::tm timestamp);
  std::tm CalculateAggregateEndTime(std::string period_name, std::tm start_time);

  bool UpdateStationLastUpdate(std::string station_uid, std::tm timestamp);
  std::tm GetStationLastUpdate(std::string station_uid);

  std::tm GetOldestStationMeasureTime(std::string uid);

  // TODO: fix this! need entity::Measurement
  std::vector<std::string> GetStationMeasure(
    std::string station_uid, 
    std::string metrics_code, 
    std::tm begin_time, 
    std::tm end_time);

  std::map<std::time_t, std::string> GetMeasurement(
    std::string station_uid, 
    std::string metrics_code,
    std::tm begin, 
    std::tm end);

  std::map<std::time_t, std::string> GetMeasurementFromLast(
    std::string station_uid, 
    std::string metrics_code, 
    uint16_t from_last_hours);
  
  void InsertMeasureSets(
    const std::map<std::time_t, std::vector<common::entity::Measurement>>& measure_sets);
  void InsertMeasureAsText(
    const common::entity::Measurement& measure);
  void InsertMeasureAsNumeric(
    const common::entity::Measurement& measure);
  uint64_t CountAllMeasurements();
  uint64_t CountStationMeasurements(std::string uid);
  uint32_t CountAllStations();

  std::vector<common::entity::Station> GetStations();  
  std::vector<aggregation::entity::Task> GetTasks();
  std::vector<aggregation::entity::Period> GetPeriods();   
  std::vector<common::entity::Metrics> GetMetrics();
  std::vector<std::string> GetMetricsCodes();

  void UpdateStationLastUpdateIfNeeded(
    std::string station_uid, 
    std::tm last_update);

  // stations					 
  void insertStation(common::entity::Station station);
  void insertStations(const std::vector<common::entity::Station>& stations);
  bool existsStationByUID(std::string uid);
  common::entity::Station getStationByUID(std::string uid);								

  // measurements			
  std::tm findNewestMeasureTimeByStationUID(std::string uid);			
  Poco::Timestamp findOldestMeasureTimeByStationUID(std::string uid);    

  // metrics
  bool insertOneMetrics(common::entity::Metrics metrics);
  bool insertMetrics(const std::vector<common::entity::Metrics>& metrics);
  bool existsMetricsByCode(const std::string& code);
		
private:
  void connect();

  common::db::Setting* configuration;
  soci::session sql;
  Poco::Logger& logger_;
};
// ~~ stormy::db::Storage
}}
