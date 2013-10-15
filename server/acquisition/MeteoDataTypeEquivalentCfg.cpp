#include "MeteoDataTypeEquivalentCfg.h"
#include "../../common/YAMLUtils.h"

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
			// TODO: split equivalents
			std::cout << "equivalents: " << YAMLUtils::getString(it, "equivalents") << std::endl;
			std::string equivalentsStr = YAMLUtils::getString(it, "equivalents");
			MeteoDataType* meteoDataType = new MeteoDataType();
			meteoDataType -> type = MeteoDataType::getTypeFromString(typeStr);
			meteoDataType->equivalents.push_back(equivalentsStr);
			equivalents.push_back(meteoDataType);
		}				
	}
	return true;
}
