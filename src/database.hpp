/*
   Copyright 2019 Dmitry Kolmakov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

#include <sqlite3.h>
#include <functional>
#include <memory>
#include <string>

#include "./utils.hpp"

namespace sqlite {

struct SqliteException : UtilException {
  SqliteException(const std::string& _reason, int rc) noexcept
      : UtilException("SQLite error ", _reason, rc) {}
};

class Database {
  const std::unique_ptr<sqlite3, std::function<void(sqlite3*)>> db;

 public:
  explicit Database(const std::string path)
      : db(
            [path]() {
              sqlite3* _db;
              int rc = sqlite3_open(path.c_str(), &_db);

              if (rc != SQLITE_OK) {
                throw SqliteException(
                    "can't open database: " + std::string(sqlite3_errmsg(_db)),
                    rc);
              }

              return _db;
            }(),
            [](sqlite3* _db) { sqlite3_close(_db); }) {}

  void exec(const std::string sql) const {
    char* zErrMsg = nullptr;

    int rc = sqlite3_exec(db.get(), sql.c_str(), NULL, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
      SqliteException e(zErrMsg, rc);
      sqlite3_free(zErrMsg);
      throw e;
    }
  }

  sqlite3_stmt* prepare_sql(const std::string& sql) const {
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
      throw SqliteException("SQL preparation error", rc);
    }

    return stmt;
  }

  void bind_value(sqlite3_stmt* stmt, const std::string& name,
                  const std::string& value) const {
    int index = sqlite3_bind_parameter_index(stmt, name.c_str());

    if (index == 0) {
      throw SqliteException(
          "SQL bind parameter index error: no " + name + " parameter", -1);
    }

    int rc =
        sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);

    if (rc != SQLITE_OK) {
      throw SqliteException("SQL bind error", rc);
    }
  }

  int last_rowid() const { return sqlite3_last_insert_rowid(db.get()); }
};

}  // namespace sqlite
