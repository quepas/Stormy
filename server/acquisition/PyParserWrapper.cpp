#include "PyParserWrapper.h"

#include <Python.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>
#include <boost/regex.hpp>
#include <Poco/Timestamp.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>

#include "PyFunction.h"
#include "PyObjectMapper.h"
#include "TypeConfiguration.h"
#include "Utils.h"
#include "MeteoUtils.h"
#include "MeteoConst.h"

using namespace Stormy;
using namespace Meteo;

using boost::any_cast;
using boost::to_lower_copy;
using boost::trim_copy;
using Poco::Timestamp;
using Poco::DateTimeParser;
using Poco::DateTime;

PyParserWrapper::PyParserWrapper( std::string _pyParserModuleName )
	:	pyParserModuleName(_pyParserModuleName)
{
}

PyParserWrapper::~PyParserWrapper()
{
}

Measurement* PyParserWrapper::parseFromURL( std::string url )
{
	PyObject* pArgs = PyTuple_New(1);
	PyObject* pURLValue = PyUnicode_FromString(url.c_str());
	PyTuple_SetItem(pArgs, 0, pURLValue);

	PyObject* pFuncResult = PyFunction(pyParserModuleName.c_str(), "run")(pArgs);
	Py_DECREF(pArgs);

	if(pFuncResult != nullptr)
	{
		std::map<std::string, std::string> data =
			PyObjectMapper::extractDictsFromDictSequence(pFuncResult);
		TypeConfiguration* types =
			new TypeConfiguration("config/meteo_data_type_config.yaml");

		// ~TODO: move to PyObjectMapper
		Measurement* result = new Measurement();
		for(auto it = data.begin(); it != data.end(); ++it) {
			std::string id = types -> getTypeIdByEquivalent(it -> first);
			Type* type = types -> getFullTypeById(id);
			std::string valueType = to_lower_copy(type -> valueType);
			std::string value = trim_copy(it -> second);

			if(value != "-") {
				if(valueType == Const::number)
					result -> data[id] = MeteoUtils::extractTemperature(value);
				else if(valueType == Const::text)
					result -> data[id] = value;
			}
		}
		Py_DECREF(pFuncResult);
		return result;
	}
	else
	{
		std::cout << "No data at current URL" << std::endl;
		return new Measurement(Const::reasonNoData);
	}
}

Measurement* PyParserWrapper::parseFromStation( Station* station )
{
	Measurement* result = parseFromURL(station -> url);
	result -> station = station;
	if(result) {
		auto data = std::map<std::string, boost::any>();
		data = result -> data;
		result -> data[Const::stationId] = station -> stationId;

		Timestamp timestamp;
		if(data.find(Const::date) != data.end()
			&& data.find(Const::time) != data.end())
		{
			std::string date = any_cast<std::string>(data[Const::date]);
			std::string time = any_cast<std::string>(data[Const::time]);

			if(Utils::checkIfStandardDate(date)
				&& Utils::checkIfStandardTime(time))
			{
				std::string dateTime = date + " " + time;
				int diffTimeZone;
				DateTime acqDateTime =
					DateTimeParser::parse(dateTime, diffTimeZone);
				timestamp = acqDateTime.timestamp();
			}
		}
		result -> timestamp = timestamp;
		result -> data[Const::mongoId] =
			boost::lexical_cast<std::string>(timestamp.epochTime());
	}
	return result;
}
