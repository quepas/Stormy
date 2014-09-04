#include <Poco/Logger.h>

#include "db_storage.h"
#include "acquisition_scheduler.h"
#include "aggregation_engine.h"
#include "analysis_operation_mgr.h"

#include "net_http_server.hpp"
#include "common/util.h"
#include "settings.hpp"

using namespace stormy;
using namespace Poco;

int main(int argc, char** argv) {
  SetupLoggers();
  auto& logger = Logger::get("main");

  auto remote_servers = LoadRemoteServerSettings("config/remote_servers.json");
  auto db_storage = LoadDatabaseSetting("config/db_storage.json");
  auto db_aggregate = LoadDatabaseSetting("config/db_aggregate.json");

  analysis::OperationMgr operation_mgr("./", db_aggregate);
  db::Storage storage_for_acquisition(db_storage);

  logger.information("==== Storage & Aggregation started. ====");
  logger.information("Measurements in storage: " + 
    std::to_string(storage_for_acquisition.CountAllMeasurements()));
  logger.information("Available stations: " + 
    std::to_string(storage_for_acquisition.CountAllStations()));
  logger.information(">>-----------------------------------------------------------");
  logger.information("=== Acquisition Servers: ");
  for (auto& server : remote_servers) {
    logger.information(ToString(server));
  }

  logger.information("=== Storage database: ");
  logger.information("\t" + ToString(db_storage));
  logger.information("=== Aggregation database: ");
  logger.information("\t" + ToString(db_aggregate));
  logger.information(">>-----------------------------------------------------------");

  acquisition::Scheduler scheduler(&storage_for_acquisition);
  scheduler.Schedule(remote_servers);

  aggregation::Engine aggregation_engine(db_storage, db_aggregate);
  aggregation::Engine::Restarter engine_restarter(300);
  engine_restarter(aggregation_engine);

  net::StorageHttpServer http_server({ db_storage, db_aggregate }, 8070);
  http_server.run(argc, argv);
}
