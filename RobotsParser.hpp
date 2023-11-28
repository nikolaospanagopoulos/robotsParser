#pragma once
#include "Downloader.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

class RobotsParser {
private:
  std::ifstream inFile;
  std::stringstream *openFile(const std::string &fileName);
  std::stringstream *fileContentsStream;
  char *buffer;
  Downloader *downloader;
  std::string *splitString(std::string &input, const char delimeter) const;
  void trimText(std::string &text) const;
  std::map<std::string, std::map<std::string, std::vector<std::string>>> rules;

  std::map<std::string, std::map<std::string, std::vector<std::string>>>
  parseRobots(std::stringstream *contents);

public:
  static void printMap(
      const std::map<std::string,
                     std::map<std::string, std::vector<std::string>>> &rules,
      int indent = 0);
  std::map<std::string, std::map<std::string, std::vector<std::string>>>
  parseFromFile(const std::string fileName);

  std::map<std::string, std::map<std::string, std::vector<std::string>>>
  parseUrlRobots(const std::string url);
  RobotsParser();
  ~RobotsParser();
};
