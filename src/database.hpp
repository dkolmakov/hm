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

#include <exception>
#include <string>
#include <sqlite3.h>

namespace SqliteDB {

    
struct SqliteException : public std::exception {
    std::string reason;

    SqliteException(std::string _reason, int rc) {
        reason = "SQLite error " + std::to_string(rc) + ": " + _reason;
    }

    const char * what () const throw () {
        return reason.c_str();
    }
};


class Database {

    sqlite3 *db;

public:

    Database(const std::string path) {
        int rc;

        rc = sqlite3_open(path.c_str(), &db);

        if ( rc ) {
            std::string reason = "can't open database: ";
            reason += sqlite3_errmsg(db);
            SqliteException e(reason, rc);
            throw e;
        }

    }

    ~Database() {
        sqlite3_close(db);
    }

    int exec(const std::string sql) {
        char *zErrMsg = 0;
        int rc;

        rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);

        if( rc != SQLITE_OK ) {
            SqliteException e(zErrMsg, rc);
            sqlite3_free(zErrMsg);
            throw e;
        }

        return rc;
    }


    int prepare_sql(const std::string& sql, sqlite3_stmt **stmt) {
        int rc;

        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, NULL);

        if( rc != SQLITE_OK ) {
            SqliteException e("SQL preparation error", rc);
            throw e;
        }

        return rc;
    }

    int bind_value(sqlite3_stmt *stmt, const char *name, const std::string& value) {
        int index = sqlite3_bind_parameter_index(stmt, name);

        if( index == 0 ) {
            std::string reason = "SQL bind parameter index error: no ";
            reason += name;
            reason += " parameter";
            std::cout << reason << std::endl;
            SqliteException e(reason, -1);
            std::cout << e.reason << std::endl;
            throw e;
        }

        int rc = sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);

        if( rc != SQLITE_OK ) {
            SqliteException e("SQL bind error", rc);
            throw e;
        }

        return rc;
    }

    int last_rowid() {
        return sqlite3_last_insert_rowid(db);
    }
};


class Query {
    Database& db;
    sqlite3_stmt *stmt;
    
public:
    Query(Database& _db, const std::string& sql) : db(_db) {
        db.prepare_sql(sql, &stmt);
    }

    Query(Database& _db, const char *sql) : db(_db) {
        db.prepare_sql(sql, &stmt);
    }
    
    ~Query() {
        sqlite3_finalize(stmt);
    }
    
    void bind(const std::string& str, const std::string& value) {
        db.bind_value(stmt, str.c_str(), value);
    }

    void bind(const char *str, const std::string& value) {
        db.bind_value(stmt, str, value);
    }

    void exec() {
        int result = SQLITE_DONE;
        
        while ((result = sqlite3_step(stmt)) == SQLITE_BUSY) {};
        
        if (result != SQLITE_DONE) {
            SqliteException e("SQL exec error", result);
            throw e;
        }
    }

    bool exec_step() {
        int result = sqlite3_step(stmt);
        
        if (result != SQLITE_ROW && result != SQLITE_DONE) {
            SqliteException e("SQL exec_step error", result);
            throw e;
        }
        
        return result == SQLITE_ROW;
    }

    std::string get_string(int number) {
        return reinterpret_cast<char const*>(sqlite3_column_text(stmt, number));
    }

    int get_int(int number) {
        return sqlite3_column_int(stmt, number);
    }
};

}
