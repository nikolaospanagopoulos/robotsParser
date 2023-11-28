#include "RobotsParser.hpp"
#include "CustomException.hpp"
#include "Downloader.hpp"
#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

void RobotsParser::trimText(std::string &text) const {

  text.erase(text.begin(), std::find_if(text.begin(), text.end(), [](int ch) {
               return !std::isspace(ch);
             }));

  text.erase(std::find_if(text.rbegin(), text.rend(),
                          [](int ch) { return !std::isspace(ch); })
                 .base(),
             text.end());
}

std::string *RobotsParser::splitString(std::string &input,
                                       const char delimeter) const {

  size_t start = 0;
  size_t end = 0;

  return nullptr;
}

RobotsParser::~RobotsParser() {

  if (fileContentsStream) {
    delete fileContentsStream;
  }

  if (inFile.is_open()) {
    inFile.close();
    std::cout << "File closed successfully \n";
  }

  if (buffer) {
    delete[] buffer;
  }
  if (downloader) {
    delete downloader;
  }
}
RobotsParser::RobotsParser()
    : fileContentsStream(nullptr), buffer(nullptr), downloader(nullptr) {}
std::string toLowerCase(const std::string &toMakeLower) {
  std::string toReturn{};
  for (auto &i : toMakeLower) {
    toReturn.push_back(std::tolower(i));
  }
  return toReturn;
}
void RobotsParser::printMap(
    const std::map<std::string, std::map<std::string, std::vector<std::string>>>
        &rules,
    int indent) {

  std::string spacing(indent, ' ');

  std::cout << spacing << "{\n";
  for (const auto &outerPair : rules) {
    std::cout << spacing << "  \"" << outerPair.first << "\": {\n";

    for (const auto &innerPair : outerPair.second) {
      std::cout << spacing << "    \"" << innerPair.first << "\": [";

      const auto &vec = innerPair.second;
      for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << "\"" << vec[i] << "\"";
        if (i < vec.size() - 1) {
          std::cout << ", ";
        }
      }

      std::cout << "],\n";
    }

    std::cout << spacing << "  },\n";
  }
  std::cout << spacing << "}\n";
}
std::map<std::string, std::map<std::string, std::vector<std::string>>>
RobotsParser::parseRobots(std::stringstream *contents) {
  std::string line;
  std::string currentAgent{};
  std::vector<std::string> allowedRules{};
  std::vector<std::string> notAllowedRules{};

  while (std::getline(*contents, line)) {
    trimText(line);
    size_t start = 0;
    size_t end = 0;
    if (line.find("Sitemap") != std::string::npos) {
      continue;
    }
    while ((end = line.find(":", start)) != std::string::npos) {
      std::string key = line.substr(start, end - start);
      start = end + 1;
      std::string value = line.substr(start);
      trimText(value);
      key = toLowerCase(key);
      if (key == "user-agent") {
        currentAgent = value;
        if (rules.find(currentAgent) == rules.end()) {
          rules[currentAgent] =
              std::map<std::string, std::vector<std::string>>{};
        }
      } else if (key == "allow" || key == "disallow") {
        rules[currentAgent][key].push_back(value);
      } else {
        continue;
      }
    }
  }
  // printMap(rules);
  return rules;
}

std::map<std::string, std::map<std::string, std::vector<std::string>>>
RobotsParser::parseFromFile(const std::string fileName) {
  openFile(fileName);
  return parseRobots(fileContentsStream);
}
std::stringstream *RobotsParser::openFile(const std::string &fileName) {
  inFile.open(fileName);
  if (!inFile) {
    throw CustomException((char *)"Couln't open file or file doesnt exist");
  }
  inFile.seekg(0, std::ios::end);
  std::streampos length = inFile.tellg();
  inFile.seekg(0, std::ios::beg);

  buffer = new char[length];
  inFile.read(buffer, length);

  fileContentsStream = new std::stringstream{};

  fileContentsStream->rdbuf()->pubsetbuf(buffer, length);

  return fileContentsStream;
};

std::map<std::string, std::map<std::string, std::vector<std::string>>>
RobotsParser::parseUrlRobots(const std::string url) {
  downloader = new Downloader{};
  std::stringstream *urlContents = downloader->getUrlString(url);

  return parseRobots(urlContents);
}
