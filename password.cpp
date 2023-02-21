#include "password.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>

namespace {
  constexpr size_t kSaltSize = 16;
  constexpr size_t kHashSize = 32;
  constexpr size_t kIterations = 100000;
}

std::string hash_password(const std::string& password) {
  unsigned char salt[kSaltSize];
  RAND_bytes(salt, kSaltSize);

  unsigned char hash[kHashSize];
  PKCS5_PBKDF2_HMAC_SHA1(password.c_str(), password.length(), salt, kSaltSize, kIterations, kHashSize, hash);

  std::string result;
  result.resize(kSaltSize + kHashSize);
  std::memcpy(result.data(), salt, kSaltSize);
  std::memcpy(result.data() + kSaltSize, hash, kHashSize);
  return result;
}

bool verify_password(const std::string& password, const std::string& hash) {
  if (hash.length() != kSaltSize + kHashSize) {
    return false;
  }

  const unsigned char* salt = reinterpret_cast<const unsigned char*>(hash.data());
  const unsigned char* stored_hash = reinterpret_cast<const unsigned char*>(hash.data() + kSaltSize);

  unsigned char test_hash[kHashSize];
  PKCS5_PBKDF2_HMAC_SHA1(password.c_str(), password.length(), salt, kSaltSize, kIterations, kHashSize, test_hash);

  return std::memcmp(stored_hash, test_hash, kHashSize) == 0;
}
