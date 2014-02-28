#include <iostream>
#include <cppunit/ui/text/TestRunner.h>

// unit tests
//#include "acquisition_util_test.h"

int main()
{
  std::cout << "============================" << std::endl;
  std::cout << "Running acquisition tests..." << std::endl;
  std::cout << "============================" << std::endl;
  
  CppUnit::TextUi::TestRunner test_runner;
  //test_runner.addTest(AcquisitionUtilTest::suite());
  test_runner.run();

  getchar();
}