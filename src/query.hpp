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

#include <string>
#include <sqlite3.h>
#include <memory>
#include <functional>

#include "utils.hpp"
#include "database.hpp"

namespace sqlite {
    
class Query {
    const Database& db;
    std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt*)>> stmt;
    
public:
    Query(const Database& _db, const std::string& sql) : db(_db), stmt(
            db.prepare_sql(sql),
            [](sqlite3_stmt* _stmt) { sqlite3_finalize(_stmt); }
         ) { }

    void bind(const std::string& str, const std::string& value) const {
        db.bind_value(stmt.get(), str, value);
    }

    void exec() const {
        int result = SQLITE_DONE;
        
        while ((result = sqlite3_step(stmt.get())) == SQLITE_BUSY) {};
        
        if (result != SQLITE_DONE) {
            throw SqliteException("SQL exec error", result);
        }
    }

    bool exec_step() const {
        int result = sqlite3_step(stmt.get());
        
        if (result != SQLITE_ROW && result != SQLITE_DONE) {
            throw SqliteException("SQL exec_step error", result);
        }
        
        return result == SQLITE_ROW;
    }

    std::string get_string(int number) const {
        // sqlite3_column_text() returns const UNSIGNED char*
        return reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), number)); 
    }

    int get_int(int number) const {
        return sqlite3_column_int(stmt.get(), number);
    }
};

}
