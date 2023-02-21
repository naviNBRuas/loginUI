#pragma once

#include <string>
#include <chrono>

class Session {
public:
  Session(const std::string& username, const std::chrono::system_clock::time_point& expiration_time);
  std::string get_username() const;
  bool is_expired() const;
private:
  std::string username_;
  std::chrono::system_clock::time_point expiration_time_;
};
