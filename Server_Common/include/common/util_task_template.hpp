#pragma once

#include <Poco/Util/TimerTask.h>

namespace stormy {
  namespace util {

template<typename Context, typename Action>
class TaskTemplate : public Poco::Util::TimerTask
{
public:
  explicit TaskTemplate(Context context) : context_(context) {}
  ~TaskTemplate() {}

  void run() override { Action::run(context_); }
private:
  Context context_;
};

}}
// ~~ stormy::util::TaskTemplate
