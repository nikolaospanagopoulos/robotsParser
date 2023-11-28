#pragma once

#include <exception>
class CustomException : public std::exception {
public:
  char *what() { return message; }

  CustomException(char *msg) : message(msg) {}

private:
  char *message;
};
