#include "py_parser.h"

#include "acquisition_util.h"
#include "db_constant.h"
#include "py_mapper.h"
#include "py_function.h"
#include "util.h"

#include <ctime>
#include <map>
#include <boost/any.hpp>
#include <boost/algorithm/string.hpp>
#include <Python.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Timestamp.h>

using namespace stormy::common;

using boost::any;
using boost::any_cast;
using boost::to_lower_copy;
using boost::trim_copy;
using std::map;
using std::string;
using std::vector;
using std::tm;
using std::time;
using Poco::DateTime;
using Poco::DateTimeParser;
using Poco::Logger;
using Poco::NumberFormatter;
using Poco::Timestamp;

namespace stormy {
  namespace py {

Parser::Parser(string parser_class)
  : logger_(Logger::get("py/Parser")),
    parser_class_(parser_class),
    metrics_config_("config/meteo_data_type_config.yaml")
{

}

Parser::~Parser()
{

}

vector<entity::Measurement> Parser::ParseFromStation(entity::Station station)
{
  logger_.information("[py/Parser] Parsing from " + station.url + ".");
  PyObject* pArgs = PyTuple_New(1);
  PyObject* pURLValue = PyUnicode_FromString(station.url.c_str());
  PyTuple_SetItem(pArgs, 0, pURLValue);

  PyObject* pFuncResult = common::py::Function(parser_class_.c_str(), "run")(pArgs);
  Py_DECREF(pArgs);

  vector<entity::Measurement> result;
  if (pFuncResult != nullptr)
  {
    auto data = mapper::PairsFromSequence(pFuncResult);
    string date, time;

    for (auto it = data.begin(); it != data.end(); ++it) {
      entity::Measurement measure;
      string metrics_code = metrics_config_.GetMetricsIdByEquivalent(it->first);
      measure.code = metrics_code;
      measure.station_uid = station.uid;
      entity::Metrics metrics = metrics_config_.GetMetricsById(metrics_code);
      string str_value = trim_copy(it->second);

      if(metrics_code == db::constant::date) {
        date = str_value;
      } else if(metrics_code == db::constant::time) {
        time = str_value;
      }
      if (metrics.is_meteo) {
        string metrics_type = to_lower_copy(metrics.type);

        if (str_value != "-" && !str_value.empty()) {
          if (metrics_type == db::constant::number) {
            double out_value;
            if (acquisition::util::TryExtractFirstNumeric(str_value, out_value)) {
              measure.value_number = out_value;
              measure.is_numeric = true;
              result.push_back(measure);
            }
          } else if (metrics_type == db::constant::text) {
            measure.value_text = str_value;
            measure.is_numeric = false;
            result.push_back(measure);
          }
        }
      }
    }    
    time_t time_now = std::time(nullptr);
    tm timestamp = *localtime(&time_now);
    DateTime acquire_time;
    int dtz;
    if (DateTimeParser::tryParse(date + " " + time, acquire_time, dtz)) {      
      time_t acquire_time_t = acquire_time.timestamp().epochTime();
      timestamp = *localtime(&acquire_time_t);
    }
    for (auto it = result.begin(); it != result.end(); ++it) {
      it->timestamp = timestamp;
    }
    Py_DECREF(pFuncResult);
    return result;
  }
  else
  {
    logger_.error("[py/Parser] No data at URL: " + station.url + ".");
    return result;
  }
}
// ~~ stormy::py::Parser
}}
