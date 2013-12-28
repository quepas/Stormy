#include "py_parser.h"

#include "acquisition_util.h"
#include "py_function.h"
#include "py_mapper.h"
#include "../../common/util.h"

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
using Poco::DateTime;
using Poco::DateTimeParser;
using Poco::Logger;
using Poco::NumberFormatter;
using Poco::Timestamp;

namespace stormy {
  namespace py {

Parser::Parser(string parser_class)
	:	logger_(Logger::get("py/Parser")),
    parser_class_(parser_class)
{

}

Parser::~Parser()
{

}

vector<entity::Measurement> Parser::ParseFromURL(string url)
{
	
}

vector<entity::Measurement> Parser::ParseFromStation(entity::Station station)
{
  logger_.information("[py/Parser] Parsing from " + station.url + ".");
  PyObject* pArgs = PyTuple_New(1);
  PyObject* pURLValue = PyUnicode_FromString(station.url.c_str());
  PyTuple_SetItem(pArgs, 0, pURLValue);

  PyObject* pFuncResult = Function(parser_class_.c_str(), "run")(pArgs);
  Py_DECREF(pArgs);

  vector<entity::Measurement> result;
  if (pFuncResult != nullptr)
  {
    auto data = mapper::PairsFromSequence(pFuncResult);
    auto types = new acquisition::config::Metrics("config/meteo_data_type_config.yaml");				
    string date, time;

    for (auto it = data.begin(); it != data.end(); ++it) {
      entity::Measurement measure;     
      string metrics_code = types->GetMetricsIdByEquivalent(it->first);
      measure.code = metrics_code;
      entity::Metrics metrics = types->GetMetricsById(metrics_code);
      string str_value = trim_copy(it->second);

      if(metrics_code == acquisition::constant::date) {
        date = str_value;
      } else if(metrics_code == acquisition::constant::time) {
        time = str_value;
      }     
      if (metrics.is_meteo) {
        string metrics_type = to_lower_copy(metrics.type);      			  

        if (str_value != "-" && !str_value.empty()) {
          if (metrics_type == acquisition::constant::number) {
            double out_value;
            if (acquisition::util::TryExtractFirstNumeric(str_value, out_value)) {
              measure.value_number = out_value;
              measure.is_numeric = true;
              result.push_back(measure);
            }
          } else if (metrics_type == acquisition::constant::text) {
            measure.value_text = str_value;
            measure.is_numeric = false;
            result.push_back(measure);
          }
        }
      }
    }

    tm timestamp;
    if(IsDate(date) && IsTime(time))
    {
      string dateTime = date + " " + time;
      int diffTimeZone;
      DateTime acqDateTime =
        DateTimeParser::parse(dateTime, diffTimeZone);
      time_t time = acqDateTime.timestamp().epochTime();
      timestamp = *gmtime(&time);
    }
    for (auto it = result.begin(); it != result.end(); ++it) {
      it->timestamp = timestamp;
      it->station_uid = station.uid;
    }
    delete types;
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
