#pragma once

#include "common/db_has_data_expiration.h"
#include "common/entity_station.h"
#include "common/entity_metrics.h"
#include "common/entity_measurement.h"
#include "common/meteo_data.hpp"
#include "common/station_data.hpp"
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

typedef std::vector<common::entity::Measurement> MeteoData_;
typedef std::vector<common::entity::Metrics> MetricsData;
typedef std::map<std::time_t, MeteoData_> MeteoDataMap;

class MongoHandler : public common::db::HasDataExpiration
{
public:
  void ExpireData() override;
  std::vector<std::string> FetchStationsUID();

  void InsertStations(const std::vector<StationData>& stations);
  bool InsertMetrics(const MetricsSettings& metrics);
  void InsertMeteo(const MeteoData& meteo_data);
  bool IsMeteoExists(const MeteoData& meteo_data);

  bool ClearCollection(std::string collection_name);
  bool ClearStations();
  bool ClearMetrics();

  std::vector<StationData> GetStations();
  MetricsData GetMetrics();

  StationData GetStationByUID(std::string uid);
  unsigned int CountMeteo(std::string uid);

  MeteoDataMap GetMeteoBetween(std::string station_uid, std::time_t from, std::time_t to);
  MeteoDataMap GetMeteo(std::string station_uid, std::time_t ts);
  MeteoDataMap GetAllMeteo(std::string station_uid);

  const std::string& db_name() const { return db_name_; }
  bool is_connected() const { return is_connected_; }

  static MongoHandler& get() {
    static MongoHandler instance;
    return instance;
  }
private:
  MongoHandler(std::string db_address = "localhost", unsigned int port = 27017);
  MongoHandler(const MongoHandler&)
    : logger_(Poco::Logger::get("main")) {}
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

}}
// ~~ stormy::db::MongoHandler
