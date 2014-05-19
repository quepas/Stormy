#pragma once

#include <ctime>

namespace stormy {
  namespace common {
    namespace db {

class HasDataExpiration 
{
public:
  HasDataExpiration()
    : expiration_seconds_(72 * 3600) {}

  virtual void ExpireData() = 0;
  std::time_t expiration_seconds() { return expiration_seconds_; }
  void set_expiration_seconds(std::time_t expiration_seconds) {
    if (expiration_seconds > 0)
      expiration_seconds_ = expiration_seconds;  
  }
protected:
  std::time_t expiration_seconds_;
};
// ~~ stormy::common::db::HasDataExpiration
}}}
