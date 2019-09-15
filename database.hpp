#pragma once

#include <exception>
#include <string>

#include <sqlite3.h>

struct SqliteException : public std::exception {
    std::string reason;

    SqliteException(std::string _reason, int rc) {
        reason = "SQLite error " + std::to_string(rc) + ": " + _reason;
    }

    const char * what () const throw () {
        return reason.c_str();
    }
};


class SqliteDB {

    sqlite3 *db;

public:

    SqliteDB(std::string path) {
        int rc;

        rc = sqlite3_open(path.c_str(), &db);

        if ( rc ) {
            std::string reason = "can't open database: ";
            reason += sqlite3_errmsg(db);
            SqliteException e(reason, rc);
            throw e;
        }

    }

    ~SqliteDB() {
        sqlite3_close(db);
    }

    int exec_sql(std::string sql) {
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


    int prepare_sql(std::string sql, sqlite3_stmt **stmt) {
        int rc;

        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, NULL);

        if( rc != SQLITE_OK ) {
            SqliteException e("SQL preparation error", rc);
            throw e;
        }

        return rc;
    }

    int bind_value(sqlite3_stmt *stmt, const char* name, std::string value) {
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
