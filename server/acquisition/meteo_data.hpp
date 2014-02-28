#pragma once

#include <string>

namespace stormy {

struct MeteoData
{
  struct Element
  {
    std::string id;
    double value;
  };

  int size;
  Element* elements;
};

// ~~ stormy::MeteoData
}