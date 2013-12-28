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
	logger_.information("[py/Parser] Parsing from " + url + ".");
	PyObject* pArgs = PyTuple_New(1);
	PyObject* pURLValue = PyUnicode_FromString(url.c_str());
	PyTuple_SetItem(pArgs, 0, pURLValue);

	PyObject* pFuncResult = Function(parser_class_.c_str(), "run")(pArgs);
	Py_DECREF(pArgs);
	
	std::vector<common::entity::Measurement> result;
	if (pFuncResult != nullptr)
	{
		map<string, string> data =
			mapper::PairsFromSequence(pFuncResult);
		auto types = new acquisition::config::Metrics("config/meteo_data_type_config.yaml");

		// ~TODO: move to PyObjectMapper		
		for (auto it = data.begin(); it != data.end(); ++it) {
      entity::Measurement measure;     
			string id = types -> GetMetricsIdByEquivalent(it -> first);
      measure.code = id;
			entity::Metrics metrics = types -> GetMetricsById(id);
			string metrics_type = to_lower_copy(metrics.type);      
			string value = trim_copy(it -> second);

			if (value != "-" && !value.empty()) {
				if (metrics_type == acquisition::constant::number) {
          double out_value;
          if (acquisition::util::TryExtractFirstNumeric(value, out_value)) {
            measure.value_number = out_value;
            measure.is_numeric = true;
            result.push_back(measure);
          }
        } else if (metrics_type == acquisition::constant::text) {
					measure.value_text = value;
          measure.is_numeric = false;
          result.push_back(measure);
        }
			}      
		}
		delete types;
		Py_DECREF(pFuncResult);
		return result;
	}
	else
	{
		logger_.error("[py/Parser] No data at URL: " + url + ".");
		return result;
	}
}

vector<entity::Measurement> Parser::ParseFromStation(entity::Station station)
{
	auto result = ParseFromURL(station.url); 
	if(!result.empty()) {
    string date;
    string time;
    for (auto it = result.begin(); it != result.end(); ++it) {      
      if (it->code == acquisition::constant::date) {
        date = it->value_text;
      } else if (it->code == acquisition::constant::time) {
        time = it->value_text;
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
      it->station_uid = station.uid;
      it->timestamp = timestamp;
    }		
	}
	return result;
}
// ~~ stormy::py::Parser
}}
