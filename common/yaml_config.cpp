#include "yaml_config.h"

#include "Utils.h"
#include "yaml_util.h"
#include <boost/lexical_cast.hpp>

using std::string;
using std::vector;
using std::map;
using boost::lexical_cast;
using YAML::Node;
using YAML::LoadFile;

namespace stormy {
  namespace common {
    namespace yaml {

Config::Config(string path)	
{
	OpenFromFile(path);	
}

Config::~Config()
{

}

bool Config::HasField(string field, uint16_t index /*= 0*/)
{
	if(CheckIndex(index)) {
		return data_[index].find(field) != data_[index].end();
	}	
	return false;
}

vector<string> Config::GetFields(uint16_t index /*= 0*/)
{
	vector<string> result;
	if(CheckIndex(index)) {
		map<string, string> entry = data_[index];
		for(auto it = entry.begin(); it != entry.end(); ++it) {
			result.push_back(it -> first);
		}
	}	
	return result;
}

uint16_t Config::Size()
{
	return data_.size();
}

string Config::AsString(string field, uint16_t index /*= 0*/)
{
	return GetValue(field, index);
}

int Config::AsInt(string field, uint16_t index /*= 0*/)
{
	string result = GetValue(field, index);
	if(!result.empty()) return lexical_cast<int>(result);
	return 0;
}

double Config::AsDouble(string field, uint16_t index /*= 0*/)
{
	string result = GetValue(field, index);
	if(!result.empty()) return lexical_cast<double>(result);
	return 0.0;
}

bool Config::AsBool(string field, uint16_t index /*= 0*/)
{	
	return Stormy::Utils::asBool(GetValue(field, index));	
}

void Config::OpenFromFile(string path)
{
	Node root = LoadFile(path);
	if(root.IsSequence()) {
		Util::forEach(root, [&](Node node) {
			ReadAndAddEntry(node);
		});		
	} else if(root.IsMap()) {
		ReadAndAddEntry(root);
	}
}

void Config::ReadAndAddEntry(Node entry)
{
	if(entry.IsMap()) {
		map<string, string> mappedEntry;
		for(auto it = entry.begin(); it != entry.end(); ++it) {
			mappedEntry.insert(make_pair(
        it -> first.as<std::string>(),
        it -> second.as<std::string>()
      ));
		}
		data_.push_back(mappedEntry);
	}
}

bool Config::CheckIndex( uint16_t index )
{
	return index <= (data_.size() - 1);
}

string Config::GetValue(string key, uint16_t index /*= 0*/)
{
	string result;
	if(CheckIndex(index) && HasField(key, index)) {
		result = data_[index][key];
	}
	return result;
}
// ~~ stormy::common::yaml::Config
}}}
