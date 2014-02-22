#include "json_config.h"

#include <fstream>
#include <json/reader.h>

using Json::Reader;
using Json::Value;
using std::string;
using std::ifstream;

namespace stormy {

JSONConfig::JSONConfig(const string& file_path)
{
  ifstream file_stream(file_path.c_str());
  if (!file_stream.is_open())
    throw no_file_exception("No valid file to open");
    
  Reader json_reader;
  bool parsing_successful = json_reader.parse(file_stream, root_, false);  
  if (!parsing_successful)
  {
    throw bad_file_exception(
      string("File is ill-formed: " 
        + json_reader.getFormattedErrorMessages()).c_str());
  }
  file_stream.close();
}

JSONConfig::~JSONConfig()
{
}
// stormy::JSONConfig
}