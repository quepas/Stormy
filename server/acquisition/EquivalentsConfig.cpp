#include "EquivalentsConfig.h"
#include "../../common/YAMLUtils.h"
#include <boost/algorithm/string.hpp>
#include <string>

using namespace Stormy;

EquivalentsConfig::EquivalentsConfig( std::string filePath )
	:	equivalents(std::vector<MeteoDataType*>())
{
	if(!load(filePath))
	{
		std::cout << "Couldn't load file properly: " << filePath << std::endl;
		equivalents.clear();
	}
}

EquivalentsConfig::~EquivalentsConfig()
{

}

std::vector<std::string> EquivalentsConfig::getEquivalentsForType( TYPE type )
{	
	MeteoDataType* meteoDataType = getMeteoDataTypeForType(type);
	if(meteoDataType != nullptr)	
		return meteoDataType -> equivalents; 
	return std::vector<std::string>();
}

MeteoDataType* EquivalentsConfig::getMeteoDataTypeForType( TYPE type )
{
	for(auto it = equivalents.begin(); it != equivalents.end(); ++it) {
		if((*it)->type == type)
			return *it;
	}
	return nullptr;
}

bool EquivalentsConfig::areEquivalentsForType( TYPE type, std::string text )
{
	MeteoDataType* meteoDataType = getMeteoDataTypeForType(type);
	std::vector<std::string> equivalentsVec = meteoDataType -> equivalents;
	for(auto it = equivalentsVec.begin(); it != equivalentsVec.end(); ++it) {
		if(*it == text)
			return true;
	}
	return false;
}

TYPE EquivalentsConfig::getTypeByEquivalent( std::string text )
{
	for(auto it = equivalents.begin(); it != equivalents.end(); ++it) {
		TYPE currentType = (*it) -> type;
		std::vector<std::string> equivalentsVec = (*it) -> equivalents;

		for(auto it = equivalentsVec.begin(); it != equivalentsVec.end(); ++it) {
			if(boost::equal(text, *it))			
				return currentType;
		}		
	}
	return T_UNKNOWN;
}

bool EquivalentsConfig::load( std::string filePath )
{
	YAML::Node root = YAML::LoadFile(filePath);

	for(auto it = root.begin(); it != root.end(); ++it)
	{
		if(YAMLUtils::isDefined(it, "type"))
		{
			std::string typeStr = YAMLUtils::getString(it, "type");
			if(!YAMLUtils::isDefined(it, "equivalents"))
			{
				std::cout << "Meteo data type is defined but with no string equivalents" << std::endl;
				return false;
			}			
			std::string equivalentsStr = YAMLUtils::getString(it, "equivalents");
			std::vector<std::string> splited; 
			boost::split(splited, equivalentsStr, boost::is_any_of(","));

			MeteoDataType* meteoDataType = new MeteoDataType();
			meteoDataType -> type = MeteoDataType::getTypeFromString(typeStr);
			meteoDataType -> equivalents = splited;			

			if(YAMLUtils::isDefined(it, "valueType"))
			{
				std::string valueTypeStr = YAMLUtils::getString(it, "valueType");
				meteoDataType -> valueType = MeteoDataType::getValueTypeFromString(valueTypeStr);
			}
			if(YAMLUtils::isDefined(it, "unit"))
			{
				std::string unitStr = YAMLUtils::getString(it, "unit");
				meteoDataType->unitType = MeteoDataType::getUnitTypeFromString(unitStr);
			}
			equivalents.push_back(meteoDataType);
		}				
	}
	return true;
}
