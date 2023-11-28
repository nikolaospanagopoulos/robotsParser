#pragma once
#include <curl/curl.h>
#include <sstream>
#include <string>
class Downloader {

private:
  std::string *buffer;
  std::stringstream *dataStream;
  CURL *curl;

public:
  std::stringstream *getUrlString(const std::string url);
  Downloader();
  ~Downloader();
};
