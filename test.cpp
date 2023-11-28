#include "CustomException.hpp"
#include "RobotsParser.hpp"
#include <iostream>
int main() {

  try {
    RobotsParser robotsParser;

    std::map<std::string, std::map<std::string, std::vector<std::string>>>
        result = robotsParser.parseFromFile("./robots.txt");

    RobotsParser::printMap(result);

  } catch (CustomException &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
