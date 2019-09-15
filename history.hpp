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
#include <iostream>

#include "database.hpp"

class History {

    SqliteDB db;

    sqlite3_stmt *insert_cmd_stmt;
    sqlite3_stmt *insert_sess_stmt;
    sqlite3_stmt *insert_last_cmd_stmt;
    sqlite3_stmt *select_last_cmd_stmt;
    sqlite3_stmt *select_by_dir_stmt;

public:

    History(std::string path) : db(path) {
        std::string sql;

        sql = "CREATE TABLE IF NOT EXISTS commands (\
				sess_id BIGINT, \
		   		date TEXT, \
				pwd TEXT, \
				cmd TEXT);"
              "CREATE TABLE IF NOT EXISTS sessions (\
				id INTEGER PRIMARY KEY, \
		   		date TEXT, \
				name TEXT);"
              "CREATE TABLE IF NOT EXISTS last_commands (\
				sess_id INTEGER PRIMARY KEY, \
		   		pwd TEXT, \
				cmd TEXT);"
              "CREATE VIRTUAL TABLE IF NOT EXISTS history_fts USING \
	 		 fts5(content=commands,pwd);";

        db.exec_sql(sql);

        sql = "INSERT INTO commands (sess_id,date,pwd,cmd) "
              " VALUES ( :sess , DATETIME() , :pwd , :cmd );";
        db.prepare_sql(sql, &insert_cmd_stmt);

        sql = "INSERT OR REPLACE INTO last_commands (sess_id, pwd, cmd) "
              "VALUES ( :sess, :pwd, :cmd );";
        db.prepare_sql(sql, &insert_last_cmd_stmt);

        sql = "SELECT * FROM last_commands WHERE sess_id = :sess AND pwd = :pwd;";
        db.prepare_sql(sql, &select_last_cmd_stmt);

	sql = "SELECT * FROM history_fts WHERE pwd MATCH :dir;";
        db.prepare_sql(sql, &select_by_dir_stmt);
    }

    ~History() {
    }

    std::string get_last_cmd(int sess_id, std::string pwd) {
        std::string cmd = "";

        db.bind_value(select_last_cmd_stmt, ":sess", std::to_string(sess_id));
        db.bind_value(select_last_cmd_stmt, ":pwd", pwd);

        while (sqlite3_step(select_last_cmd_stmt) == SQLITE_ROW) {
            cmd = std::string(reinterpret_cast< char const* >(sqlite3_column_text(select_last_cmd_stmt, 2)));
        };

        return cmd;
    }

    void set_last_cmd(int sess_id, std::string pwd, std::string cmd) {
        db.bind_value(insert_last_cmd_stmt, ":sess", std::to_string(sess_id));
        db.bind_value(insert_last_cmd_stmt, ":pwd", pwd);
        db.bind_value(insert_last_cmd_stmt, ":cmd", cmd);

        while (sqlite3_step(insert_last_cmd_stmt) != SQLITE_DONE) {};

    }

    void insert_cmd(int64_t session, std::string pwd, std::string cmd) {
        std::string last_cmd = get_last_cmd(session, pwd);

        if (cmd != last_cmd) {

            db.bind_value(insert_cmd_stmt, ":sess", std::to_string(session));
            db.bind_value(insert_cmd_stmt, ":pwd", pwd);
            db.bind_value(insert_cmd_stmt, ":cmd", cmd);

            while (sqlite3_step(insert_cmd_stmt) != SQLITE_DONE) {};

            set_last_cmd(session, pwd, cmd);
        }
    }

    int64_t insert_sess(std::string name) {
        std::string sql = "INSERT INTO sessions (date,name) VALUES (";
        sql += "DATETIME(),";
        sql += "\"" + name + "\");";
        db.exec_sql(sql);

        return db.last_rowid();
    }

    void select_by_dir(std::string dir) {
	db.bind_value(select_by_dir_stmt, ":dir", dir);

	while (sqlite3_step(select_by_dir_stmt) == SQLITE_ROW) {
        	std::cout << sqlite3_column_text(select_by_dir_stmt, 3) << std::endl;
    	};
    }

};

