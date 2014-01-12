#pragma once

#include <ctime>
#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <mongo/client/dbclient.h>
#include <Poco/Logger.h>

#include "../../common/db_handler.h"
#include "../../common/db_has_data_expiration.h"
#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"
#include "../../common/entity_measurement.h"

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
  MongoHandler(std::string dbAddress = "localhost");
  MongoHandler(const MongoHandler&) 
    : Handler("MongoDB"), 
      logger_(Poco::Logger::get("db/MongoHandler")) {}
  MongoHandler& operator=(const MongoHandler&) {}
  ~MongoHandler();

  void connect(std::string dbAddress);			
  mongo::DBClientConnection connection;
  Poco::Logger& logger_;
};
// ~~ stormy::db::MongoHandler
}}
