#include "MeteoDataTypeEquivalentCfg.h"
#include "../../common/YAMLUtils.h"
#include <boost/algorithm/string.hpp>

Stormy::MeteoDataTypeEquivalentCfg::MeteoDataTypeEquivalentCfg( std::string filePath )
	:	equivalents(std::vector<MeteoDataType*>())
{
	if(!load(filePath))
	{
		std::cout << "Couldn't load file properly: " << filePath << std::endl;
		equivalents.clear();
	}
}

Stormy::MeteoDataTypeEquivalentCfg::~MeteoDataTypeEquivalentCfg()
{

}

std::vector<std::string> Stormy::MeteoDataTypeEquivalentCfg::getEquivalents( TYPE type )
{

}

Stormy::MeteoDataType Stormy::MeteoDataTypeEquivalentCfg::getEquivalentsData( TYPE type )
{

}

bool Stormy::MeteoDataTypeEquivalentCfg::isEquivalent( std::string text )
{

}

bool Stormy::MeteoDataTypeEquivalentCfg::load( std::string filePath )
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
