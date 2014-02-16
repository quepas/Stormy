#include <iostream>
#include <cppunit/ui/text/TestRunner.h>

// unit tests
#include "util_test.h"
#include "rest_uri_parser_test.h"

int main() 
{
  std::cout << "=======================" << std::endl;
  std::cout << "Running common tests..." << std::endl;
  std::cout << "=======================" << std::endl;

  CppUnit::TextUi::TestRunner test_runner;
  test_runner.addTest(UtilTest::suite());
  test_runner.addTest(RestURIParserTest::suite());
  test_runner.run();

  getchar();
  return 0;
}