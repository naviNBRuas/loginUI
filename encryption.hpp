#pragma once

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>

class Encryption {
public:
  static std::string encrypt(const std::string& plaintext, const std::string& key);
  static std::string decrypt(const std::string& ciphertext, const std::string& key);
private:
  static const int KEY_LENGTH = 32;
};

inline void handle_error() {
  throw std::runtime_error("Encryption error.");
}

std::string Encryption::encrypt(const std::string& plaintext, const std::string& key) {
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  if (!ctx) handle_error();

  if (key.size() != KEY_LENGTH) handle_error();

  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char*>(key.c_str()), NULL)) handle_error();

  std::string ciphertext(plaintext.size() + EVP_CIPHER_CTX_block_size(ctx), '\0');
  int len = 0;
  if (1 != EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()), &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size())) handle_error();

  int ciphertext_len = len;
  if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()) + len, &len)) handle_error();

  ciphertext_len += len;
  ciphertext.resize(ciphertext_len);

  EVP_CIPHER_CTX_free(ctx);

  return ciphertext;
}

std::string Encryption::decrypt(const std::string& ciphertext, const std::string& key) {
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  if (!ctx) handle_error();

  if (key.size() != KEY_LENGTH) handle_error();

  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char*>(key.c_str()), NULL)) handle_error();

  std::string plaintext(ciphertext.size(), '\0');
  int len = 0;
  if (1 != EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plaintext.data()), &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size())) handle_error();

  int plaintext_len = len;
  if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plaintext.data()) + len, &len)) handle_error();

  plaintext_len += len;
  plaintext.resize(plaintext_len);

  EVP_CIPHER_CTX_free(ctx);

  return plaintext;
}
