#include "RESTConst.h"

using namespace Stormy;
using namespace REST;

std::string Const::none = "none";
std::string Const::emptyJSON = "{}";
std::string Const::station = "station";
std::string Const::meteo = "meteo";
std::string Const::meteoStationIdPattern = "/meteo/[0-9a-f]{32}/{0, 1}";
std::string Const::stationPattern = "/station/{0, 1}";