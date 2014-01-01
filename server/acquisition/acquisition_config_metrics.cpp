#include "acquisition_config_metrics.h"

#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string.hpp>
#include "../../common/yaml_util.h"

using namespace stormy::common;
using boost::equal;
using boost::split;
using boost::is_any_of;
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
		if (yaml::IsDefined(it, "id"))
		{
			string id = yaml::GetString(it, "id");
			if (!yaml::IsDefined(it, "valueType")) {
				//cout << "For " << id << " valueType is not defined" << endl;
				continue;
			}
			string valueType = yaml::GetString(it, "valueType");
			if (!yaml::IsDefined(it, "valueUnit")) {
				//cout << "For " << id << " valueUnit is not defined" << endl;
				continue;
			}
			string valueUnit = yaml::GetString(it, "valueUnit");
			if (!yaml::IsDefined(it, "equivalents")) {
				//cout << "For " << id << " equivalents is not defined" << endl;
				continue;
			}
			string equivalents = yaml::GetString(it, "equivalents");			

			string valueFormat = "";
			if (yaml::IsDefined(it, "valueFormat")) {
				valueFormat = yaml::GetString(it, "valueFormat");
			}
			bool isMeteo = true;
			if (yaml::IsDefined(it, "isMeteo")) {
				isMeteo = yaml::GetBool(it, "isMeteo");
			}
			entity::Metrics type;
			type.code = id;
			type.type = valueType;
			type.unit = valueUnit;
			type.equivalents = equivalents;
			type.format = valueFormat;
			type.is_meteo = isMeteo;
			configuration_.push_back(type);
		}
	}
	return true;
}

string Metrics::GetMetricsIdByEquivalent(string equivalent)
{
	for (auto it = configuration_.begin(); it != configuration_.end(); ++it) {
		string id = it->code;
		vector<string> equivalents;
    vector<string> splitedEquivalents;
    split(splitedEquivalents, it->equivalents, is_any_of(";,"));

		for (auto it = splitedEquivalents.begin(); it != splitedEquivalents.end(); ++it) {
			if(equal(equivalent, *it))
				return id;
		}
	}
	return "_none";
}

entity::Metrics Metrics::GetMetricsById(string id)
{
	for (auto it = configuration_.begin(); it != configuration_.end(); ++it) {
		entity::Metrics result = *it;
		if(equal(id, result.code))
			return result;
	}
	return entity::Metrics();
}

string Metrics::GetFirstEquivalentByMetricsId(string id)
{
	entity::Metrics type = GetMetricsById(id);
	if (!type.equivalents.empty()) {
		return type.equivalents;
	}
	return "_none";
}

entity::Metrics Metrics::GetMetricsById(
  string id, 
  vector<entity::Metrics> types)
{
	for (auto it = types.begin(); it != types.end(); ++it) {
		if (it->code == id) {
			return *it;
		}
	}
	return entity::Metrics();
}
// ~~ stormy::acquisition::config::Metrics
}}}
