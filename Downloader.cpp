#include "Downloader.hpp"
#include "CustomException.hpp"
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <new>
#include <sstream>

Downloader::Downloader() {
  buffer = nullptr;
  curl_global_init(0);
  dataStream = nullptr;
}
Downloader::~Downloader() {
  if (buffer) {
    delete buffer;
  }
  if (dataStream) {
    delete dataStream;
  }
  curl_easy_cleanup(curl);
  curl_global_cleanup();
}
size_t funcToWrite(char *contents, size_t size, size_t nmemb, void *userData) {
  size_t newLength = size * nmemb;

  try {

    ((std::string *)userData)->append((char *)contents, newLength);

  } catch (std::bad_alloc &e) {
    throw CustomException((char *)e.what());
  }

  return newLength;
}

std::stringstream *Downloader::getUrlString(const std::string url) {

  curl = curl_easy_init();
  CURLcode res;

  if (curl) {
    buffer = new std::string{};
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, funcToWrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      throw CustomException((char *)(curl_easy_strerror(res)));
    }
  }

  dataStream = new std::stringstream{*buffer};

  return dataStream;
}
