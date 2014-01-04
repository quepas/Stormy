#pragma once

#include <string>
#include <cstdint>
#include <soci.h>
#include <Poco/Logger.h>

#include "../../common/db_setting.h"
#include "../../common/util.h"

#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"
#include "../../common/entity_measurement.h"

namespace stormy {
  namespace db {
    
class Storage
{
public:
  Storage(common::db::Setting storage_setting);
  ~Storage();   

  std::tm GetOldestStationMeasureTime(std::string uid);  
  
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
  uint32_t CountAllStations();

  std::vector<common::entity::Station> GetStations();    
  std::vector<common::entity::Metrics> GetMetrics();
  std::vector<std::string> GetMetricsCodes();

  std::tm GetStationLastUpdate(std::string station_uid);
  void UpdateStationLastUpdateIfNeeded(
    std::string station_uid, 
    std::tm last_update);

  // stations					 
  void insertStation(common::entity::Station station);
  void insertStations(const std::vector<common::entity::Station>& stations);
  bool existsStationByUID(std::string uid);
  common::entity::Station GetStationByUID(std::string uid);								

  // measurements			
  std::tm findNewestMeasureTimeByStationUID(std::string uid);

  // metrics
  bool insertOneMetrics(common::entity::Metrics metrics);
  bool insertMetrics(const std::vector<common::entity::Metrics>& metrics);
  bool existsMetricsByCode(const std::string& code);
	
  common::db::Setting Configuration() {
    return configuration_;
  }
private:
  void connect();

  common::db::Setting configuration_;
  soci::session sql_;
  Poco::Logger& logger_;
};
// ~~ stormy::db::Storage
}}
