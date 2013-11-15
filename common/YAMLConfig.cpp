#include "YAMLConfig.h"

#include "Utils.h"
#include "YAMLUtils.h"

using namespace Stormy;
using namespace std;
using namespace YAML;

YAMLConfig::YAMLConfig( string path )	
{
	openFromFile(path);	
}

YAMLConfig::~YAMLConfig()
{

}

bool YAMLConfig::hasField( string field, unsigned int index /*= 0*/ )
{
	if(checkIndex(index)) {
		return data[index].find(field) != data[index].end();
	}	
	return false;
}

vector<string> YAMLConfig::getFields( unsigned int index /*= 0*/ )
{
	vector<string> result;
	if(checkIndex(index)) {
		map<string, string> entry = data[index];
		for(auto it = entry.begin(); it != entry.end(); ++it) {
			result.push_back(it -> first);
		}
	}	
	return result;
}

unsigned int YAMLConfig::Size()
{
	return data.size();
}

std::string YAMLConfig::asString( string field, unsigned int index /*= 0*/ )
{
	return getValue(field, index);
}

int YAMLConfig::asInt( string field, unsigned int index /*= 0*/ )
{
	std::string result = getValue(field, index);
	if(!result.empty()) return lexical_cast<int>(result);
	return 0;
}

double YAMLConfig::asDouble( string field, unsigned int index /*= 0*/ )
{
	std::string result = getValue(field, index);
	if(!result.empty()) return lexical_cast<double>(result);
	return 0.0;
}

bool YAMLConfig::asBool( string field, unsigned int index /*= 0*/ )
{	
	return Utils::asBool(getValue(field, index));	
}

void YAMLConfig::openFromFile( string path )
{
	Node root = LoadFile(path);
	if(root.IsSequence()) {
		YAMLUtils::forEach(root, [&](Node node) {
			readAndAddEntry(node);
		});		
	} else if(root.IsMap()) {
		readAndAddEntry(root);
	}
}

void YAMLConfig::readAndAddEntry( Node entry )
{
	if(entry.IsMap()) {
		map<string, string> mappedEntry;
		for(auto it = entry.begin(); it != entry.end(); ++it) {
			mappedEntry.insert(make_pair(it -> first.as<std::string>(),
										 it -> second.as<std::string>()));
		}
		data.push_back(mappedEntry);
	}
}

bool YAMLConfig::checkIndex( unsigned int index )
{
	return index <= (data.size() - 1);
}

string YAMLConfig::getValue( string key, unsigned int index /*= 0*/ )
{
	std::string result = "";
	if(checkIndex(index) && hasField(key, index)) {
		result = data[index][key];
	}
	return result;
}
