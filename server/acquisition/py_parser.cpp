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

Stormy::MeasurementPtr Parser::ParseFromURL(string url)
{
	logger_.information("[py/Parser] Parsing from " + url + ".");
	PyObject* pArgs = PyTuple_New(1);
	PyObject* pURLValue = PyUnicode_FromString(url.c_str());
	PyTuple_SetItem(pArgs, 0, pURLValue);

	PyObject* pFuncResult = Function(parser_class_.c_str(), "run")(pArgs);
	Py_DECREF(pArgs);
	
	Stormy::MeasurementPtr result(new Stormy::Meteo::Measurement());
	if(pFuncResult != nullptr)
	{
		map<string, string> data =
			mapper::PairsFromSequence(pFuncResult);
		auto types = new acquisition::config::Metrics("config/meteo_data_type_config.yaml");

		// ~TODO: move to PyObjectMapper		
		for(auto it = data.begin(); it != data.end(); ++it) {
			string id = types -> GetMetricsIdByEquivalent(it -> first);
			entity::Metrics type = types -> GetMetricsById(id);
			string valueType = to_lower_copy(type.type);
			string value = trim_copy(it -> second);

			if(value != "-") {
				if(valueType == acquisition::constant::number)
					result -> data[id] = acquisition::util::ExtractTemperature(value);
				else if(valueType == acquisition::constant::text)
					result -> data[id] = value;
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

Stormy::MeasurementPtr Parser::ParseFromStation(entity::Station station)
{
	Stormy::MeasurementPtr result = ParseFromURL(station.url);
  // TODO: remove this dependency (deep copy)
  auto ptr_station = new Stormy::Meteo::Station();
  ptr_station->url = station.url;
  ptr_station->name = station.name;
  ptr_station->parserClass = station.parser_class;
  ptr_station->refreshTime = station.refresh_time;
  ptr_station->stationId = station.uid;
	result -> station = ptr_station;
	// ~~

	if(result) {
		auto data = map<string, any>();
		data = result -> data;
		result -> data[acquisition::constant::stationId] = station.uid;

		Timestamp timestamp;
		if(data.find(acquisition::constant::date) != data.end()
			&& data.find(acquisition::constant::time) != data.end())
		{
			string date = any_cast<string>(data[acquisition::constant::date]);
			string time = any_cast<string>(data[acquisition::constant::time]);

			if(IsDate(date)
				&& IsTime(time))
			{
				string dateTime = date + " " + time;
				int diffTimeZone;
				DateTime acqDateTime =
					DateTimeParser::parse(dateTime, diffTimeZone);
				timestamp = acqDateTime.timestamp();
			}
		}
		result -> timestamp = timestamp;
		result -> data[acquisition::constant::mongoId] =
			NumberFormatter::format(timestamp.epochTime());
	}
	return result;
}
// ~~ stormy::py::Parser
}}
