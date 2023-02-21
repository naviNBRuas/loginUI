#include "session.hpp"

Session::Session(const std::string& username, const std::chrono::system_clock::time_point& expiration_time)
  : username_(username), expiration_time_(expiration_time)
{}

std::string Session::get_username() const {
  return username_;
}

bool Session::is_expired() const {
  return std::chrono::system_clock::now() >= expiration_time_;
}
