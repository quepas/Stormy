#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>

#include "settings.hpp"
#include "db_storage.h"

namespace stormy {
  namespace acquisition {

class Task : public Poco::Util::TimerTask
{
public:
  Task(db::Storage* db_storage, RemoteServerSetting server);
  ~Task();

  void run() override;
private:
  RemoteServerSetting server_;
  db::Storage* db_storage_;
  Poco::Logger& logger_;
};

}}
// ~~ stormy::acquisition::Task