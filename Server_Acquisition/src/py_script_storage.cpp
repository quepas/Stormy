#include "py_script_storage.hpp"

using std::make_pair;
using std::string;

namespace stormy {

PyScriptStorage::PyScriptStorage()
{

}

PyScriptStorage::~PyScriptStorage()
{
  for (auto entry : scripts_) {
    delete entry.second;
  }
  scripts_.clear();
}

void PyScriptStorage::Push(std::string id, PyParseScript* script)
{
  scripts_.insert(make_pair(id, script));
}

bool PyScriptStorage::Remove(string id)
{
  if (!Exists(id)) {
    return false;
  }
  scripts_.erase(id);
  return true;
}

bool PyScriptStorage::Exists(std::string id) const
{
  return scripts_.find(id) != scripts_.end();
}

PyParseScript* PyScriptStorage::Fetch(std::string id)
{
  if (Exists(id)) {
    return scripts_[id];
  } 
  else {
    return nullptr;
  }
}

}
// ~~ stormy::PyScriptStorage
