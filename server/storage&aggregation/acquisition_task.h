#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>

#include "acquisition_setting.h"
#include "db_storage.h"

namespace stormy {
  namespace acquisition {

class Task : public Poco::Util::TimerTask
{
public:
	Task(db::Storage* _dbStorage, Setting* _server);
	~Task();

	void run() override;
private:
	Setting* server;
	db::Storage* dbStorage;
  Poco::Logger& logger_;
};
// ~~ stormy::acquisition::Task
}}