#pragma once

#include <ctime>
#include <string>

namespace stormy {
  namespace acquisition {
    namespace util {

bool TryExtractFirstNumeric(std::string text, double& out_value);

std::time_t MinutesToMiliseconds(std::time_t minutes);
std::time_t SecondsToMiliseconds(std::time_t seconds);

std::string GetMeteoDb();
std::string GetStationDb();
std::string GetTypeDb();
// ~~ stormy::acquisition::util
}}}
