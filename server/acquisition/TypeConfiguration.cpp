#include "TypeConfiguration.h"

#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string.hpp>
#include "../../common/YAMLUtils.h"

using namespace Stormy;
using namespace Meteo;
using namespace std;

TypeConfiguration::TypeConfiguration( string filepath )
{
	if(!load(filepath)) {
		cout << "Couldn't load file properly: " << filepath << endl;
		types.clear();
	}
}

TypeConfiguration::~TypeConfiguration()
{

}

bool TypeConfiguration::load( string filepath )
{
	YAML::Node root = YAML::LoadFile(filepath);

	for(auto it = root.begin(); it != root.end(); ++it)
	{		
		if(YAMLUtils::isDefined(it, "id"))
		{
			string id = YAMLUtils::getString(it, "id");
			if(!YAMLUtils::isDefined(it, "valueType")) {
				cout << "For " << id << " valueType is not defined" << endl;
				continue;
			}
			string valueType = YAMLUtils::getString(it, "valueType");
			if(!YAMLUtils::isDefined(it, "valueUnit")) {
				cout << "For " << id << " valueUnit is not defined" << endl;
				continue;
			}
			string valueUnit = YAMLUtils::getString(it, "valueUnit");
			if(!YAMLUtils::isDefined(it, "equivalents")) {
				cout << "For " << id << " equivalents is not defined" << endl;
				continue;
			}
			string equivalents = YAMLUtils::getString(it, "equivalents");
			vector<string> splitedEquivalents;
			boost::split(splitedEquivalents, equivalents, boost::is_any_of(";,"));

			string valueFormat = "";
			if(YAMLUtils::isDefined(it, "valueFormat")) {
				valueFormat = YAMLUtils::getString(it, "valueFormat");
			}
			bool isMeteo = true;
			if(YAMLUtils::isDefined(it, "isMeteo")) {
				isMeteo = YAMLUtils::getBoolFromString(it, "isMeteo");
			}
			TypePtr type(new Type());
			type -> id = id;
			type -> valueType = valueType;
			type -> valueUnit = valueUnit;
			type -> equivalents = splitedEquivalents;
			type -> valueFormat = valueFormat;
			type -> isMeteo = isMeteo;
			types.push_back(type);
		}
	}
	return true;
}

string TypeConfiguration::getTypeIdByEquivalent( string equivalent )
{
	for(auto it = types.begin(); it != types.end(); ++it) {
		string id = (*it) -> id;
		vector<string> equivalents = (*it) -> equivalents;
		for(auto it = equivalents.begin(); it != equivalents.end(); ++it) {
			if(boost::equal(equivalent, *it))
				return id;
		}
	}
	return "_none";
}

TypePtr TypeConfiguration::getFullTypeById( string id )
{
	for(auto it = types.begin(); it != types.end(); ++it) {
		TypePtr result = *it;
		if(boost::equal(id, result -> id))
			return result;
	}
	return TypePtr(nullptr);
}

string TypeConfiguration::getFirstEquivalentById( string id )
{
	TypePtr type = getFullTypeById(id);
	if(type != nullptr && type -> equivalents.size() > 0) {
		return type -> equivalents[0];
	}
	return "_none";
}

TypePtr TypeConfiguration::getTypeById( string id, TypePtrVector types )
{
	for(auto it = types.begin(); it != types.end(); ++it) {
		if((*it) -> id == id) {
			return *it;
		}
	}
	return TypePtr(nullptr);
}
