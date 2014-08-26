#pragma once

#include "db_has_data_expiration.h"
#include "entity_station.h"
#include "entity_metrics.h"
#include "entity_measurement.h"
#include "settings.hpp"

#include <ctime>
#include <string>
#include <map>
#include <vector>
#include <Poco/Logger.h>
#include <Poco/MongoDB/Connection.h>
#include <Poco/MongoDB/Database.h>

namespace stormy {
  namespace db {

class MongoHandler : public common::db::HasDataExpiration
{
public:
  void ExpireData() override;
  std::vector<std::string> FetchStationsUID();

  void InsertMeasurement(std::vector<common::entity::Measurement> meteoData);

  void clearStationsData();
  void InsertStationsData(const StationSettings& stations);
  void InsertStationData(StationSetting station);
  std::vector<common::entity::Station> getStationsData();

  common::entity::Station GetStationByUID(std::string uid);
  unsigned int CountMeasureSetsForStationByUID(std::string uid);

  std::map<std::time_t, std::vector<common::entity::Measurement>> 
    GetMeasureSetsForStationBetweenTS(
      std::string station_uid, 
      std::time_t from, 
      std::time_t to);
  std::map<std::time_t, std::vector<common::entity::Measurement>> 
    GetMeasureSetsForStationAndTS(
      std::string station_uid, 
      std::time_t ts);
  std::map<std::time_t, std::vector<common::entity::Measurement>>
    GetAllMeasureSetsForStation(std::string station_uid);

  bool RemoveMetrics();
  bool InsertMetrics(const MetricsSettings& metrics);
  std::vector<common::entity::Metrics> GetMetrics();

  const std::string& db_name() const { return db_name_; }
  bool is_connected() const { return is_connected_; }

  static MongoHandler& get() {
    static MongoHandler instance;
    return instance;
  }
private:
  MongoHandler(std::string db_address = "localhost", unsigned int port = 27017);
  MongoHandler(const MongoHandler&)
    : logger_(Poco::Logger::get("db/MongoHandler")) {}
  MongoHandler& operator=(const MongoHandler&) {}
  ~MongoHandler();

  Poco::MongoDB::Connection* connection_;
  Poco::MongoDB::Database* database_;
  std::string db_name_;
  bool is_connected_;
  void Connect(std::string db_name, std::string db_address, unsigned int port);
  void CheckLastErrors();
  Poco::Logger& logger_;
};
// ~~ stormy::db::MongoHandler
}}