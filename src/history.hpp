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
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "database.hpp"
#include "query.hpp"
#include "path.hpp"
#include "progress_bar.hpp"
#include "utils.hpp"
#include "schema.hpp"

using schema::ColumnBase;
using schema::Column;
using schema::Schema;

struct HistoryException : UtilException {
    HistoryException(const std::string& _reason) noexcept
        : UtilException("History manager error ", _reason) {}
};

struct LastCommandsSchemaColumnsList {
    const Column<unsigned long int> sess_id{"sess_id", "INTEGER PRIMARY KEY"};
    const Column<std::string> pwd{"pwd", "TEXT"};
    const Column<std::string> cmd{"cmd", "TEXT"};
};

struct LastCommandsSchema : LastCommandsSchemaColumnsList, Schema {
    const std::string select = "SELECT * FROM " + table_name +
                               " WHERE " + sess_id.binding + 
                               " AND " +  pwd.binding + ";";
    const std::string insertOrReplace = "INSERT OR REPLACE INTO " + table_name +
                                        " (" + list + ") VALUES (" + bindingsList + ");";
    const std::string create_table = "CREATE TABLE IF NOT EXISTS " + table_name + " (" +
                                      listWithTypes + ");";
                                      
    LastCommandsSchema() noexcept
        : Schema("last_commands", sess_id, pwd, cmd) {}
};

struct SessionsSchemaColumnsList {
    const Column<unsigned long int> id{"id", "INTEGER PRIMARY KEY"};
    const Column<std::string> date{"date", "TEXT"};
    const Column<std::string> name{"name", "TEXT"};
};

struct SessionsSchema : SessionsSchemaColumnsList, Schema {
    const std::string select_by_name = "SELECT * FROM " + table_name +
                                       " WHERE " + name.binding + ";";
    const std::string select_by_id = "SELECT * FROM " + table_name + 
                                     " WHERE " + id.binding + ";";
    const std::string create_table = "CREATE TABLE IF NOT EXISTS " + table_name + " (" +
                                      listWithTypes + ");";
    const std::string insert = "INSERT INTO " + table_name + 
                               " (" + getList(date, name) + ") VALUES (DATETIME(), " + getBindingsList(name) + ");";
    const std::string update = "UPDATE " + table_name + 
                               " SET " + name.binding + 
                               " WHERE " + id.binding + ";";
                               
    SessionsSchema() noexcept
        : Schema("sessions", id, date, name) {}
};

struct HistoryFtsSchemaColumnsList {
    const Column<unsigned long int> sess_id{"sess_id", ""};
    const Column<std::string> date{"date", ""};
    const Column<std::string> pwd{"pwd", ""};
    const Column<std::string> cmd{"cmd", ""};
};

struct HistoryFtsSchema : HistoryFtsSchemaColumnsList, Schema {
    const std::string create_table = "CREATE VIRTUAL TABLE IF NOT EXISTS " + table_name + " USING \
                                      fts5(" + list + ", tokenize=\"unicode61 tokenchars \'-._/\'\");";

    HistoryFtsSchema() noexcept
        : Schema("history_fts", sess_id, date, pwd, cmd) {}
};

struct CommandsSchemaColumnsList {
    const Column<unsigned long int> sess_id{"sess_id", "BIGINT"};
    const Column<std::string> date{"date", "TEXT"};
    const Column<std::string> pwd{"pwd", "TEXT"};
    const Column<std::string> cmd{"cmd", "TEXT"};
    const Column<std::string> rc{"rc", "TEXT"};
};

struct CommandsSchema : CommandsSchemaColumnsList, Schema {
    const std::string insert = "INSERT INTO " + table_name + 
                               " (" + list + ") VALUES (" + bindingsList + ");";
    const std::string create_table = "CREATE TABLE IF NOT EXISTS " + table_name + " (" +
                                      listWithTypes + ");";
                                      
    CommandsSchema() noexcept
        : Schema("commands", sess_id, date, pwd, cmd, rc) {} 
};


class History {
    const LastCommandsSchema lcSch;
    const SessionsSchema sSch;
    const HistoryFtsSchema ftsSch;
    const CommandsSchema cSch;

    const sqlite::Database db;
    
    std::string prepare_path_for_search(const std::string& input) {
        apathy::Path path(input);
        
        return "\"" + path.absolute().sanitize().trim().string() + "\"";
    }
    
    
    std::vector<std::string> split(const std::string& s, const std::string& separator)
    {
        std::vector<std::string> elements;
        size_t pos = 0;
        size_t prev_pos = 0;
        
        while ((pos = s.find(separator, prev_pos)) != std::string::npos)
        {
            elements.push_back(s.substr(prev_pos, pos - prev_pos));
            prev_pos = pos + separator.size();
        }
        
        elements.push_back(s.substr(prev_pos, s.size() - prev_pos));

        return elements;
    }    

    std::string get_last_cmd(const std::string& sess_id, const std::string& pwd) {
        sqlite::Query query(db, lcSch.select);
        std::string cmd = "";

        query.bind(lcSch.sess_id.bName, sess_id); // TODO: Implement type safe binding
        query.bind(lcSch.pwd.bName, pwd);

        while (query.exec_step()) {
            cmd = query.get_string(lcSch.cmd.getPos());
        };

        return cmd;
    }

    void set_last_cmd(const std::string& sess_id, const std::string& cwd, const std::string& cmd) {
        sqlite::Query query(db, lcSch.insertOrReplace);

        query.bind(lcSch.sess_id.bName, sess_id); // TODO: Implement type safe binding
        query.bind(lcSch.pwd.bName, cwd);
        query.bind(lcSch.cmd.bName, cmd);
        query.exec();
    }

    void get_sess_id_by_name(std::vector<int>& ids, const std::string& sess_name) {
        sqlite::Query query(db, sSch.select_by_name);

        query.bind(sSch.name.bName, sess_name); // TODO: Implement type safe binding

        while (query.exec_step()) {
            ids.push_back(query.get_int(sSch.id.getPos()));
        };
    }

    std::string logic_phrase_from_ids(std::vector<int>& ids) {
        std::string phrase = "";
        
        for (size_t i = 0; i < ids.size(); ++i) {
            phrase += std::to_string(ids[i]);
            
            if (i + 1 < ids.size())
                phrase += " OR ";
        }
    
        return phrase;
    }
    
    std::string prepare_select(const std::string& path, const bool recursively, const std::string& sess) {
        std::string sess_phrase = "";
        
        if (sess.length()) {
            std::vector<int> ids;
            get_sess_id_by_name(ids, sess);
            sess_phrase = ftsSch.sess_id.name + ":(" + logic_phrase_from_ids(ids) + ")";
        }
        
        std::string path_phrase = "";
        if (path.length()) {
            auto dir = prepare_path_for_search(path);

            if (recursively)
                dir += "*";
            
            path_phrase = ftsSch.pwd.name + ": " + dir;
        }

        std::string conjunction = "";
        if (sess_phrase.length() && path_phrase.length())
            conjunction = "AND";
        
        return "SELECT * FROM " + ftsSch.table_name + " WHERE " + ftsSch.table_name + " MATCH '" + 
                sess_phrase + " " + conjunction + " " + path_phrase + "'";
    }

    void insert(const std::string& session, const std::string& datetime, const std::string& path, const std::string& cmd, const std::string& rc) {
        sqlite::Query query(db, cSch.insert);
        
        query.bind(cSch.sess_id.bName, session); 
        query.bind(cSch.date.bName, datetime);
        query.bind(cSch.pwd.bName, path);
        query.bind(cSch.cmd.bName, cmd);
        query.bind(cSch.rc.bName, rc);
        query.exec();
    }
    
    
public:

    History(const std::string& path) : db(path) {
        auto sql = cSch.create_table +
                   sSch.create_table +
                   lcSch.create_table +
                   ftsSch.create_table + 
                   "CREATE TRIGGER IF NOT EXISTS history_update AFTER INSERT ON " + cSch.table_name + " BEGIN \
                            INSERT INTO " + ftsSch.table_name + " (" +
                                ftsSch.list + ") VALUES (" + 
                                ftsSch.getCustomList([](const ColumnBase& v){ return "new." + v.name; }) + 
                            "); \
                   END;";
        db.exec(sql);
    }

    ~History() {
    }

    void insert_cmd(const std::string& session, const std::string& datetime, const std::string& cwd, const std::string& cmd, const std::string& rc) {
        const std::string path = prepare_path_for_search(cwd);
        const std::string last_cmd = get_last_cmd(session, path);
        
        if (cmd != last_cmd) {
            insert(session, datetime, path, cmd, rc);
            set_last_cmd(session, path, cmd);
        }
    }

    int64_t insert_sess(const std::string& name) {
        sqlite::Query query(db, sSch.insert);
        
        query.bind(sSch.name.bName, name);
        query.exec();

        return db.last_rowid();
    }

    std::string get_sess_name(const std::string& id) {
        sqlite::Query query(db, sSch.select_by_id);
        std::string result = "";
        
        query.bind(sSch.id.bName, id);

        while (query.exec_step()) {
            result = query.get_string(sSch.name.getPos());
        };
        
        return result;
    }
    
    void set_sess_name(const std::string& id, const std::string& sess_name) {
        sqlite::Query query(db, sSch.update);

        query.bind(sSch.name.bName, sess_name);
        query.bind(sSch.id.bName, id);
        query.exec();
    }
    
    void select(bool by_dir, const std::string& path, bool recursively, bool by_sess, const std::string& sess) {
        std::string select_sql = prepare_select(
                            (by_dir) ? path : "", recursively, 
                            (by_sess) ? sess : "");
        sqlite::Query query(db, select_sql);
        
        while (query.exec_step()) {
            std::cout << query.get_string(ftsSch.cmd.getPos()) << std::endl;
        };
    }
    
    void parse_input_file(const std::string& filename, const std::string& separator) {
        std::ifstream input(filename.c_str(), std::ios::in);
        std::string line;

        std::ifstream to_count(filename.c_str(), std::ios::in); 
        size_t total_lines = std::count(std::istreambuf_iterator<char>(to_count), std::istreambuf_iterator<char>(), '\n');
        size_t current_line = 0;

        std::cout << "Loading " << total_lines << " commands from the temporary file " << filename << std::endl;
        ProgressBar<size_t> bar(total_lines, current_line);

        if (!input)
            throw HistoryException("Failed to read commands from file " + filename);

        // To speed-up loading process - perform all inserts as single transaction
        db.exec("BEGIN TRANSACTION;");

        while (getline(input, line)) {
            std::vector<std::string> elements = split(line, separator);
            
            if (elements.size() != cSch.size) {
                --total_lines;
                continue; // Drop incorrect lines
            }
            
            insert(elements[0], elements[1], elements[2], elements[3], elements[4]);
            
            current_line++;
            bar.show_progress(current_line);
        }
        
        db.exec("COMMIT;");

        std::cout << "Loaded " << total_lines << " commands." << std::endl;
        
        input.close();
    }

};

