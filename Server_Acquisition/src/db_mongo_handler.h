#pragma once

#include <ctime>
#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <Poco/Logger.h>

#include <Poco/MongoDB/Connection.h>
#include <Poco/MongoDB/Database.h>

#include "db_handler.h"
#include "db_has_data_expiration.h"
#include "entity_station.h"
#include "entity_metrics.h"
#include "entity_measurement.h"

namespace stormy {
  namespace db {

class MongoHandler : public common::db::Handler, public common::db::HasDataExpiration
{
public:
  void ExpireData() override;
  std::vector<std::string> FetchStationsUID();

  void InsertMeasurement(std::vector<common::entity::Measurement> meteoData);

  void clearStationsData();
  void insertStationsData(const std::vector<common::entity::Station>& stations);
  void insertStationData(common::entity::Station station);
  std::vector<common::entity::Station> getStationsData();

  common::entity::Station GetStationByUID(std::string uid);
  uint32_t CountMeasureSetsForStationByUID(std::string uid);

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
  bool InsertMetrics(
    const std::vector<common::entity::Metrics>& metrics_vec);
  std::vector<common::entity::Metrics> GetMetrics();

  static MongoHandler& get() {
    static MongoHandler instance;
    return instance;
  }
private:
  MongoHandler(std::string db_address = "localhost", unsigned int port = 27017);
  MongoHandler(const MongoHandler&)
    : Handler("MongoDB"), 
      logger_(Poco::Logger::get("db/MongoHandler")) {}
  MongoHandler& operator=(const MongoHandler&) {}
  ~MongoHandler();

  Poco::MongoDB::Connection* connection_;
  Poco::MongoDB::Database* database_;
  std::string db_name_;
  void Connect(std::string db_name, std::string db_address, unsigned int port);
  void CheckLastErrors();
  Poco::Logger& logger_;
};
// ~~ stormy::db::MongoHandler
}}
