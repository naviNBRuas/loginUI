#pragma once

#include <memory>
#include <stdexcept>

template<typename T>
class Memory {
public:
  template<typename... Args>
  static std::unique_ptr<T> make_unique(Args&&... args) {
    try {
      return std::make_unique<T>(std::forward<Args>(args)...);
    }
    catch(const std::bad_alloc&) {
      throw std::runtime_error("Memory allocation failed.");
    }
  }
};
