#include "acquisition_config_metrics.h"

#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string.hpp>
#include "../../common/yaml_util.h"

using boost::equal;
using std::string;
using std::vector;
using YAML::Node;
using YAML::LoadFile;

namespace stormy {
  namespace acquisition {
    namespace config {

Metrics::Metrics(string filepath)
{
	if (!Load(filepath)) {
		//cout << "Couldn't load file properly: " << filepath << endl;
		configuration_.clear();
	}
}

Metrics::~Metrics()
{

}

bool Metrics::Load(string filepath)
{
	Node root = LoadFile(filepath);

	for (auto it = root.begin(); it != root.end(); ++it)
	{		
		if (common::yaml::IsDefined(it, "id"))
		{
			string id = common::yaml::GetString(it, "id");
			if (!common::yaml::IsDefined(it, "valueType")) {
				//cout << "For " << id << " valueType is not defined" << endl;
				continue;
			}
			string valueType = common::yaml::GetString(it, "valueType");
			if (!common::yaml::IsDefined(it, "valueUnit")) {
				//cout << "For " << id << " valueUnit is not defined" << endl;
				continue;
			}
			string valueUnit = common::yaml::GetString(it, "valueUnit");
			if (!common::yaml::IsDefined(it, "equivalents")) {
				//cout << "For " << id << " equivalents is not defined" << endl;
				continue;
			}
			string equivalents = common::yaml::GetString(it, "equivalents");
			vector<string> splitedEquivalents;
			boost::split(splitedEquivalents, equivalents, boost::is_any_of(";,"));

			string valueFormat = "";
			if (common::yaml::IsDefined(it, "valueFormat")) {
				valueFormat = common::yaml::GetString(it, "valueFormat");
			}
			bool isMeteo = true;
			if (common::yaml::IsDefined(it, "isMeteo")) {
				isMeteo = common::yaml::GetBool(it, "isMeteo");
			}
			Stormy::TypePtr type(new Stormy::Meteo::Type());
			type -> id = id;
			type -> valueType = valueType;
			type -> valueUnit = valueUnit;
			type -> equivalents = splitedEquivalents;
			type -> valueFormat = valueFormat;
			type -> isMeteo = isMeteo;
			configuration_.push_back(type);
		}
	}
	return true;
}

string Metrics::GetMetricsIdByEquivalent(string equivalent)
{
	for (auto it = configuration_.begin(); it != configuration_.end(); ++it) {
		string id = (*it) -> id;
		vector<string> equivalents = (*it) -> equivalents;
		for (auto it = equivalents.begin(); it != equivalents.end(); ++it) {
			if(equal(equivalent, *it))
				return id;
		}
	}
	return "_none";
}

Stormy::TypePtr Metrics::GetMetricsById(string id)
{
	for (auto it = configuration_.begin(); it != configuration_.end(); ++it) {
		Stormy::TypePtr result = *it;
		if(equal(id, result -> id))
			return result;
	}
	return Stormy::TypePtr(nullptr);
}

string Metrics::GetFirstEquivalentByMetricsId(string id)
{
	Stormy::TypePtr type = GetMetricsById(id);
	if (type != nullptr && type -> equivalents.size() > 0) {
		return type -> equivalents[0];
	}
	return "_none";
}

Stormy::TypePtr Metrics::GetMetricsById(
  string id, 
  Stormy::TypePtrVector types)
{
	for (auto it = types.begin(); it != types.end(); ++it) {
		if ((*it) -> id == id) {
			return *it;
		}
	}
	return Stormy::TypePtr(nullptr);
}
// ~~ stormy::acquisition::config::Metrics
}}}
