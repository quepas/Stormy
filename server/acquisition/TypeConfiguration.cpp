#include "TypeConfiguration.h"

#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string.hpp>
#include "../../common/YAMLUtils.h"

using namespace Stormy;
using namespace Meteo;

TypeConfiguration::TypeConfiguration( std::string filepath )
{
	if(!load(filepath)) {
		std::cout << "Couldn't load file properly: " << filepath << std::endl;
		types.clear();
	}
}

TypeConfiguration::~TypeConfiguration()
{

}

bool TypeConfiguration::load( std::string filepath )
{
	YAML::Node root = YAML::LoadFile(filepath);

	for(auto it = root.begin(); it != root.end(); ++it)
	{
		if(YAMLUtils::isDefined(it, "id"))
		{
			std::string id = YAMLUtils::getString(it, "id");
			if(!YAMLUtils::isDefined(it, "valueType")) {
				std::cout << "For " << id << " valueType is not defined" << std::endl;
				continue;
			}
			std::string valueType = YAMLUtils::getString(it, "valueType");
			if(!YAMLUtils::isDefined(it, "valueUnit")) {
				std::cout << "For " << id << " valueUnit is not defined" << std::endl;
				continue;
			}
			std::string valueUnit = YAMLUtils::getString(it, "valueUnit");
			if(!YAMLUtils::isDefined(it, "equivalents")) {
				std::cout << "For " << id << " equivalents is not defined" << std::endl;
				continue;
			}
			std::string equivalents = YAMLUtils::getString(it, "equivalents");			
			std::vector<std::string> splitedEquivalents;
			boost::split(splitedEquivalents, equivalents, boost::is_any_of(";,"));

			std::string valueFormat = "";
			if(YAMLUtils::isDefined(it, "valueFormat")) {
				valueFormat = YAMLUtils::getString(it, "valueFormat");
			}
			bool isMeteo = true;
			if(YAMLUtils::isDefined(it, "isMeteo")) {
				isMeteo = YAMLUtils::getBoolFromString(it, "isMeteo");
			}
			Type* type = new Type();
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

std::string TypeConfiguration::getTypeIdByEquivalent( std::string equivalent )
{
	for(auto it = types.begin(); it != types.end(); ++it) {
		std::string id = (*it) -> id;
		std::vector<std::string> equivalents = (*it) -> equivalents;
		for(auto it = equivalents.begin(); it != equivalents.end(); ++it) {
			if(boost::equal(equivalent, *it))
				return id;
		}
	}
	return "_none";
}

Type* TypeConfiguration::getFullTypeById( std::string id )
{
	for(auto it = types.begin(); it != types.end(); ++it) {
		Type* result = *it;
		if(boost::equal(id, result -> id))
			return result;
	}
	return nullptr;
}

std::string TypeConfiguration::getFirstEquivalentById( std::string id )
{
	Type* type = getFullTypeById(id);
	if(type != nullptr && type -> equivalents.size() > 0) {
		return type -> equivalents[0];
	}
	return "_none";
}
