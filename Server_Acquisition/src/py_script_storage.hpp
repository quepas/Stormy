#pragma once

#include "py_parse_script.hpp"

#include <map>
#include <string>

namespace stormy {

typedef std::map<std::string, PyParseScript*> ScriptMap;

class PyScriptStorage
{
public:
  PyScriptStorage();
  ~PyScriptStorage();

  void Push(std::string id, PyParseScript* script);
  bool Remove(std::string id);
  bool Exists(std::string id) const;
  PyParseScript* Fetch(std::string id);
  const ScriptMap& scripts() const { return scripts_; }

private:
  ScriptMap scripts_;
};

}
// ~~ stormy::PyScriptStorage
