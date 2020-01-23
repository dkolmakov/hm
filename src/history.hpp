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

#include "path.hpp"
#include "progress_bar.hpp"
#include "SQLiteCpp/SQLiteCpp.h"

class History {

    SqliteDB db;

    sqlite3_stmt *insert_cmd_stmt;
    sqlite3_stmt *insert_sess_stmt;
    sqlite3_stmt *insert_last_cmd_stmt;
    sqlite3_stmt *select_last_cmd_stmt;

   
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
        std::string cmd = "";

        db.bind_value(select_last_cmd_stmt, ":sess", sess_id);
        db.bind_value(select_last_cmd_stmt, ":pwd", pwd);

        while (sqlite3_step(select_last_cmd_stmt) == SQLITE_ROW) {
            cmd = std::string(reinterpret_cast< char const* >(sqlite3_column_text(select_last_cmd_stmt, 2)));
        };
        
        sqlite3_reset(select_last_cmd_stmt);

        return cmd;
    }

    int set_last_cmd(const std::string& sess_id, const std::string& cwd, const std::string& cmd) {
        int result = SQLITE_DONE;
        db.bind_value(insert_last_cmd_stmt, ":sess", sess_id);
        db.bind_value(insert_last_cmd_stmt, ":pwd", cwd);
        db.bind_value(insert_last_cmd_stmt, ":cmd", cmd);

        while ((result = sqlite3_step(insert_last_cmd_stmt)) == SQLITE_BUSY) {};

        sqlite3_reset(insert_last_cmd_stmt);
        
        return result;
    }

    void get_sess_id_by_name(std::vector<int>& ids, const std::string& sess_name) {
        sqlite3_stmt *select_stmt;
        std::string sql = "SELECT * FROM sessions WHERE name = :name;";
        
        db.prepare_sql(sql, &select_stmt);
        db.bind_value(select_stmt, ":name", sess_name);

        while (sqlite3_step(select_stmt) == SQLITE_ROW) {
            ids.push_back(sqlite3_column_int(select_stmt, 0));
        };
        
        sqlite3_reset(select_stmt);
    }

    std::string logic_phrase_from_ids(std::vector<int>& ids) {
        std::string phrase = "";
        
        for (size_t i = 0; i < ids.size(); i++) {
            phrase += std::to_string(ids[i]);
            
            if (i + 1 < ids.size())
                phrase += " OR ";
        }
    
        return phrase;
    }
    
    sqlite3_stmt *prepare_select(const std::string& path, bool recursively, const std::string& sess) {
        sqlite3_stmt *select_stmt;
       
        std::string sess_phrase = "";
        if (sess.length()) {
            std::vector<int> ids;
            get_sess_id_by_name(ids, sess);
            sess_phrase = "sess_id: (" + logic_phrase_from_ids(ids) + ")";
        }
        
        std::string path_phrase = "";
        if (path.length()) {
            auto dir = prepare_path_for_search(path);

            if (recursively)
                dir += "*";
            
            path_phrase = "pwd: " + dir;
        }

        std::string conjunction = "";
        if (sess_phrase.length() && path_phrase.length())
            conjunction = "AND";
        
        std::string sql = "SELECT * FROM history_fts WHERE history_fts MATCH '" + 
                          sess_phrase + " " + conjunction + " " + path_phrase + "'";
        db.prepare_sql(sql, &select_stmt);
        
        return select_stmt;
    }

    int insert(const std::string& session, const std::string& datetime, const std::string& path, const std::string& cmd, const std::string& rc) {
        int result = SQLITE_DONE;
        
        db.bind_value(insert_cmd_stmt, ":sess", session);
        db.bind_value(insert_cmd_stmt, ":datetime", datetime);
        db.bind_value(insert_cmd_stmt, ":pwd", path);
        db.bind_value(insert_cmd_stmt, ":cmd", cmd);
        db.bind_value(insert_cmd_stmt, ":rc", rc);
            
        while ((result = sqlite3_step(insert_cmd_stmt)) == SQLITE_BUSY) {};

        sqlite3_reset(insert_cmd_stmt);
        return result;
    }
    
    
public:

    History(const std::string& path) : db(path) {
        std::string sql;

        sql = "CREATE TABLE IF NOT EXISTS commands (\
				sess_id BIGINT, \
		   		date TEXT, \
				pwd TEXT, \
				cmd TEXT, \
				rc TEXT);"
              "CREATE TABLE IF NOT EXISTS sessions (\
				id INTEGER PRIMARY KEY, \
		   		date TEXT, \
				name TEXT);"
              "CREATE TABLE IF NOT EXISTS last_commands (\
				sess_id INTEGER PRIMARY KEY, \
		   		pwd TEXT, \
				cmd TEXT);"
              "CREATE VIRTUAL TABLE IF NOT EXISTS history_fts USING \
	 		   fts5(sess_id,date,pwd,cmd, tokenize=\"unicode61 tokenchars \'-._/\'\");"
              "CREATE TRIGGER IF NOT EXISTS history_update AFTER INSERT ON commands BEGIN \
                    INSERT INTO history_fts(sess_id,date,pwd,cmd) VALUES (new.sess_id,new.date,new.pwd,new.cmd); \
               END;";
        db.exec_sql(sql);

        sql = "INSERT INTO commands (sess_id,date,pwd,cmd,rc) "
              " VALUES ( :sess , :datetime , :pwd , :cmd, :rc );";
        db.prepare_sql(sql, &insert_cmd_stmt);

        sql = "INSERT OR REPLACE INTO last_commands (sess_id, pwd, cmd) "
              "VALUES ( :sess, :pwd, :cmd );";
        db.prepare_sql(sql, &insert_last_cmd_stmt);

        sql = "SELECT * FROM last_commands WHERE sess_id = :sess AND pwd = :pwd;";
        db.prepare_sql(sql, &select_last_cmd_stmt);
    }

    ~History() {
    }

    int insert_cmd(const std::string& session, const std::string& datetime, const std::string& cwd, const std::string& cmd, const std::string& rc) {
        int result = SQLITE_DONE;
        std::string path = prepare_path_for_search(cwd);
        std::string last_cmd = get_last_cmd(session, path);
        
        if (cmd != last_cmd) {
            result = insert(session, datetime, path, cmd, rc);

            if (result == SQLITE_DONE)
                set_last_cmd(session, path, cmd);
        }
        
        return result;
    }

    int64_t insert_sess(const std::string& name) {
        std::string sql = "INSERT INTO sessions (date,name) VALUES (";
        sql += "DATETIME(),";
        sql += "\"" + name + "\");";
        int rc = db.exec_sql(sql);
        
        if (rc != SQLITE_OK) {
            std::cout << rc << std::endl;
            throw -11;
        }

        return db.last_rowid();
    }

    std::string get_sess_name(const std::string& id) {
        std::string result = "";
        sqlite3_stmt *select_stmt;
        std::string sql = "SELECT * FROM sessions WHERE id = :id;";
        
        db.prepare_sql(sql, &select_stmt);
        db.bind_value(select_stmt, ":id", id);

        while (sqlite3_step(select_stmt) == SQLITE_ROW) {
            result = std::string(reinterpret_cast< char const* >(sqlite3_column_text(select_stmt, 2)));
        };
        
        sqlite3_reset(select_stmt);
        
        return result;
    }
    
    int set_sess_name(const std::string& id, const std::string& sess_name) {
        int result = SQLITE_DONE;
        sqlite3_stmt *set_stmt;
        std::string sql = "UPDATE sessions SET name = :name WHERE id = :id;";
        
        db.prepare_sql(sql, &set_stmt);
        db.bind_value(set_stmt, ":name", sess_name);
        db.bind_value(set_stmt, ":id", id);

        while ((result = sqlite3_step(set_stmt)) == SQLITE_BUSY) { };
        
        sqlite3_reset(set_stmt);
        
        return result;
    }
    
    int select(bool by_dir, const std::string& path, bool recursively, bool by_sess, const std::string& sess) {
        int result = SQLITE_DONE;
        size_t counter = 0;
        
        sqlite3_stmt *select_stmt = prepare_select(
                            (by_dir) ? path : "", recursively, 
                            (by_sess) ? sess : "");
        
        while ((result = sqlite3_step(select_stmt)) == SQLITE_ROW) {
            std::cout << sqlite3_column_text(select_stmt, 3) << std::endl;
            counter++;
        };
        
        sqlite3_reset(select_stmt);
        
        std::cerr << "Selected " << counter << " commands..." << std::endl;
        
        return result;
        
    }
    
    int parse_input_file(const std::string& filename, const std::string& separator) {
        std::ifstream input(filename.c_str(), std::ios::in);
        std::string line;

        std::ifstream to_count(filename.c_str(), std::ios::in); 
        size_t total_lines = std::count(std::istreambuf_iterator<char>(to_count), std::istreambuf_iterator<char>(), '\n');
        size_t current_line = 0;

        std::cout << "Loading " << total_lines << " commands from the temporary file " << filename << std::endl;
        ProgressBar<size_t> bar(total_lines, current_line);

        if (!input)
            return -11; // TODO: Add meaningful return code

        // To speed-up loading process - perform all inserts as single transaction
        db.exec_sql("BEGIN TRANSACTION;");

        while (getline(input, line)) {
            std::vector<std::string> elements = split(line, separator);
            
            if (elements.size() != 5) 
                continue; // Drop incorrect lines
            
            insert(elements[0], elements[1], elements[2], elements[3], elements[4]);
            
            current_line++;
            bar.show_progress(current_line);
        }
        
        db.exec_sql("COMMIT;");
        
        input.close();
        
        return 0;
    }

};

