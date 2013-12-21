#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>

#include "AcquisitionServer.h"
#include "DBStorage.h"

namespace stormy {
  namespace acquisition {

class Task : public Poco::Util::TimerTask
{
public:
	Task(Stormy::DBStorage* _dbStorage, Stormy::AcquisitionServer* _server);
	~Task();

	void run() override;
private:
	Stormy::AcquisitionServer* server;
	Stormy::DBStorage* dbStorage;
  Poco::Logger& logger_;
};
// ~~ stormy::acquisition::Task
}}