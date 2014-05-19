#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

namespace stormy {
  namespace common {

#define TRY try {
#define CATCH                         \
  } catch(const std::exception& ex){  \
    logger_.error(ex.what());         \
  }
#define CATCH_MSG(msg)                                  \
  } catch(const std::exception& ex) {                   \
    logger_.error(std::string(msg) + ": " + ex.what()); \
  }

double ToDouble(std::string number);
bool IsNumeric(std::string number);
std::string MD5(std::string text);
bool IsHexMD5(std::string text);
std::string ExtractMD5(std::string text);
std::string ExtractURIEndPathSegment(std::string URI);
bool ToBool(std::string text);
bool IsMatch(std::string text, std::string regex);

// date & time
bool IsDate(std::string date);  // YYYY-MM-DD
bool IsTime(std::string time);  // HH:MM
std::time_t LocaltimeNow();
std::time_t MakeUTCIfPossible(std::time_t time);

// const
const std::string numeric_pattern = "-?[0-9]*([.][0-9]+)?";
const std::string date_pattern = "[0-9]{4}-[0-9]{2}-[0-9]{2}";
const std::string time_pattern = "[0-9]{1, 2}:[0-9]{1, 2}";
const std::string hex_md5_pattern = "[0-9a-f]{32}";

template<typename T, typename V>
bool Contains(T seq, V key)
{
  return find(seq.begin(), seq.end(), key) != seq.end();
}

template<typename T, typename V>
void Each(const T& data, V func)
{
  std::for_each(data.begin(), data.end(), func);
}

template<typename T, typename V>
T Last(V seq)
{
  if(seq.empty()) return T();
  return *(--seq.end());
}
// ~~ stormy::common::util
}}
