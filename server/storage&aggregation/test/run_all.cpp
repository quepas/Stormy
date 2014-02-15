#include <iostream>
#include <cppunit/ui/text/TestRunner.h>

int main() 
{
  std::cout << "========================================" << std::endl;
  std::cout << "Running storage and aggregation tests..." << std::endl;
  std::cout << "========================================" << std::endl;

  CppUnit::TextUi::TestRunner test_runner;
  test_runner.run();

  getchar();
}