#pragma once

#include <string>

namespace stormy {
  namespace rest {
    namespace constant {

const std::string none = "none";
const std::string emptyJSON = "{}";
const std::string station = "station";
const std::string meteo = "meteo";

// api: /meteo/:stationId
const std::string meteoStationIdPattern = "/meteo/[0-9a-f]{32}/{0, 1}";
// api: /meteo/:stationId/:type
const std::string meteoStationIdTypePattern = "/meteo/[0-9a-f]{32}/[0-9a-zA-z]{1, 50}";
// api: /meteo/:stationId/:timestamp
const std::string meteoStationIdTimestampPattern = "/meteo/[0-9a-f]{32}/[0-9]{10}/{0, 1}";
// api: /station
const std::string stationPattern = "/station/{0, 1}";
// api: /info
const std::string infoPattern = "/info/{0, 1}";
// api: /info/:stationId
const std::string infoStationPattern = "/info/[0-9a-f]{32}/{0, 1}";
// api: /metrics
const std::string metricsPattern = "/metrics/{0, 1}";
// ~~ stormy::rest::constant
}}}
