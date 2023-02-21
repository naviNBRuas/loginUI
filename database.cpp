#include "database.hpp"
#include <iostream>

Database::Database(const std::string& db_file) {
  open_result_ = sqlite3_open(db_file.c_str(), &db_);
  if (open_result_ != SQLITE_OK) {
    std::cerr << "Failed to open database: " << sqlite3_errmsg(db_) << std::endl;
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

Database::~Database() {
  if (db_) {
    sqlite3_close(db_);
  }
}

void Database::create_user_table() {
  const char* sql = "CREATE TABLE IF NOT EXISTS users (email TEXT PRIMARY KEY, password TEXT)";
  char* error_msg = nullptr;
  int result = sqlite3_exec(db_, sql, nullptr, nullptr, &error_msg);
  if (result != SQLITE_OK) {
    std::cerr << "Failed to create users table: " << error_msg << std::endl;
    sqlite3_free(error_msg);
  }
}

void Database::insert_user(const std::string& email, const std::string& password) {
  const char* sql = "INSERT INTO users (email, password) VALUES (?, ?)";
  sqlite3_stmt* stmt;
  int result = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
  if (result == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
      std::cerr << "Failed to insert user: " << sqlite3_errmsg(db_) << std::endl;
    }
  } else {
    std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db_) << std::endl;
  }
  sqlite3_finalize(stmt);
}

bool Database::authenticate_user(const std::string& email, const std::string& password) {
  const char* sql = "SELECT password FROM users WHERE email = ?";
  sqlite3_stmt* stmt;
  int result = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
  if (result == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
      const unsigned char* db_password = sqlite3_column_text(stmt, 0);
      return password == reinterpret_cast<const char*>(db_password);
    }
  } else {
    std::cerr << "Failed to prepare select statement: " << sqlite3_errmsg(db_) << std::endl;
  }
  sqlite3_finalize(stmt);
  return false;
}
