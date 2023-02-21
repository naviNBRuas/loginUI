#pragma once

#include <string>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

class Sandbox {
public:
  static std::string execute(const std::string& program, const std::string& input);
};

std::string Sandbox::execute(const std::string& program, const std
