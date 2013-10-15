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
			std::cout << "type: " << YAMLUtils::getString(it, "type") << std::endl;
		}
		// TODO: split equivalents
		if(YAMLUtils::isDefined(it, "equivalents"))
		{
			std::cout << "equivalents: " << YAMLUtils::getString(it, "equivalents") << std::endl;
		}
	}

	return true;
}
