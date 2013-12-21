#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>

#include "acquisition_setting.h"
#include "DBStorage.h"

namespace stormy {
  namespace acquisition {

class Task : public Poco::Util::TimerTask
{
public:
	Task(Stormy::DBStorage* _dbStorage, Setting* _server);
	~Task();

	void run() override;
private:
	Setting* server;
	Stormy::DBStorage* dbStorage;
  Poco::Logger& logger_;
};
// ~~ stormy::acquisition::Task
}}