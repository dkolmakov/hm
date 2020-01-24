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

    SQLite::Database db;

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
        SQLite::Statement query(db, "SELECT * FROM last_commands WHERE sess_id = :sess AND pwd = :pwd;");
        
        query.bind(":sess", sess_id);
        query.bind(":pwd", pwd);

        while (query.executeStep()) {
            cmd = std::string((const char *)query.getColumn(2));
        };
        
        return cmd;
    }

    void set_last_cmd(const std::string& sess_id, const std::string& cwd, const std::string& cmd) {
        SQLite::Statement query(db, "INSERT OR REPLACE INTO last_commands (sess_id, pwd, cmd) VALUES ( :sess, :pwd, :cmd );");

        query.bind(":sess", sess_id);
        query.bind(":pwd", cwd);
        query.bind(":cmd", cmd);
        
        query.exec();
    }

    void get_sess_id_by_name(std::vector<int>& ids, const std::string& sess_name) {
        SQLite::Statement query(db, "SELECT * FROM sessions WHERE name = :name;");
        
        query.bind(":name", sess_name);

        while (query.executeStep()) {
            ids.push_back((int)query.getColumn(0));
        };
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
    
    std::string prepare_select(const std::string& path, bool recursively, const std::string& sess) {
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
        
        return "SELECT * FROM history_fts WHERE history_fts MATCH '" + 
               sess_phrase + " " + conjunction + " " + path_phrase + "'";
    }

    void insert(const std::string& session, const std::string& datetime, const std::string& path, const std::string& cmd, const std::string& rc) {
        SQLite::Statement query(db, "INSERT INTO commands (sess_id,date,pwd,cmd,rc) VALUES ( :sess , :datetime , :pwd , :cmd, :rc );");
        
        query.bind(":sess", session);
        query.bind(":datetime", datetime);
        query.bind(":pwd", path);
        query.bind(":cmd", cmd);
        query.bind(":rc", rc);
        
        query.exec();
    }
    
    
public:

    History(const std::string& path) : db(path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
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
                
        db.exec(sql);
    }

    ~History() {
    }

    void insert_cmd(const std::string& session, const std::string& datetime, const std::string& cwd, const std::string& cmd, const std::string& rc) {
        std::string path = prepare_path_for_search(cwd);
        std::string last_cmd = get_last_cmd(session, path);
        
        if (cmd != last_cmd) {
            insert(session, datetime, path, cmd, rc);
            set_last_cmd(session, path, cmd);
        }
    }

    int64_t insert_sess(const std::string& name) {
        SQLite::Statement query(db, "INSERT INTO sessions (date,name) VALUES (DATETIME(), :name );");
        
        query.bind(":name", name);
        query.exec();

        return db.getLastInsertRowid();
    }

    std::string get_sess_name(const std::string& id) {
        std::string result = "";
        SQLite::Statement query(db, "SELECT * FROM sessions WHERE id = :id;");
        
        query.bind(":id", id);

        while (query.executeStep()) {
            result = (const char *)query.getColumn(2);
        };
        
        return result;
    }
    
    void set_sess_name(const std::string& id, const std::string& sess_name) {
        SQLite::Statement query(db, "UPDATE sessions SET name = :name WHERE id = :id;");

        query.bind(":name", sess_name);
        query.bind(":id", id);

        query.exec();
    }
    
    void select(bool by_dir, const std::string& path, bool recursively, bool by_sess, const std::string& sess) {
        size_t counter = 0;
        std::string select_sql = prepare_select(
                                   (by_dir) ? path : "", recursively, 
                                   (by_sess) ? sess : "");
        SQLite::Statement query(db, select_sql);
        
        while (query.executeStep()) {
            std::cout << (const char *)query.getColumn(3) << std::endl;
            counter++;
        };
        
        std::cerr << "Selected " << counter << " commands..." << std::endl;
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
        SQLite::Transaction transaction(db);

        while (getline(input, line)) {
            std::vector<std::string> elements = split(line, separator);
            
            if (elements.size() != 5) 
                continue; // Drop incorrect lines
            
            insert(elements[0], elements[1], elements[2], elements[3], elements[4]);
            
            current_line++;
            bar.show_progress(current_line);
        }
        
        transaction.commit();
        
        input.close();
        
        return 0;
    }

};

