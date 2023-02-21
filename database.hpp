#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

class Database {
 public:
  Database(const std::string& db_file);
  ~Database();

  void create_user_table();
  void insert_user(const std::string& email, const std::string& password);
  bool authenticate_user(const std::string& email, const std::string& password);

 private:
  sqlite3* db_;
  int open_result_;
};
