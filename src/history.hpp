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

#include "database.hpp"
#include "path.hpp"

using namespace apathy;

class History {

    SqliteDB db;

    sqlite3_stmt *insert_cmd_stmt;
    sqlite3_stmt *insert_sess_stmt;
    sqlite3_stmt *insert_last_cmd_stmt;
    sqlite3_stmt *select_last_cmd_stmt;
    sqlite3_stmt *select_by_dir_stmt;
    sqlite3_stmt *select_by_dir_rec_stmt;

    std::string prepare_path_for_search(const std::string& input) {
        Path path(input);
        
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

    void set_last_cmd(const std::string& sess_id, const std::string& cwd, const std::string& cmd) {
        db.bind_value(insert_last_cmd_stmt, ":sess", sess_id);
        db.bind_value(insert_last_cmd_stmt, ":pwd", cwd);
        db.bind_value(insert_last_cmd_stmt, ":cmd", cmd);

        while (sqlite3_step(insert_last_cmd_stmt) != SQLITE_DONE) {};

        sqlite3_reset(insert_last_cmd_stmt);
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

        sql = "SELECT * FROM (SELECT * FROM history_fts WHERE pwd MATCH :dir) WHERE pwd = :sdir;";
        db.prepare_sql(sql, &select_by_dir_stmt);

        sql = "SELECT * FROM history_fts WHERE pwd MATCH :dir;";
        db.prepare_sql(sql, &select_by_dir_rec_stmt);
    }

    ~History() {
    }

    void insert_cmd(const std::string& session, const std::string& datetime, const std::string& cwd, const std::string& cmd, const std::string& rc) {
        std::string path = prepare_path_for_search(cwd);
        std::string last_cmd = get_last_cmd(session, path);
        
        if (cmd != last_cmd) {

            db.bind_value(insert_cmd_stmt, ":sess", session);
            db.bind_value(insert_cmd_stmt, ":datetime", datetime);
            db.bind_value(insert_cmd_stmt, ":pwd", path);
            db.bind_value(insert_cmd_stmt, ":cmd", cmd);
            db.bind_value(insert_cmd_stmt, ":rc", rc);

            while (sqlite3_step(insert_cmd_stmt) != SQLITE_DONE) {};

            sqlite3_reset(insert_cmd_stmt);
            
            set_last_cmd(session, path, cmd);
        }
    }

    int64_t insert_sess(const std::string& name) {
        std::string sql = "INSERT INTO sessions (date,name) VALUES (";
        sql += "DATETIME(),";
        sql += "\"" + name + "\");";
        db.exec_sql(sql);

        return db.last_rowid();
    }

    void select_by_dir(const std::string& path, bool recursively) {
        auto dir = prepare_path_for_search(path);

        if (recursively)
            dir += "*";

        db.bind_value(select_by_dir_rec_stmt, ":dir", dir);

        while (sqlite3_step(select_by_dir_rec_stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_text(select_by_dir_rec_stmt, 3) << std::endl;
        };
        
        sqlite3_reset(select_by_dir_rec_stmt);
    }
    
    int parse_input_file(const std::string& filename, const std::string& separator) {
        std::ifstream input(filename.c_str(), std::ios::in);
        std::string line;
        
        if (!input)
            return -11; // TODO: Add meaningful return code
            
        while (getline(input, line)) {
            std::vector<std::string> elements = split(line, separator);
            
            for (auto val : elements)
                std::cout << val << " ";
            std::cout << std::endl;
            
            if (elements.size() < 5) 
                return -12;
            
            insert_cmd(elements[0], elements[1], elements[2], elements[3], elements[4]);
        }
        
        input.close();
        
        return 0;
    }

};

