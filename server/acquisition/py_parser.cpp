#include "py_parser.h"

#include "py_function.h"
#include "py_mapper.h"
#include "../../common/util.h"
#include "MeteoUtils.h"

#include <map>
#include <boost/any.hpp>
#include <boost/algorithm/string.hpp>
#include <Python.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Timestamp.h>

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
		auto types = new Stormy::Meteo::TypeConfiguration("config/meteo_data_type_config.yaml");

		// ~TODO: move to PyObjectMapper		
		for(auto it = data.begin(); it != data.end(); ++it) {
			string id = types -> getTypeIdByEquivalent(it -> first);
			Stormy::TypePtr type = types -> getFullTypeById(id);
			string valueType = to_lower_copy(type -> valueType);
			string value = trim_copy(it -> second);

			if(value != "-") {
				if(valueType == Stormy::Meteo::Const::number)
					result -> data[id] = Stormy::MeteoUtils::extractTemperature(value);
				else if(valueType == Stormy::Meteo::Const::text)
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

Stormy::MeasurementPtr Parser::ParseFromStation(Stormy::Meteo::Station station)
{
	Stormy::MeasurementPtr result = ParseFromURL(station.url);
	result -> station = new Stormy::Meteo::Station(station);
	
	if(result) {
		auto data = map<string, any>();
		data = result -> data;
		result -> data[Stormy::Meteo::Const::stationId] = station.stationId;

		Timestamp timestamp;
		if(data.find(Stormy::Meteo::Const::date) != data.end()
			&& data.find(Stormy::Meteo::Const::time) != data.end())
		{
			string date = any_cast<string>(data[Stormy::Meteo::Const::date]);
			string time = any_cast<string>(data[Stormy::Meteo::Const::time]);

			if(stormy::common::IsDate(date)
				&& stormy::common::IsTime(time))
			{
				string dateTime = date + " " + time;
				int diffTimeZone;
				DateTime acqDateTime =
					DateTimeParser::parse(dateTime, diffTimeZone);
				timestamp = acqDateTime.timestamp();
			}
		}
		result -> timestamp = timestamp;
		result -> data[Stormy::Meteo::Const::mongoId] =
			NumberFormatter::format(timestamp.epochTime());
	}
	return result;
}
// ~~ stormy::py::Parser
}}
